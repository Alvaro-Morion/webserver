/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Filename: handle_error.cpp                                               */
/*   Author:   Peru Riezu <riezumunozperu@gmail.com>                          */
/*   github:   https://github.com/priezu-m                                    */
/*   Licence:  GPLv3                                                          */
/*   Created:  2024/06/07 13:33:33                                            */
/*   Updated:  2024/06/09 12:46:23                                            */
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

static ReturnType handle_error_internal_internal(std::string const &error_file, std::string const &status_line)
{
	int const         fd = open(error_file.c_str(), O_RDONLY);
	ssize_t const     file_size = get_file_size(fd);
	std::string const current_time = get_current_time_as_string();
	std::string const headers = status_line + "Server: webserv/0.1\r\n" + "Date: " + current_time + "\r\n" +
								"Content-Type: text/html\r\n" + "Content-Length: " + std::to_string(file_size) +
								"\r\n" + "Connection: close" + "\r\n\r\n";

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

static ReturnType handle_error_internal(int error_code, t_c_individual_server_config const &config)
{
	std::string file_name;
	std::string status_line;

	switch (error_code) // NOLINT
	{
		case 505:
			file_name = config.get_error_pages()->get_http_version_not_supported();
			status_line = "HTTP/1.1 505 HTTP Version Not Supported\r\n";
			break;
		case 501:
			file_name = config.get_error_pages()->get_not_implemeted();
			status_line = "HTTP/1.1 501 Not Implemented\r\n";
			break;
		case 500:
			file_name = config.get_error_pages()->get_internal_server_error();
			status_line = "HTTP/1.1 500 Internal Server Error\r\n";
			break;
		case 414:
			file_name = config.get_error_pages()->get_uri_too_long();
			status_line = "HTTP/1.1 414 URI Too Long\r\n";
			break;
		case 413:
			file_name = config.get_error_pages()->get_content_too_large();
			status_line = "HTTP/1.1 413 Content Too Large\r\n";
			break;
		case 411:
			file_name = config.get_error_pages()->get_length_requiered();
			status_line = "HTTP/1.1 411 Length Required\r\n";
			break;
		case 408:
			file_name = config.get_error_pages()->get_request_timeout();
			status_line = "HTTP/1.1 408 Request Timeout\r\n";
			break;
		case 404:
			file_name = config.get_error_pages()->get_not_found();
			status_line = "HTTP/1.1 404 Not Found\r\n";
			break;
		case 403:
			file_name = config.get_error_pages()->get_forbidden();
			status_line = "HTTP/1.1 403 Forbidden\r\n";
			break;
		case 400:
			file_name = config.get_error_pages()->get_bad_request();
			status_line = "HTTP/1.1 400 Bad Request\r\n";
	}
	return (handle_error_internal_internal(file_name, status_line));
}

ReturnType handle_error(int error_code, const t_c_individual_server_config &config)
{
	try
	{
		return (handle_error_internal(error_code, config));
	}
	catch (...)
	{
		return (ReturnType(-1, "", NO_CHILD));
	}
}

#pragma GCC diagnostic pop
