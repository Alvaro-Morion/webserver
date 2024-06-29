/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Filename: handle_cgi.cpp                                                 */
/*   Author:   Peru Riezu <riezumunozperu@gmail.com>                          */
/*   github:   https://github.com/priezu-m                                    */
/*   Licence:  GPLv3                                                          */
/*   Created:  2024/06/13 19:35:27                                            */
/*   Updated:  2024/06/29 21:13:34                                            */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "engine.hpp"
#include <arpa/inet.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <netinet/in.h>
#include <string>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define WRITE_END 1
#define READ_END  0

;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic warning "-Weverything"
#pragma GCC diagnostic ignored "-Wempty-translation-unit"
#pragma GCC diagnostic ignored "-Wunused-macros"
#pragma GCC diagnostic ignored "-Wextra-semi"
#pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
#pragma GCC diagnostic ignored "-Wc++98-compat"
#pragma GCC diagnostic ignored "-Wwritable-strings"
#pragma GCC diagnostic ignored "-Wlanguage-extension-token"
#pragma GCC diagnostic ignored "-Wpre-c++20-compat-pedantic"
#pragma GCC diagnostic ignored "-Wc++20-designator"
#pragma GCC diagnostic ignored "-Wc++98-compat-extra-semi"
;

static std::string get_query_string(std::string const &resource)
{
	const size_t pos = resource.find(-'?');

	if (pos == std::string::npos)
	{
		return ("");
	}
	return (resource.substr(pos + 1, resource.size() - (pos + 1)));
}

static std::string remove_filename(std::string const &target_file)
{
	const size_t pos = target_file.find_last_of('/');

	return (target_file.substr(0, pos + 1));
}

static std::string get_filename(std::string const &target_file)
{
	const size_t pos = target_file.find_last_of('/');

	return (target_file.substr(pos, target_file.size() - pos));
}

static ReturnType handle_cgi_internal_internal(std::string const &target_file, std::string const &body,
											   t_c_individual_server_config const &config, char const **new_env,
											   char const **new_argv)
{
	int const memfd = memfd_create("", 0);
	int       pipefds[2];
	pid_t     c_pid;

	if (memfd == -1)
	{
		return (handle_error(500, config)); // internal server error
	}
	if (ftruncate(memfd, static_cast<ssize_t>(body.length())) == -1)
	{
		close(memfd);
		return (handle_error(500, config)); // internal server error
	}
	if (write(memfd, body.c_str(), body.size()) != static_cast<ssize_t>(body.size()))
	{
		close(memfd);
		return (handle_error(500, config)); // internal server error
	}
	if (pipe(pipefds) == -1)
	{
		close(memfd);
		return (handle_error(500, config)); // internal server error
	}
	c_pid = fork();
	if (c_pid == -1)
	{
		lseek(memfd, 0, SEEK_SET);
		close(memfd);
		close(pipefds[WRITE_END]);
		close(pipefds[READ_END]);
		return (handle_error(500, config)); // internal server error
	}
	if (c_pid == 0)
	{
		if ((dup2(pipefds[WRITE_END], STDOUT_FILENO) == -1) || (dup2(memfd, STDIN_FILENO) == -1))
		{
			fprintf(stderr, "fatal error when juking output and input of cgi script");
			exit(EXIT_FAILURE); // NOLINT
		}
		close(pipefds[WRITE_END]);
		close(pipefds[READ_END]);
		close(memfd);
		execve(target_file.c_str(), reinterpret_cast<char *const *>(reinterpret_cast<intptr_t>(new_argv)),
			   reinterpret_cast<char *const *>(reinterpret_cast<intptr_t>(new_env)));
		fprintf(stderr, "fatal error when launchinng cgi script");
		exit(EXIT_FAILURE); // NOLINT
	}
	close(memfd);
	close(pipefds[WRITE_END]);
	return (ReturnType(pipefds[READ_END], "", c_pid));
}

static ReturnType handle_cgi_internal(std::string const &target_file, std::string const &resource,
									  std::string const &body, std::string const &method,
									  t_c_individual_server_config const &config, struct in_addr ip)
{
	char              dst[16];
	std::string const content_length_var = std::string("CONTENT_LENGTH=") + std::to_string(body.size());
	std::string const path_translated_var = std::string("PATH_TRANSLATED=") + remove_filename(target_file);
	std::string const query_string_var = std::string("QUERY_STRING=") + get_query_string(resource);
	std::string const remote_addr_var = std::string("REMOTE_ADDR=") + inet_ntop(AF_INET, &ip, dst, sizeof(dst));
	std::string const method_var = std::string("REQUEST_METHOD=") + method;
	std::string const script_name_var = std::string("SCRIPT_NAME=") + get_filename(target_file);
	std::string const server_name_var = std::string("SERVER_NAME=") + *config.get_host_name();
	std::string const server_port_var = std::string("SERVER_PORT=") + std::to_string(config.get_port());
	char const       *new_argv[] = {target_file.c_str(), nullptr};
	char const       *new_env[] = {content_length_var.c_str(),
								   "GATEWAY_INTERFACE=1.1",
								   "PATH_INFO=/",
								   path_translated_var.c_str(),
								   query_string_var.c_str(),
								   remote_addr_var.c_str(),
								   method_var.c_str(),
								   script_name_var.c_str(),
								   server_name_var.c_str(),
								   server_port_var.c_str(),
								   "SERVER_PROTOCOL=HTTP/1.1",
								   "SERVER_SOFTWARE=webserv/0.1",
								   nullptr};

	// std::cout << "**** this the query string  **** " << query_string_var << std::endl; 
	return (handle_cgi_internal_internal(target_file, body, config, new_env, new_argv));
}

ReturnType handle_cgi(std::string &resource, t_c_route const &route, t_c_individual_server_config const &config,
					  std::string const &body, std::string const &method, struct in_addr ip)
{
	std::string target_file;
	struct stat statbuf;

	target_file = get_new_location(resource, route);
	if (stat(target_file.c_str(), &statbuf) == -1)
	{
		if (errno == ENOENT || errno == ENOTDIR)
		{
			return (handle_error(404, config)); // not found
		}
		if (errno == EACCES)
		{
			return (handle_error(403, config)); // forbidden
		}
		return (handle_error(500, config)); // internal server error
	}
	if (S_ISDIR(statbuf.st_mode) != 0)
	{
		return (handle_dir(target_file, route, config, statbuf));
	}
	if (((statbuf.st_mode & S_IXUSR) == 0) && ((statbuf.st_mode & S_IXGRP) == 0))
	{
		return (handle_error(403, config)); // forbidden
	}
	return (handle_cgi_internal(target_file, resource, body, method, config, ip));
}

#pragma GCC diagnostic pop
