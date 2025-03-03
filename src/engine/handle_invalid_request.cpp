/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Filename: handle_invalid_request.cpp                                     */
/*   Author:   Peru Riezu <riezumunozperu@gmail.com>                          */
/*   github:   https://github.com/priezu-m                                    */
/*   Licence:  GPLv3                                                          */
/*   Created:  2024/06/07 11:53:58                                            */
/*   Updated:  2024/07/20 23:49:26                                            */
/*                                                                            */
/* ************************************************************************** */

#include "engine.hpp"
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

static ReturnType handle_invalid_request_internal(void)
{
	int const         fd = open("default_error_pages/400", O_RDONLY);
	ssize_t const     file_size = get_file_size(fd);
	std::string const current_time = get_current_time_as_string();
	std::string const headers = std::string("HTTP/1.1 400 Bad Request\r\n") + "Server: webserv/0.1\r\n" +
								"Date: " + current_time + "\r\n" + "Content-Type: text/html\r\n" +
								"Content-Length: " + to_string(file_size) + "\r\n" + "Connection: close" +
								"\r\n\r\n";

	if (fd == -1)
	{
		return (ReturnType(-1, std::string(""), NO_CHILD));
	}
	if (file_size == -1 || current_time.empty() == true)
	{
		return (ReturnType(-1, std::string(""), NO_CHILD));
	}
	return (ReturnType(fd, headers, NO_CHILD));
}

ReturnType handle_invalid_request(void)
{
	try
	{
		return (handle_invalid_request_internal());
	}
	catch (...)
	{
		return (ReturnType(-1, "", NO_CHILD));
	}
}

#pragma GCC diagnostic pop
