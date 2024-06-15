#pragma once

#include "unistd.h"
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

#define BACKLOG 1000

class MySocket
{
	private:
		int         sockfd;
		uint16_t    port;
		sockaddr_in address;

	public:
		MySocket(uint16_t port);
		~MySocket();

		void        bind_socket(uint16_t port);
		void        test_connection(int value, std::string error);

		int         getSockfd(void);
		uint16_t    getPort(void);
		sockaddr_in getAddress(void);

		void        setAddress(uint16_t port);
};