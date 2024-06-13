#include "./Connection.hpp"

static std::string tolower_str(std::string input)
{
	std::string result;
	for(std::string::iterator it = input.begin(); it != input.end(); it++)
	{
		result.push_back(tolower(*it));
	}
	return result;
}

static std::string get_header_value(std::string name, std::string request) // Gets the trimmed value of a header.
{
        size_t      start, end;
        std::string value;

        if ((start = tolower_str(request).find(tolower_str(name) + ":")) == std::string::npos)
        {
                return (std::string());
        }
        start += std::string(name).length() + 1;
        end = request.find("\r\n", start);
        value = request.substr(start, end - start);
        while (isspace(value.front()))
        {
                value.erase(0, 1);
        }
        while (isspace(value.back()))
        {
                value.erase(value.length() - 1, 1);
        }
	//std::cout << value << std::endl;
        return (value);
}

Connection::Connection(uint16_t port, t_c_global_config *global_config, ReturnType &resp) : 
	port(port), response(resp), bytes_sent(0)
{
	this->global_config = global_config;
	this->config = NULL;
	memset(&address, 0, sizeof(address));
	ready_to_send = false;
	sent_response = false;
	header_sent = false;
}

Connection::~Connection()
{
	//std::cout << "Connection in fd " << confd << "closed\n";
	close(confd);
}

int Connection::accept_connection(int sockfd)
{
	int size = 1;
	socklen_t addrlen;

	confd = accept(sockfd, &address, &addrlen);
	if (confd < 0)
	{
		perror("Accept connection");
	}
	else
	{
		setsockopt(confd, SOL_SOCKET, SOCK_NONBLOCK, &size, sizeof(int));
		setsockopt(confd, SOL_SOCKET, SOCK_CLOEXEC, &size, sizeof(int));
		std::cout << "New connection accepted in port " << port << " fd: " << confd << std::endl;
	}
	return (confd);
}

int Connection::read_request(void)
{
	ssize_t nbytes;
	char	buffer[BUFFER_SIZE];

	if((nbytes = recv(confd, buffer, BUFFER_SIZE - 1, MSG_DONTWAIT)) > 0)
	{
		buffer[nbytes] = 0;
		request_buffer += buffer;
	}
	if (nbytes < 0)
	{
		return (-1);
	}
	//std::cout << nbytes << "bytes read\n";
	//std::cout << "Current buffer: " << request_buffer << std::endl;
	return(0);
}

void Connection::select_config(void)
{
	std::string hostname = get_header_value("host", request_buffer);
        if (hostname == "")
        {
                throw handle_invalid_request();
        }
        hostname = hostname.substr(0, hostname.find(":"));
        try
	{
		t_c_individual_server_config::t_c_light_key key(&hostname, port);
		std::set<t_c_individual_server_config, std::less<>>::iterator config_it = global_config->get_servers().find(key);
        	if (config_it == global_config->get_servers().end())
        	{
                	throw handle_invalid_request();
        	}
       		config = &(*config_it);
	}
	catch(...)
	{
		throw handle_invalid_request();
	}
}

void Connection::generate_response(void)
{
	std::cout << "request:" << request_buffer << std::endl;
	if (response == ReturnType(-1, "", NO_CHILD))
	{
		response = handle_request(request_buffer, *config, ((struct sockaddr_in *)(&address))->sin_addr);
	}
	ready_to_send = true;
}

int Connection::send_response(void)
{
	ssize_t	nbytes;
	char	buffer[BUFFER_SIZE];
	if (response.is_cgi())
	{
		waitpid(response.get_child_pid(), NULL, 0);
	}
	if (!header_sent)
	{
		if (bytes_sent < response.get_headers().length())
		{
			// Send headers check errors.
			nbytes = send(confd, response.get_headers().c_str(), response.get_headers().length(), MSG_DONTWAIT);
                	if (nbytes < 0)
                	{	
                        	perror("Send");
                        	return (-1);
                	}
                	bytes_sent += nbytes;
		}
		if (bytes_sent == response.get_headers().length())
		{
			//Headers finished -> prepare to send body.
			bytes_sent = 0;
			header_sent = true;
			response_buffer.clear();
		}
	}
	else if(!sent_response)
	{
		if (response_buffer.empty() && response.get_fd() > 0)
		{
			//Read response fd to buffer.
			while((nbytes = read(response.get_fd(), buffer, BUFFER_SIZE - 1)) != 0)
                	{
                        	if (nbytes < 0)
                        	{		
                                	perror("Response fd");
                                	response_buffer.clear();
                                	return (-1);
                        	}
                        	buffer[nbytes] = 0;
                        	response_buffer.append(buffer, nbytes);
                	}
		}
		if (bytes_sent < response_buffer.length())
		{
			//Send buffer to socket.
			nbytes = send(confd, response_buffer.c_str(), response_buffer.length(), MSG_DONTWAIT);
                	if (nbytes < 0)
                	{
                        	perror("Send");
                        	return (-1);
                	}
                	bytes_sent += nbytes;
	}
		if (bytes_sent == response_buffer.length())
		{
			//All has been sent.
			sent_response = true;
		}
	}
	return(0);
}

void Connection::check_body_length(void) const
{
        if (config->get_client_body_size_limit() != UINT64_MAX &&
                        get_header_value("content-length", request_buffer) != "")
        {
                size_t content_length = std::atol(get_header_value("content-length", request_buffer).c_str());
                if (content_length > config->get_client_body_size_limit())
                {
                        throw handle_error(413, *config);
                }
        }
}

void Connection::check_not_chunked(void) const
{
	std::string te = get_header_value("transfer-encoding", request_buffer);
	if (te != "" && te.find("chunked") != std::string::npos)
	{
		throw handle_error(411, *config);
	}
}

bool Connection::headers_read(void) const
{
	return (request_buffer.find("\r\n\r\n") != std::string::npos);
}

bool Connection::request_read(void)
{
	if (!headers_read())
	{
		//std::cout << "Still reading headers\n";
		return (false);
	}
	size_t content_length = std::atoll(get_header_value("content-length", request_buffer).c_str());
	size_t body_length = request_buffer.length() - request_buffer.find("\r\n\r\n") - 4;
	//std::cout << "Content-length:" << content_length << "body_length:" << body_length << std::endl;
	if (!config) 
	{
		// The headers are read -> choose config and check headers.
		try
		{
			select_config();
			check_body_length();
			check_not_chunked();
		}
		catch (ReturnType &error_response)
		{	
			//Error in checks or config -> error response -> don't read body.
			std::cout << "Error response set\n";
			response = error_response;
			return(true);
		}
	}
	//std::cout << "Field: " << content_length << "\t body: " << body_length << std::endl;
	if (content_length > body_length)
	{
		//std::cout << "Not finished: " << request_buffer << std::endl;
		return(false);
	}
	else if (content_length < body_length)
	{
		request_buffer.erase(request_buffer.find("\r\n\r\n") + 4 + content_length);
		return (true);
	}
	else
	{
		return (true);
	}
}

bool Connection::response_sent(void) const
{
	return (sent_response);
}

int Connection::getConFd(void) const
{
	return (confd);
}

bool Connection::response_ready(void) const
{
	return (ready_to_send);
}

struct sockaddr const &Connection::getAddress(void) const
{
	return (address);
}

std::string Connection::getRequestBuffer(void) const
{
	return (request_buffer);
}

ReturnType const &Connection::getResponse(void) const
{
	return(response);
}

t_c_global_config const *Connection::getGlobalConfig(void) const
{
	return (global_config);
}

t_c_individual_server_config const *Connection::getConfig(void) const
{
	return (config);
}
