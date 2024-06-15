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
		std::cout << "Socket for port: " << ntohs(*iter) << " created with fd " << socket->getSockfd() << std::endl;
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
	//std::cout << fd << " to poll" << std::endl;
}

void Server::server_loop(void)
{
	int                sockfd;
	struct epoll_event events[MAX_EVENTS];
	int                nevents;

	while (true)
	{
		//std::cout << "epoll waiting ...\n";
		if ((nevents = epoll_wait(epollfd, events, sizeof(events), -1)) == -1)
		{
			perror("Epoll wait");
			exit(EXIT_FAILURE);
		}
		//std::cout << "epoll returned " << nevents << " events\n";
		for (int n = 0; n < nevents; n++)
		{
			sockfd = events[n].data.fd;
			//std::cout << sockfd << std::endl;
			//std::cout << "event " << events[n].events << " in: " << sockfd << std::endl;
			if ((events[n].events & EPOLLERR) == EPOLLERR || (events[n].events & EPOLLHUP) == EPOLLHUP ||
					(events[n].events & EPOLLRDHUP) == EPOLLRDHUP)
			{
				delete connection_map[sockfd];
				connection_map.erase(sockfd);
			}
			else if (socket_map.find(sockfd) != socket_map.end())
			{
				ReturnType resp(-1, "", NO_CHILD);
				Connection *connection = new Connection(socket_map[sockfd]->getPort(), global_config, resp);
				if(connection->accept_connection(sockfd) < 0)
					delete connection;
				else
				{
					connection_map[connection->getConFd()] = connection;
					manage_epoll(connection->getConFd(), EPOLL_CTL_ADD, EPOLLIN | EPOLLOUT | EPOLLHUP | EPOLLRDHUP);
				}
			}
			else if ((connection_map.find(sockfd) == connection_map.end()) && (events[n].events & EPOLLIN) == EPOLLIN) 
			{
				//CGI pipe is ready.
				int status = connection_response_map[sockfd]->build_response();
				if (status <= 0)
				{
					if (status < 0)
					{
						//Error -> cierre de conexión
						connection_map.erase(connection_response_map[sockfd]->getConFd());
						delete connection_response_map[sockfd];	
					}
					connection_response_map.erase(sockfd); //Respuesta construida.
				}
			}
			else if ((events[n].events & EPOLLIN) == EPOLLIN)
			{
				Connection *connection = connection_map[sockfd];
				if (connection->read_request() < 0)
				{
					//std::cout << "Error while reading\n";
					delete connection;
					connection_map.erase(sockfd);
				}
				if (connection->request_read())
				{
					std::cout << "Request read. Generating response for ";
					int fd = connection->generate_response();
					if (fd > 0 && connection->getResponse().is_cgi())
					{
						manage_epoll(fd, EPOLL_CTL_ADD, EPOLLIN);
						connection_response_map[fd] = connection;
					}
					else if (fd < 0 && !connection->response_ready())
					{
						delete connection;
						connection_map.erase(sockfd);
					}
				}
			}
			else if (connection_map[sockfd]->response_ready() && (events[n].events & EPOLLOUT) == EPOLLOUT)
			{
				//std::cout << "Writing\n";
				//std::cout << connection_map[sockfd]->send_response() << "\t"<< connection_map[sockfd]->response_sent() << std::endl;
				if (connection_map[sockfd]->send_response() < 0 || connection_map[sockfd]->response_sent())
				{
					std::cout << "Response sent:\n";
					std::cout << connection_map[sockfd]->getResponseBuffer() << std::endl;
					delete connection_map[sockfd];
					connection_map.erase(sockfd);
				}
			}
			//std::cout << "end of the loop" << std::endl;
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
