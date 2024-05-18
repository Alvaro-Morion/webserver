#pragma once

#include "../../config/config.hpp"
#include "../MySocket/MySocket.hpp"

#include <cstdlib>
#include <cstring>
#include <error.h>
#include <map>
#include <sstream>
#include <stdio.h>
#include <string>
#include <sys/epoll.h>
#include <vector>

#define MAX_EVENTS  50
#define BUFFER_SIZE 1024

class Server
{
	private:
		t_c_global_config        *global_config;
		int                       epollfd;
		struct epoll_event        events[MAX_EVENTS];
		std::map<int, MySocket *> socket_map;    // Listado de sockets abieros.
		std::map<int, int>        conn_sock_map; // Tabla conexión-socket.
	public:
		Server(t_c_global_config config);

		void                config_epoll(t_c_global_config config); // Crea la epoll, añade todos los sockets y wait.
		void                epoll_add(int fd);                      // Añade el fd a e epoll.
		void                server_loop();          // Espera peticiones y las redirige hacia el servidor o el cliente.
		void                manage_request(int fd); // Gestiona la petición una vez que se sale de epollwait.*/

		t_c_global_config  *getGlobalConfig();
		int                 getEpoll();
		struct epoll_event *getEvents();
};