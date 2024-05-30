#pragma once

#include "../../config/config.hpp"
#include "../MySocket/MySocket.hpp"
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

#define MAX_EVENTS  500
#define BUFFER_SIZE 30000

class Server
{
	private:
		t_c_global_config        *global_config;
		int                       epollfd;
		std::map<int, MySocket *> socket_map;      // Openned sockets with their fds.
		std::map<int, uint16_t>   client_port_map; // Relates client fd with the port.
	public:
		Server(t_c_global_config *config);
		Server(std::set<uint16_t> ports); // For testing purposes.
		~Server();

		void                                config_epoll(std::set<uint16_t> ports);
		void                                epoll_add(int fd, uint32_t mask);
		void                                server_loop(void);
		void                                manage_request(int fd);
		t_c_individual_server_config const &select_config(int fd, std::string request);
		void                                send_response(int response_fd, int socketfd);

		t_c_global_config                  *getGlobalConfig(void);
		int                                 getEpoll(void);
		std::map<int, MySocket *>          &getSocket_Map(void);
		std::map<int, uint16_t>            &getClientPortMap(void);
};
