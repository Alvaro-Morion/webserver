/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Filename: engine.hpp                                                     */
/*   Author:   Peru Riezu <riezumunozperu@gmail.com>                          */
/*   github:   https://github.com/priezu-m                                    */
/*   Licence:  GPLv3                                                          */
/*   Created:  2024/06/07 11:48:31                                            */
/*   Updated:  2024/06/07 16:45:37                                            */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../config/t_c_individual_server_config/t_c_individual_server_config.hpp"
#include <string>
#include <unistd.h>

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
		int   fd;
		pid_t child_pid; // value will be NO_CHILD if it was not a cgi

	public:
		ReturnType(int fd_param, pid_t child_pid_param);
		~ReturnType(void);

		int   get_fd(void) const;
		pid_t get_child_pid(void) const;
		bool  is_cgi(void) const;
		bool  has_unrecoverable_error(void) const;
};

ReturnType handle_invalid_request(void);
ReturnType handle_error(int error_code, t_c_individual_server_config const &config);
ReturnType handle_request(std::string const &request, t_c_individual_server_config const &config);

#pragma GCC diagnostic pop
