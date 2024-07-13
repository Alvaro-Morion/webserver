#pragma once

#include "../../config/config.hpp"
#include "../../engine/engine.hpp"
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>

#define BUFFER_SIZE 10000

class Connection
{
	private:
		int                                 confd;
		uint16_t                            port;
		struct sockaddr_in                  address;
		std::string                         request_buffer;
		std::string                         response_buffer;
		ReturnType                          response;
		size_t                              bytes_sent;
		bool                                ready_to_send;
		bool                                sent_response;
		t_c_global_config const            *global_config;
		t_c_individual_server_config const *config;
		time_t								last_activity;

	public:
		Connection(uint16_t port, t_c_global_config *global_config, ReturnType &resp);
		~Connection();

		int                                 accept_connection(int sockfd);
		int                                 read_request(void);
		void                                select_config(void);
		int                                 generate_response(void);
		int									generate_timeout_response(void);
		int                                 build_response(void);   // For CGI (designed to pass though epoll)
		int                                 build_response(int fd); // For regular files.
		int                                 send_response(void);
		void                                check_body_length(void) const;
		void                                check_not_chunked(void) const;
		bool                                headers_read(void) const;
		bool                                response_ready(void) const;
		bool                                request_read(void);
		bool                                response_sent(void) const;
		int                                child_error(void);
		int                                 getConFd(void) const;
		struct sockaddr_in const           &getAddress(void) const;
		std::string                         getRequestBuffer(void) const;
		std::string                         getResponseBuffer(void) const;
		ReturnType const                   &getResponse(void) const;
		t_c_global_config const            *getGlobalConfig(void) const;
		t_c_individual_server_config const *getConfig(void) const;
		time_t								getLastActivity(void) const;
		void                                set_ready(void);
};
