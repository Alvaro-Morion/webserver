#pragma once

#include "../../config/t_c_global_config/t_c_global_config.hpp"
#include "../../engine/engine.hpp"
#include "../MySocket/MySocket.hpp"
#include "../Connection/Connection.hpp"
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <sys/epoll.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <vector>

#define MAX_EVENTS  10000

class Server
{
	private:
		t_c_global_config        	*global_config;
		int                       	epollfd;
		std::map<int, MySocket *>	socket_map;      // Openned sockets with their fds.
		std::map<int, Connection *>	connection_map;  // Relates client fd with connection object.
		std::map<int, Connection *> connection_response_map; //Relates fd of response and connection object.
	public:
		Server(t_c_global_config *config);
		Server(std::set<uint16_t> ports); // For testing purposes.
		~Server();

		void                                config_epoll(std::set<uint16_t> ports);
		void                                manage_epoll(int fd, int op, uint32_t mask);
		void                                server_loop(void);

		t_c_global_config                  *getGlobalConfig(void);
		int                                 getEpoll(void);
		std::map<int, MySocket *>          &getSocket_Map(void);
		std::map<int, Connection *>        &getConnectionMap(void);
};
