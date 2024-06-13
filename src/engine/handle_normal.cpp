/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Filename: handle_normal.cpp                                              */
/*   Author:   Peru Riezu <riezumunozperu@gmail.com>                          */
/*   github:   https://github.com/priezu-m                                    */
/*   Licence:  GPLv3                                                          */
/*   Created:  2024/06/12 16:12:24                                            */
/*   Updated:  2024/06/13 19:40:58                                            */
/*                                                                            */
/* ************************************************************************** */

#include "engine.hpp"
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

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

ReturnType handle_normal(std::string &resource, t_c_route const &route,
								t_c_individual_server_config const &config)
{
	std::string       headers;
	std::string const current_time = get_current_time_as_string();
	ssize_t           file_size;
	std::string       target_file;
	int               fd;
	struct stat       statbuf;

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
		return (handle_dir(resource, route, config, statbuf));
	}
	if (((statbuf.st_mode & S_IRUSR) == 0) && ((statbuf.st_mode & S_IRGRP) == 0))
	{
		return (handle_error(403, config)); // forbidden
	}
	fd = open(resource.c_str(), O_RDONLY);
	if (fd == -1 || current_time.empty() == true)
	{
		return (handle_error(500, config)); // internal server error
	}
	file_size = get_file_size(fd);
	headers = std::string("HTTP/1.1 301 Moved Permanently\r\n") + "Server: webserv/0.1\r\n" + "Date: " + current_time +
			  "\r\n" + "Content-Type: text/html\r\n" + "Content-Length: " + std::to_string(file_size) + "\r\n" +
			  "Connection: close" + "\r\n\r\n";
	if (file_size == -1 || current_time.empty() == true)
	{
		return (ReturnType(-1, std::string(""), NO_CHILD));
	}
	return (ReturnType(fd, headers, NO_CHILD));
}

#pragma GCC diagnostic pop
