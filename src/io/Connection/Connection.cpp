#include "./Connection.hpp"
#include <cerrno>
#include <cstdlib>

static std::string tolower_str(std::string input)
{
	std::string result;
	for (std::string::iterator it = input.begin(); it != input.end(); it++)
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
	while (isspace(value[0]))
	{
		value.erase(0, 1);
	}
	while (isspace(value[value.size() - 1]))
	{
		value.erase(value.length() - 1, 1);
	}
	// std::cout << value << std::endl;
	return (value);
}

Connection::Connection(uint16_t port, t_c_global_config *global_config, ReturnType &resp)
	: port(port), response(resp), bytes_sent(0)
{
	this->global_config = global_config;
	this->config = NULL;
	ready_to_send = false;
	sent_response = false;
}

Connection::~Connection()
{
	if (response.get_fd() != -1)
	{
		close(response.get_fd());
	}
	close(confd);
	std::cout << "Connection in fd " << confd << " closed\n";
}

int Connection::accept_connection(int sockfd)
{
	// int       size = 1;
	socklen_t addrlen = sizeof(address);

	confd = accept(sockfd, (struct sockaddr *)&address, &addrlen);
	if (confd < 0)
	{
		perror("Accept connection");
	}
	else
	{
		if (fcntl(confd, F_SETFL, fcntl(confd, F_GETFL, 0) | O_NONBLOCK) == -1)
		{
			perror("fcntl O_NONBLOCK");
			return (-1);
		}
		if (fcntl(confd, F_SETFD, fcntl(confd, F_GETFD, 0) | FD_CLOEXEC) == -1)
		{
			perror("fcntl FD_EXEC");
			return (-1);
		}
		std::cout << "New connection accepted in port " << ntohs(port) << " fd: " << confd << std::endl;
	}
	// std::cout << "End of accept " << fcntl(confd, F_GETFL, 0) << "\n";
	time(&last_activity);
	return (confd);
}

int Connection::read_request(void)
{
	ssize_t nbytes;
	char    buffer[BUFFER_SIZE];

	if ((nbytes = recv(confd, buffer, BUFFER_SIZE - 1, MSG_DONTWAIT)) > 0)
	{
		buffer[nbytes] = 0;
		request_buffer += std::string(buffer, nbytes);
	}
	if (nbytes < 0)
	{
		return (-1);
	}
	time(&last_activity);
	// std::cout << nbytes << "bytes read\n";
	// std::cout << "Current buffer: " << request_buffer << std::endl;
	return (0);
}

void Connection::select_config(void)
{
	try
	{
		std::string hostname = get_header_value("host", request_buffer);
		if (hostname == "")
		{
			throw 400;
		}
		hostname = hostname.substr(0, hostname.find(":"));

		t_c_individual_server_config::t_c_light_key                   key(&hostname, port);
		std::set<t_c_individual_server_config, std::less<> >::iterator config_it =
			global_config->get_servers().find(key);
		if (config_it == global_config->get_servers().end())
		{
			throw 400;
		}
		config = &(*config_it);
	}
	catch (...)
	{
		throw handle_invalid_request();
	}
}

int Connection::generate_response(void)
{
	// std::cout << "request:\n\"" << request_buffer << '\"' << std::endl;
	if (response == ReturnType(-1, "", NO_CHILD))
	{
		response = handle_request(request_buffer, *config, ((struct sockaddr_in *)&address)->sin_addr);
	}
	ready_to_send = response.get_fd() < 0;
	response_buffer = response.get_headers();
	if (!ready_to_send && !response.is_cgi())
	{
		return (build_response(response.get_fd()));
	}
	return (response.get_fd());
}

int Connection::generate_timeout_response(void)
{
	close(response.get_fd());
	kill(response.get_child_pid(), SIGTERM);
	response = handle_error(408, *config);
	ready_to_send = response.get_fd() < 0;
	response_buffer = response.get_headers();
	if (!ready_to_send)
	{
		return (build_response(response.get_fd()));
	}
	time(&last_activity);
	return (response.get_fd());
}

