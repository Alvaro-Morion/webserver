#pragma once

#include "../../config/config.hpp"
#include "../MySocket/MySocket.hpp"
#include <map>
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
		std::map<int, MySocket *> socket_map;    // Openned sockets with their fds.
		std::map<int, int>        conn_sock_map; // Connection-socket file descriptor table.
	public:
		Server(t_c_global_config config);
		~Server();

		void                       config_epoll(std::set<uint16_t> ports); // Epoll creation
		void                       epoll_add(int fd);                      // Adds fd to epoll.
		void                       server_loop(void);                      // Waits for requets and accept new clients,
		void                       manage_request(int fd);                 // Manages pending request.

		t_c_global_config         *getGlobalConfig(void);
		int                        getEpoll(void);
		struct epoll_event        *getEvents(void);
		std::map<int, MySocket *> &getSocket_Map(void);
		std::map<int, int>        &getConn_Sock_Map(void);
};