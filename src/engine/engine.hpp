/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Filename: engine.hpp                                                     */
/*   Author:   Peru Riezu <riezumunozperu@gmail.com>                          */
/*   github:   https://github.com/priezu-m                                    */
/*   Licence:  GPLv3                                                          */
/*   Created:  2024/06/07 11:48:31                                            */
/*   Updated:  2024/06/13 18:48:05                                            */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../config/t_c_individual_server_config/t_c_individual_server_config.hpp"
#include <string>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>

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

#define NO_CHILD 0

class ReturnType
{
	private:
		int         fd;
		std::string headers;
		pid_t       child_pid; // value will be NO_CHILD if it was not a cgi

	public:
		ReturnType(int fd_param, std::string const &headers_param, pid_t child_pid_param);
		ReturnType(ReturnType const &copy);
		~ReturnType(void);

		ReturnType const &operator=(ReturnType const &copy);
		bool			  operator==(ReturnType const &rhs) const;

		int                get_fd(void) const;
		std::string const &get_headers(void) const;
		pid_t              get_child_pid(void) const;
		bool               is_cgi(void) const;
		bool               has_unrecoverable_error(void) const;
};

ReturnType  handle_invalid_request(void);
ReturnType  handle_error(int error_code, t_c_individual_server_config const &config);
ReturnType  handle_request(std::string const &request, t_c_individual_server_config const &config, struct in_addr ip);
std::string get_current_time_as_string(void);
ssize_t     get_file_size(int fd);
ReturnType  handle_normal(std::string &resource, t_c_route const &route,
				t_c_individual_server_config const &config);
void        normalize_resource(std::string resource);
std::string get_new_location(std::string resource, t_c_route const &route);
void        remove_query(std::string &uri);
ReturnType  handle_redirect(std::string const &resource, t_c_route const &route);
ReturnType  handle_dir(std::string &resource, t_c_route const &route, t_c_individual_server_config const &config, struct stat statbuf);
ReturnType  handle_error_internal_internal(std::string const &error_file, std::string const &status_line);

#pragma GCC diagnostic pop
