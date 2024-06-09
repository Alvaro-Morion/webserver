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
	{
		delete iter->second;
	}
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
		manage_epoll(socket->getSockfd(), EPOLL_CTL_ADD, EPOLLIN);
		socket_map[socket->getSockfd()] = socket;
	}
}

void Server::manage_epoll(int fd, int op, uint32_t mask)
{
	struct epoll_event event;

	memset(&event, 0, sizeof(event));
	event.events = mask;
	event.data.fd = fd;

	if (epoll_ctl(epollfd, op, fd, &event) == -1)
	{
		perror("Epoll_ctl");
		close(fd);
	}
	std::cout << fd << " to poll" << std::endl;
}

void Server::server_loop(void)
{
	int                sockfd;
	struct epoll_event events[MAX_EVENTS];
	int                nevents;

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
				ReturnType resp(-1, "", NO_CHILD);
				Connection *connection = new Connection(epollfd, socket_map[sockfd]->getPort(),
						global_config, resp);
				if(connection->accept_connection(sockfd) < 0)
					delete connection;
				else
				{
					connection_map[connection->getConFd()] = connection;
					manage_epoll(connection->getConFd(), EPOLL_CTL_ADD, EPOLLIN | EPOLLRDHUP | EPOLLHUP);
				}
			}
			else if ((events[n].events & EPOLLIN) == EPOLLIN)
			{
				Connection *connection = connection_map[sockfd];
				connection->read_request();
				if (connection->request_read())
				{
					connection->generate_response();
					manage_epoll(connection->getConFd(), EPOLL_CTL_MOD, EPOLLOUT | EPOLLRDHUP | EPOLLHUP);
				}
			}
			else if ((events[n].events & EPOLLOUT) == EPOLLOUT)
			{
				connection_map[sockfd]->send_response();
				if (connection_map[sockfd]->response_sent())
				{
					delete connection_map[sockfd];
					connection_map.erase(sockfd);
				}
			}
			else
			{
				delete connection_map[sockfd];
				connection_map.erase(sockfd);
			}
			std::cout << "end of the loop" << std::endl;
		}
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

std::map<int, MySocket *> &Server::getSocket_Map(void)
{
	return this->socket_map;
}

std::map<int, Connection *> &Server::getConnectionMap(void)
{
	return connection_map;
}
