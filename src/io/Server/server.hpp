#pragma once

//#include "../../config/config.hpp"
#include "../MySocket/MySocket.hpp"
#include <map>
#include <set>
#include <string>
#include <sys/epoll.h>
#include <vector>

#define MAX_EVENTS  500
#define BUFFER_SIZE 30000

class Server
{
	private:
		// t_c_global_config			*global_config;
		int							epollfd;
		std::map<int, MySocket *>	socket_map;		// Openned sockets with their fds.
		std::map<int, uint16_t>		client_port_map; // Relates client fd with the port.
	public:
		// Server(t_c_global_config config);
		Server(std::set<uint16_t> ports);			//For testing purposes.
		~Server();

		void							config_epoll(std::set<uint16_t> ports); // Epoll creation
		void							epoll_add(int fd, uint32_t mask);       // Adds fd to epoll.
		void							server_loop(void);                      // Waits for requets and accept new clients,
		void							manage_request(int fd);                 // Manages pending request.
		void							select_config(int fd, std::string request);

		//t_c_global_config         		*getGlobalConfig(void);
		int                        		getEpoll(void);
		struct epoll_event        		*getEvents(void);
		std::map<int, MySocket *> 		&getSocket_Map(void);
};