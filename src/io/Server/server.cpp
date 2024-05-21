#include "./server.hpp"

// Server::Server(t_c_global_config config)
// {
// 	this->global_config = &config;
// 	config_epoll(config.get_ports());
// 	server_loop();
// }

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
		epoll_add(socket->getSockfd(), EPOLLIN | EPOLLET);
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
			std::cout << "event in: " << sockfd << std::endl;
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
				std::cout << confd << std::endl;
				epoll_add(confd, EPOLLIN | EPOLLET);
				client_port_map[confd] = socket_map[sockfd]->getPort();
				std::cout << "New client: " << confd << " Accepted in port " << socket_map[sockfd]->getPort() << std::endl;
			}
			else
			{
				std::cout << "A request has been received...\n";
				manage_request(sockfd);
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

	recv_str.clear();
	while ((nbytes = recv(fd, buffer, BUFFER_SIZE - 1, MSG_DONTWAIT)) > 0)
	{
		buffer[nbytes] = 0;
		recv_str += buffer;
	}
	std::cout << nbytes << std::endl;
	if (nbytes == -1) // EWOULDBLOCK o se ha terminado de leer.
	{
		std::cout << recv_str << std::endl;
		select_config(fd, recv_str);
		std::string str; // Send (standard message so far).
		str = "HTTP/1.1 200 OK\r\n\r\n<html><body>Hello</body></html>\r\n\r\n";
		send(fd, str.c_str(), str.length(), 0);
		std::cout << "Message sent\n";
	}
	else
		perror("recv");
	socket_map.erase(fd);
	close(fd);
}

std::vector<std::string> split_request(std::string str, std::string delimiter)
{
	std::vector<std::string> result;
	std::string::size_type start = 0;
	std::string::size_type end;

	while((end = str.find(delimiter, start) != str.npos))
	{
		result.push_back(str.substr(start, end - start));
		start = end + 2;
	}
	result.push_back(str.substr(start));
	return(result);
}

void	Server::select_config(int fd, std::string request)
{
	uint16_t port = client_port_map[fd];
	std::string host_name;

	//std::vector<std::string> tokens = split_request(request, "\r\n");
	// for(std::vector<std::string>::iterator iter = tokens.begin(); iter != tokens.end(); iter++)
	// {
	// 	if (iter->find("Host: ", strlen("Host: ")) == 0)
	// 	{
	// 		host_name = iter->substr(6, iter->find(':', 6));
	// 		break;
	// 	}
	// 	std::cout << host_name << std::endl;
	// 	std::cout << port << std::endl;
	// }
	//Buscar en global config.

}

// t_c_global_config *Server::getGlobalConfig(void)
// {
// 	return this->global_config;
// }

int Server::getEpoll(void)
{
	return this->epollfd;
}

std::map<int, MySocket *> &Server::getSocket_Map(void)
{
	return this->socket_map;
}