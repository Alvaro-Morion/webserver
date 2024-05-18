#pragma once

#include "../io.hpp"
#include <cstdlib>
#include <cstring>
#include <errno.h>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <sstream>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>

class MySocket
{
	private:
		int                           sockfd;
		struct addrinfo              *address;
		t_c_individual_server_config *config;

	public:
		MySocket(t_c_individual_server_config config);
		~MySocket();

		void                          bind_socket();
		void                          test_connection(int value, std::string error); // perror y exit

		int                           getSockfd();
		struct addrinfo              *getAddress();
		t_c_individual_server_config *getConfig();

		void                          setAddress(int domain, int type, int protocol, int port, std::string host);
};