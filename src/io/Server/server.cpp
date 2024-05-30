#include "./server.hpp"

Server::Server(t_c_global_config *config)
{
	this->global_config = config;
	config_epoll(config->get_ports());
	server_loop();
}

Server::Server(std::set<uint16_t> ports)
{
	config_epoll(ports);
	server_loop();
}

Server::~Server()
{
	close(epollfd);
	for (std::map<int, MySocket *>::iterator iter = socket_map.begin(); iter != socket_map.end(); iter++)
		delete iter->second;
}

void Server::config_epoll(std::set<uint16_t> ports)
{
	if ((epollfd = epoll_create(ports.size())) == -1)
	{
		perror("Epoll_create");
		exit(EXIT_FAILURE);
	}

	for (std::set<uint16_t>::iterator iter = ports.begin(); iter != ports.end(); iter++)
	{
		MySocket *socket = new MySocket(*iter);
		std::cout << "Socket for port: " << *iter << " created with fd " << socket->getSockfd() << std::endl;
		epoll_add(socket->getSockfd(), EPOLLIN | EPOLLOUT);
		socket_map[socket->getSockfd()] = socket;
	}
}

void Server::epoll_add(int fd, uint32_t mask)
{
	struct epoll_event event;

	memset(&event, 0, sizeof(event));
	event.events = mask;
	event.data.fd = fd;

	if (epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event) == -1)
	{
		perror("Epoll_ctl");
		exit(EXIT_FAILURE);
	}
	std::cout << fd << " added to poll" << std::endl;
}

void Server::server_loop(void)
{
	int 				confd;
	int 				sockfd;
	struct epoll_event	events[MAX_EVENTS];
	int					nevents;

	while (true)
	{
		std::cout << "epoll waiting ...\n";
		if ((nevents = epoll_wait(epollfd, events, sizeof(events), -1)) == -1)
		{
			perror("Epoll wait");
			exit(EXIT_FAILURE);
		}
		std::cout << "epoll returned " << nevents << " events\n";
		for (int n = 0; n < nevents; n++)
		{
			sockfd = events[n].data.fd;
			std::cout << sockfd << std::endl;
			std::cout << "event " << events[n].events << " in: " << sockfd << std::endl;
			if (socket_map.find(sockfd) != socket_map.end())
			{
				int size = 1;
				std::cout << "Accepting new connection...\n";
				confd = accept(sockfd, NULL, NULL);
				if (confd < 0)
				{
					perror("Accept");
					exit(EXIT_FAILURE);
				}
				setsockopt(confd, SOL_SOCKET, SOCK_NONBLOCK, &size, sizeof(int));
				epoll_add(confd, EPOLLIN | EPOLLET);
				client_port_map[confd] = socket_map[sockfd]->getPort();
				std::cout << "New client: " << confd << " Accepted in port " << socket_map[sockfd]->getPort() << std::endl;
			}
			else
			{
				std::cout << "A request has been received...\n";
				manage_request(sockfd);
				socket_map.erase(sockfd);
				close(sockfd);
			}
			std::cout << "end of the loop" << std::endl;
		}
	}
}

void Server::manage_request(int fd)
{
	std::string recv_str;
	char        buffer[BUFFER_SIZE];
	int         nbytes;
	std::string line;

	recv_str.clear();
	while ((nbytes = recv(fd, buffer, BUFFER_SIZE - 1, MSG_DONTWAIT)) > 0)
	{
		buffer[nbytes] = 0;
		recv_str += buffer;
	}
	std::cout << nbytes << std::endl;
	if (nbytes == -1 && recv_str.length() > 0) // EWOULDBLOCK o se ha terminado de leer.
	{
		std::cout << recv_str << std::endl;
		try
		{
			t_c_individual_server_config server_config = select_config(fd, recv_str);
			//ReturnType engine(request, config)
			//send_response(response, fd);
		}
		catch (std::string &error_file)
		{
			send_response(open(error_file.c_str(), O_RDONLY), fd);
			std::cout << "Error message sent\n";
			return;
		}	
	}
	else
		perror("recv");
}

static std::string get_header_value(std::string name, std::string request) // Gets the trimmed value of a header.
{
	size_t start, end;
	std::string value;

	if((start = request.find(name + ":")) == std::string::npos)
		return("");
	start += std::string(name + ":").length();
	end = request.find("\r\n", start);
	value = request.substr(start, end - start);
	while(isspace(value.front()))
		value.erase(0,1);
	while(isspace(value.back()))
		value.erase(value.length()-1, 1);
	return(value);
}

t_c_individual_server_config const &Server::select_config(int fd, std::string request)
{
	int port = client_port_map[fd];
	std::string hostname;
	
	if((hostname = get_header_value("Host", request)) == "") 
		throw 400;
	hostname = hostname.substr(0, hostname.find(":"));
	std::cout << "Parsing request. Port: " << port << " Hostname: " << hostname << std::endl;
	
	std::set<t_c_individual_server_config, std::less<>> servers = global_config->get_servers();
	t_c_individual_server_config::t_c_light_key key(&hostname, port);
	std::set<t_c_individual_server_config, std::less<>>::iterator config = servers.find(key);
	
	if (config != servers.end())
	{
		if(get_header_value("Content-length", request) != "")
		{
			size_t content_length = std::atoll(get_header_value("Content-length", request).c_str());
			if (content_length > config->get_client_body_size_limit())
				throw 413;//handle_error(413, *config);
			return (*config);
		}
	}
	throw 400;//handle_invalid_request();
}

void Server::send_response(int response_fd, int socketfd)
{
	std::string response = "";
	size_t nbytes;
	char buf[BUFFER_SIZE];

	while((nbytes = read(response_fd, buf, BUFFER_SIZE)) > 0)
	{
		buf[nbytes] = 0;
		response += buf;
	}
	if (nbytes != 0)
	{
		perror("Response file");
		return;
	}
	if(send(socketfd, &response, response.length(), MSG_DONTWAIT) == -1)
		perror("Write");
}

t_c_global_config *Server::getGlobalConfig(void)
{
	return this->global_config;
}

int Server::getEpoll(void)
{
	return this->epollfd;
}

std::map<int, MySocket *> &Server::getSocket_Map(void)
{
	return this->socket_map;
}

std::map<int, uint16_t> &Server::getClientPortMap(void)
{
	return client_port_map;
}
