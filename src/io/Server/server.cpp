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
	// std::cout << fd << " to poll" << std::endl;
}

void Server::server_loop(void)
{
	int                sockfd;
	struct epoll_event events[MAX_EVENTS];
	int                nevents;

	while (true)
	{
		// std::cout << "epoll waiting ...\n";
		if ((nevents = epoll_wait(epollfd, events, sizeof(events) / sizeof(events[0]), INACTIVE_TIME*100)) == -1)
		{
			perror("Epoll wait");
			exit(EXIT_FAILURE);
		}
		//std::cout << "epoll returned " << nevents << " events\n";
		for (int n = 0; n < nevents; n++)
		{
			sockfd = events[n].data.fd;
			// std::cout << sockfd << std::endl;
			std::cout << "event " << events[n].events << " in: " << sockfd << std::endl;
			if (socket_map.find(sockfd) != socket_map.end())
			{
				ReturnType  resp(-1, "", NO_CHILD);
				Connection *connection = new Connection(socket_map[sockfd]->getPort(), global_config, resp);
				if (connection->accept_connection(sockfd) < 0)
				{
					delete connection;
				}
				else
				{
					connection_map[connection->getConFd()] = connection;
					manage_epoll(connection->getConFd(), EPOLL_CTL_ADD, EPOLLIN | EPOLLRDHUP);
				}
			}
			else if (connection_response_map.find(sockfd) != connection_response_map.end() &&
					 (events[n].events & EPOLLHUP) == EPOLLHUP)
			{
				//std::cout << "CGI Process Hangup\n";
				close(sockfd);
				if (connection_response_map[sockfd]->child_error() < 0)
				{
					delete_connection(connection_response_map[sockfd]->getConFd());
				}
				else
				{
					manage_epoll(connection_response_map[sockfd]->getConFd(), EPOLL_CTL_MOD, EPOLLOUT);
				}
				connection_response_map.erase(sockfd);
			}
			else if ((connection_response_map.find(sockfd) != connection_response_map.end()) &&
					 (events[n].events & EPOLLIN) == EPOLLIN)
			{
				// CGI pipe is ready.
				// std::cout << "Reading from pipe\n";
				int status = connection_response_map[sockfd]->build_response();
				if (status <= 0)
				{
					if (status < 0) // Error, cierre de conexión
					{
						delete_connection(connection_response_map[sockfd]->getConFd());
					}
					connection_response_map.erase(sockfd);
					close(sockfd);
				}
			}
			else if ((events[n].events & EPOLLERR) == EPOLLERR || (events[n].events & EPOLLHUP) == EPOLLHUP ||
					 (events[n].events & EPOLLRDHUP) == EPOLLRDHUP)
			{
				delete_connection(sockfd);
			}
			else if ((events[n].events & EPOLLIN) == EPOLLIN)
			{
				// std::cout << "Reading event in " << sockfd << std::endl;
				Connection *connection = connection_map[sockfd];
				if (connection->read_request() < 0)
				{
					// std::cout << "Error while reading\n";
					delete_connection(sockfd);
				}
				if (connection->request_read())
				{
					std::cout << "Request read. Generating response for " << sockfd << std::endl;
					int fd = connection->generate_response();
					if (fd > 0 && connection->getResponse().is_cgi())
					{
						manage_epoll(fd, EPOLL_CTL_ADD, EPOLLIN);
						connection_response_map[fd] = connection;
						children.push_back(connection->getResponse().get_child_pid());
					}
					else if (connection->response_ready())
					{
						manage_epoll(connection->getConFd(), EPOLL_CTL_MOD, EPOLLOUT);
					}
					else
					{
						delete_connection(sockfd);
					}
				}
			}
			else if ((events[n].events & EPOLLOUT) == EPOLLOUT)
			{
				// std::cout << "write event in" << sockfd << "\n";
				if (connection_map[sockfd]->send_response() < 0 || connection_map[sockfd]->response_sent())
				{
					std::cout << "Response sent in:" << sockfd << std::endl;
					//std::cout << "Response sent\n\"" << connection_map[sockfd]->getResponseBuffer() << "\"\n";
					manage_epoll(sockfd, EPOLL_CTL_DEL, 1);
					connection_response_map.erase(connection_map[sockfd]->getResponse().get_fd());
					delete_connection(sockfd);
				}
			}
		}
		inactive_client();
		child_reaper();
	}
}

void Server::child_reaper(void)
{
	std::vector<pid_t>::iterator child;
	//std::cout << "Number of children: " << children.size() << std::endl;

	if (children.empty())
	{
		return;
	}
	for (child = children.begin(); child != children.end();)
	{
		if (waitpid(*child, NULL, WNOHANG) != 0)
		{
			std::cout << *child << " reaped\n";
			child = children.erase(child);
		}
		else
		{
			child++;
		}
	}
}

void Server::inactive_client(void)
{
	std::map<int, Connection *>::iterator conn;
	time_t c_time;
	//std::cout << "Number of clients: " << connection_map.size() << std::endl;
	if (connection_map.empty())
	{
		return;
	}
	time(&c_time);
	for (conn = connection_map.begin(); conn != connection_map.end();)
	{
		if(difftime(c_time, conn->second->getLastActivity()) > INACTIVE_TIME)
		{
			if(!(conn->second->getResponse() == ReturnType(-1, "", NO_CHILD)) && !conn->second->response_ready())
			{
				if (conn->second->getResponse().is_cgi())
				{
					manage_epoll(conn->second->getResponse().get_fd(), EPOLL_CTL_DEL, 1);
				}
				if(conn->second->generate_timeout_response())
				{
					manage_epoll(conn->second->getConFd(), EPOLL_CTL_MOD, EPOLLOUT);
					conn++;
				}
				else
				{
					delete_connection((conn++)->first);
				}
			}
			else
			{
				delete_connection((conn++)->first);
			}
		}
		else
		{
			conn++;
		}
	}
}

void Server::delete_connection(int fd)
{
	delete connection_map[fd];
	connection_map.erase(fd);
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
