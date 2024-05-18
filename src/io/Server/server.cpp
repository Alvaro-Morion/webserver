#include "./server.hpp"

Server::Server(t_c_global_config config)
{
    this->global_config = &config;
    config_epoll(config);
    server_loop();
}

void Server::config_epoll(t_c_global_config config)
{
    std::vector<t_c_individual_server_config> server_list = config.get_servers();
    if((epollfd = epoll_create(server_list.size())) == -1)
    {
        perror("Epoll_create: ");
        exit(EXIT_FAILURE);
    }
    
    for(ulong n = 0; n < server_list.size(); n++)
    {
        MySocket socket(server_list[n]);        
        epoll_add(socket.getSockfd());
        socket_map[socket.getSockfd()] = &socket;   
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

void Server::server_loop()
{
    int confd;
    int sockfd;

    while(true)
    {
        if (epoll_wait(epollfd, getEvents(), sizeof(getEvents()), -1) == -1)
        {
            perror("Epoll wait: ");
            exit(EXIT_FAILURE);
        }
        for (int n = 0; n < MAX_EVENTS; n++)
        {
            sockfd = events[n].data.fd;
            if(socket_map.find(sockfd) == socket_map.end())
            {
                // New Connection, accept and add fd to epoll.
                confd = accept(sockfd, socket_map[sockfd]->getAddress()->ai_addr, &socket_map[sockfd]->getAddress()->ai_addrlen);
                if (confd < 0)
                {
                    perror("Accept: ");
                    exit(EXIT_FAILURE);
                }
                epoll_add(confd);
                conn_sock_map[confd] = sockfd;
            }
            else
                //Already existing connection. Request.
                manage_request(sockfd);
        }
    }
}

void Server::manage_request(int fd)
{
    std::string recv_str;
    char buffer[BUFFER_SIZE];
    int nbytes;
    
    recv_str.clear();
    
    while ((nbytes = recv(fd, buffer, BUFFER_SIZE - 1, 0)) > 0)
    {
        buffer[nbytes] = 0;
        recv_str += buffer;
    }
    
    if(nbytes == -1) //EWOULDBLOCK o recv_str vacía
    {
        //engine(recv_str, *socket_map[conn_sock_map[fd]]->getConfig())q
    }
}

t_c_global_config *Server::getGlobalConfig()
{
    return this->global_config;
}

int Server::getEpoll()
{
    return this->epollfd;
}

struct epoll_event *Server::getEvents()
{
    return this->events;
}