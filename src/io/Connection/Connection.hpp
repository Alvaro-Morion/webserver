#pragma once

#include "../../config/config.hpp"
#include "../../engine/engine.hpp"
#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>

#define BUFFER_SIZE 100000

class Connection
{
	private:
		int 								epollfd;
		int									confd;
		uint16_t							port;
		std::string							request_buffer;
		std::string							response_buffer;
		ReturnType							response;
		size_t								bytes_sent;
		bool								header_sent;
		bool								sent_response;
		t_c_global_config const 			*global_config;
		t_c_individual_server_config const	*config;
	public:
		Connection(int epollfd, uint16_t port, t_c_global_config *global_config, ReturnType &resp);
		~Connection();

		int									accept_connection(int sockfd);
		void								read_request(void);
		void								select_config(void);
		void								generate_response(void);
		void								send_response(void);
		void								check_body_length(void) const;
		void								check_not_chunked(void) const;
		bool								headers_read(void) const;
		bool								request_read(void);
		bool								response_sent(void) const;
		int									getEpollFd(void) const;
		int									getConFd(void) const;
		std::string 						getRequestBuffer(void) const;
		t_c_global_config const				*getGlobalConfig(void) const;
		t_c_individual_server_config const	*getConfig(void) const;
};
