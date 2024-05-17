#pragma once

#include "../../config/config.hpp"
#include "../MySocket/MySocket.hpp"

#include <sys/epoll.h>
#include <map>
#include <vector>

class Server
{
    private:
        int epollfd;
        std::map<int, t_c_individual_server_config> socket_configs;
    public:
        Server(t_c_global_config config);

        void config_epoll(t_c_global_config config); //Crea la epoll, añade todos los sockets y wait.
        void server_loop(); //Espera peticiones y las redirige hacia el servidor o el cliente.  
        void manage_request(); //Gestiona la petición una vez que se sale de epollwait.*/

        int getEpoll();
        std::map<int, t_c_individual_server_config> socket_config;   
};