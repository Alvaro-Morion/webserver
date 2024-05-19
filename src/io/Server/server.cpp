#include "./server.hpp"

Server::Server(t_c_global_config config)
{
	this->global_config = &config;
	config_epoll(config.get_ports());
	server_loop();
}

Server::~Server()
{
	//Closes accepted connections and deletes sockets.
	for(std::map<int, int>::iterator iter = conn_sock_map.begin(); iter != conn_sock_map.end(); iter++)
		close(iter->first);
	close(epollfd);
	for(std::map<int, MySocket *>::iterator iter = socket_map.begin(); iter != socket_map.end(); iter++)
		delete iter->second;
}

void Server::config_epoll(std::set<uint16_t> ports)
{
	if ((epollfd = epoll_create(ports.size())) == -1)
	{
		perror("Epoll_create: ");
		exit(EXIT_FAILURE);
	}

	for (std::set<uint16_t>::iterator iter = ports.begin(); iter != ports.end(); iter++)
	{
		MySocket *socket = new MySocket(*iter);
		epoll_add(socket->getSockfd());
		socket_map[socket->getSockfd()] = socket;
	}
}

void Server::epoll_add(int fd)
{
	struct epoll_event event;

	memset(&event, 0, sizeof(event));
	event.events = EPOLLIN | EPOLLOUT;
	event.data.fd = fd;

	if (epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event) == -1)
	{
		perror("Epoll_ctl: ");
		exit(EXIT_FAILURE);
	}
}

void Server::server_loop(void)
{
	int confd;
	int sockfd;

	while (true)
	{
		if (epoll_wait(epollfd, events, sizeof(events), -1) == -1)
		{
			perror("Epoll wait: ");
			exit(EXIT_FAILURE);
		}
		for (int n = 0; n < MAX_EVENTS; n++)
		{
			sockfd = events[n].data.fd;
			if (socket_map.find(sockfd) == socket_map.end())
			{
				confd = accept(sockfd, NULL, NULL);
				if (confd < -1)
				{
					perror("Accept: ");
					exit(EXIT_FAILURE);
				}
				epoll_add(confd);
				conn_sock_map[confd] = sockfd;
			}
			else
			{
				// Already existing connection. Request.
				// manage_request(sockfd);
			}
		}
	}
}

void Server::manage_request(int fd)
{
	std::string recv_str;
	char        buffer[BUFFER_SIZE];
	int         nbytes;

	recv_str.clear();

	while ((nbytes = recv(fd, buffer, BUFFER_SIZE - 1, 0)) > 0)
	{
		buffer[nbytes] = 0;
		recv_str += buffer;
	}

	if (nbytes == -1 || nbytes == 0) // EWOULDBLOCK o se ha terminado de leer.
	{
		// select_config(fd);
		// engine(recv_str, *socket_map[conn_sock_map[fd]]->getConfig())q
	}
}

t_c_global_config *Server::getGlobalConfig(void)
{
	return this->global_config;
}

int Server::getEpoll(void)
{
	return this->epollfd;
}

struct epoll_event *Server::getEvents(void)
{
	return this->events;
}

std::map<int, MySocket *> &Server::getSocket_Map(void)
{
	return this->socket_map;
}

std::map<int, int> &Server::getConn_Sock_Map(void)
{
	return this->conn_sock_map;
}