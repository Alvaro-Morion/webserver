#pragma once

#include "../io.hpp"
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

class MySocket
{
    private:
        int sockfd;
        struct addrinfo *address;
    public:
        
        MySocket(t_c_individual_server_config config);
        ~MySocket();

        void bind_socket();
        void test_connection(int value, std::string error); //perror y exit

        int getSockfd();
        struct addrinfo *getAddress();

        void setAddress(int domain, int type, int protocol, int port, std::string host);
};