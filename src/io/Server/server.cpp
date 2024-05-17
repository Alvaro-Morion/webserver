#include "./server.hpp"

Server::Server(t_c_global_config config)
{
    config_epoll(config);
    server_loop();
    manage_request();
}

void Server::config_epoll(t_c_global_config config)
{
    std::vector<t_c_individual_server_config> server_list = config.get_servers();
    struct epoll_event ev;
    if((this->epollfd = epoll_create(server_list.size())) == -1)
    {
        perror("Epoll: ");
        exit(EXIT_FAILURE);
    }
    
    memset(&ev, 0, sizeof(ev));
    ev.events = EPOLLIN | EPOLLOUT;

    for(std::vector<t_c_individual_server_config>::iterator iter = server_list.begin(); iter != server_list.end(); iter++)
    {
        MySocket socket(*iter);
        this->socket_configs[socket.getSockfd()] = *iter;
        if (epoll_ctl(this->epollfd, EPOLL_CTL_ADD, socket.getSockfd(), &ev) == -1)
        {
            perror("Epoll_ctl: ");
            exit(EXIT_FAILURE);
        }
    }
}