/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Filename: handle_redirect.cpp                                            */
/*   Author:   Peru Riezu <riezumunozperu@gmail.com>                          */
/*   github:   https://github.com/priezu-m                                    */
/*   Licence:  GPLv3                                                          */
/*   Created:  2024/06/12 16:50:32                                            */
/*   Updated:  2024/06/29 18:07:45                                            */
/*                                                                            */
/* ************************************************************************** */

#include "engine.hpp"
#include <fcntl.h>
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

ReturnType handle_redirect(std::string const &resource, t_c_route const &route)
{
	int const         fd = open("default_error_pages/301", O_RDONLY);
	ssize_t const     file_size = get_file_size(fd);
	std::string const current_time = get_current_time_as_string();
	std::string const headers =
		std::string("HTTP/1.1 301 Moved Permanently\r\n") + "Server: webserv/0.1\r\n" + "Date: " + current_time +
		"\r\n" + "Content-Type: text/html\r\n" + "Content-Length: " + std::to_string(file_size) + "\r\n" +
		"Location: " + get_new_location(resource, route) + "\r\n" + "Connection: close" + "\r\n\r\n";

	if (fd == -1)
	{
		return (ReturnType(-1, std::string(""), NO_CHILD));
	}
	if (file_size == -1 || current_time.empty() == true)
	{
		close(fd);
		return (ReturnType(-1, std::string(""), NO_CHILD));
	}
	return (ReturnType(fd, headers, NO_CHILD));
}

#pragma GCC diagnostic pop