int Connection::build_response(void) // For CGI (goes through epoll)
{
	char    buffer[BUFFER_SIZE];
	ssize_t nbytes;

	if ((nbytes = read(response.get_fd(), buffer, BUFFER_SIZE - 1)) > 0)
	{
		buffer[nbytes] = 0;
		response_buffer.append(std::string(buffer, nbytes));
	}
	else
	{
		if (nbytes < 0)
		{
			perror("Response file");
		}
		ready_to_send = !nbytes;
	}
	return (nbytes);
}

int Connection::build_response(int fd)
{
	ssize_t size = get_file_size(fd);
	if (size == -1)
	{
		perror("Response file size");
		return (-1);
	}
	if (!size)
	{
		ready_to_send = true;
		return (size);
	}
	void *file_ptr = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (file_ptr == MAP_FAILED)
	{
		perror("Response file map");
		return (-1);
	}
	response_buffer.append(std::string(static_cast<char *>(file_ptr), size));
	ready_to_send = true;
	if (munmap(file_ptr, size) == -1)
	{
		perror("Unmap response file");
	}
	return (size);
}

int Connection::send_response(void)
{
	ssize_t nbytes;

	nbytes = send(confd, response_buffer.c_str() + bytes_sent, response_buffer.length() - bytes_sent, MSG_DONTWAIT);
	if (nbytes < 0)
	{
		perror("Send");
		return (-1);
	}
	bytes_sent += nbytes;
	if (bytes_sent == response_buffer.length())
	{
		sent_response = true;
	}
	time(&last_activity);
	return (0);
}

void Connection::check_body_length(void) const
{
	if (config->get_client_body_size_limit() != UINT64_MAX && get_header_value("content-length", request_buffer) != "")
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
		// std::cout << "Still reading headers\n";
		return (false);
	}
	size_t content_length = std::atoll(get_header_value("content-length", request_buffer).c_str());
	size_t body_length = request_buffer.length() - request_buffer.find("\r\n\r\n") - 4;
	// std::cout << "Content-length:" << content_length << "body_length:" << body_length << std::endl;
	if (config == NULL)
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
			// Error in checks or config -> error response -> don't read body.
			std::cout << "Error response set\n";
			response = error_response;
			return (true);
		}
	}
	// std::cout << "Field: " << content_length << "\t body: " << body_length << std::endl;
	if (content_length > body_length)
	{
		// std::cout << "Not finished: " << request_buffer << std::endl;
		return (false);
	}
	if (content_length < body_length)
	{
		request_buffer.erase(request_buffer.find("\r\n\r\n") + 4 + content_length);
	}
	return (true);
}

bool Connection::response_sent(void) const
{
	return (sent_response);
}

int Connection::child_error(void)
{
	int status;

	if (waitpid(response.get_child_pid(), &status, WNOHANG) > 0)
	{
		if (WIFEXITED(status))
		{
			if (WEXITSTATUS(status) == EXIT_FAILURE)
			{
				close(response.get_fd());
				response = handle_error(500, *config);
				ready_to_send = response.get_fd() < 0;
				response_buffer = response.get_headers();
				if (!ready_to_send)
				{
					return (build_response(response.get_fd()));
				}
			}
			ready_to_send = true;
			return (0);
		}
	}
	return (0);
}

int Connection::getConFd(void) const
{
	return (confd);
}

bool Connection::response_ready(void) const
{
	return (ready_to_send);
}

struct sockaddr_in const &Connection::getAddress(void) const
{
	return (address);
}

std::string Connection::getRequestBuffer(void) const
{
	return (request_buffer);
}

std::string Connection::getResponseBuffer(void) const
{
	return (response_buffer);
}

ReturnType const &Connection::getResponse(void) const
{
	return (response);
}

t_c_global_config const *Connection::getGlobalConfig(void) const
{
	return (global_config);
}

t_c_individual_server_config const *Connection::getConfig(void) const
{
	return (config);
}

time_t Connection::getLastActivity(void) const
{
	return (last_activity);
}

void Connection::set_ready(void)
{
	ready_to_send = true;
}
