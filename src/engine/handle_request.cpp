/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Filename: handle_request.cpp                                             */
/*   Author:   Peru Riezu <riezumunozperu@gmail.com>                          */
/*   github:   https://github.com/priezu-m                                    */
/*   Licence:  GPLv3                                                          */
/*   Created:  2024/06/07 14:41:43                                            */
/*   Updated:  2024/06/09 17:30:43                                            */
/*                                                                            */
/* ************************************************************************** */

#include "engine.hpp"
#include <cerrno>
#include <cstddef>
#include <ctime>
#include <fcntl.h>
#include <functional>
#include <set>
#include <string>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
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

static std::string get_method(std::string const &request, size_t &i)
{
	size_t const beginning = request.find_first_not_of(" \t", 0);
	i = request.find_first_of(" \t", beginning);

	return (std::string(request.begin() + static_cast<std::string::difference_type>(beginning),
						request.begin() + static_cast<std::string::difference_type>(i)));
}

static std::string get_resource(std::string const &request, size_t &i)
{
	size_t const beginning = request.find_first_not_of(" \t", i);
	i = request.find_first_of(" \t", beginning);

	return (std::string(request.begin() + static_cast<std::string::difference_type>(beginning),
						request.begin() + static_cast<std::string::difference_type>(i)));
}

static std::string get_protocol(std::string const &request, size_t &i)
{
	size_t const beginning = request.find_first_not_of(" \t", i);
	i = request.find_first_of(" \t\r", beginning);

	return (std::string(request.begin() + static_cast<std::string::difference_type>(beginning),
						request.begin() + static_cast<std::string::difference_type>(i)));
}

static std::string get_body(std::string const &request, size_t &i)
{
	size_t const beginning = request.find("\r\n\r\n", i);
	size_t const end = request.size();

	return (std::string(request.begin() + static_cast<std::string::difference_type>(beginning),
						request.begin() + static_cast<std::string::difference_type>(end)));
}

static std::pair<t_c_route, bool> get_resource_rules(std::string                             resource,
													 std::set<t_c_route, std::less<>> const &routes)
{
	std::set<t_c_route, std::less<>>::const_iterator it;
	size_t                                           i;

	if (routes.empty() == true)
	{
		return (std::pair<t_c_route, bool>(t_c_route(), false));
	}
	if (resource.end()[-1] != '/')
	{
		resource.append("/a/"); // dummy text
	}
	else
	{
		resource.append("a/");
	}
	do
	{
		resource.pop_back();
		if (resource.empty() == false)
		{
			i = resource.find_last_of('/');
			resource.erase(i + 1, resource.size() - (i + 1));
		}
		it = routes.upper_bound(resource);
		it--;
	} while (it->get_path() != resource && resource.empty() == false);
	if (it->get_path() != resource)
	{
		return (std::pair<t_c_route, bool>(t_c_route(), false));
	}
	return (std::pair<t_c_route, bool>(*it, true));
}

static bool method_allowed(std::string const &method, t_c_resource const &resource_rules)
{
	if (method == "GET")
	{
		if (resource_rules.get_get_allowed() == false)
		{
			return (false);
		}
		return (true);
	}
	if (method == "POST")
	{
		if (resource_rules.get_post_allowed() == false)
		{
			return (false);
		}
		return (true);
	}
	if (resource_rules.get_delet_allowed() == false) // metehod will allways be delete at this point of execution
	{
		return (false);
	}
	return (true);
}

std::string get_current_time_as_string(void)
{
	time_t       t = time(NULL);
	struct tm    tm;
	size_t const buffer_size = 200;
	char         buff[buffer_size];

	if ((t == -1) || (gmtime_r(&t, &tm) == nullptr))
	{
		return ("");
	}
	strftime(buff, buffer_size, "%a, %d %b %y %T GMT", &tm);
	return (std::string(buff));
}

static std::string get_new_location(std::string resource, t_c_route const &route)
{
	resource.erase(0, route.get_path().size());
	resource.insert(0, route.get_resource().get_root(), route.get_resource().get_root().size());
	return (resource);
}

ssize_t get_file_size(int fd)
{
	struct stat statbuf;

	if (fstat(fd, &statbuf) == -1)
	{
		return (-1);
	}
	return (statbuf.st_size);
}

static ReturnType handle_redirect(std::string const &resource, t_c_route const &route)
{
	int const         fd = open("default_error_pages/301", O_RDONLY);
	ssize_t const     file_size = get_file_size(fd);
	std::string const current_time = get_current_time_as_string();
	std::string const headers =
		std::string("HTTP/1.1 301 Moved Permanently\n\r") + "Server: webserv/0.1\n\r" + "Date: " + current_time +
		"\n\r" + "Content-Type: text/html\n\r" + "Content-Length: " + std::to_string(file_size) + "\n\r" +
		"Location: " + get_new_location(resource, route) + "\r\n" + "Connection: close" + "\n\r\n\r";

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

static std::vector<std::string> split(std::string const &s)
{
	size_t pos_start = 0;
	size_t pos_end;
	std::string token;
	std::vector<std::string> res;

	pos_end = s.find('/', pos_start);
	while (pos_end != std::string::npos)
	{
		token = s.substr(pos_start, pos_end - pos_start);
		pos_start = pos_end + 1;
		if (token.empty() == false)
		{
			res.push_back(token);
		}
		pos_end = s.find('/', pos_start);
	}
	if (s.substr(pos_start).empty() == false)
	{
		res.push_back(s.substr(pos_start));
	}
	return res;
}

static std::string normalize_resource(std::string const &resource)
{
	std::vector<std::string> tokens = split(resource);
	std::string              res = "/";
	size_t                   i;

	i = 0;
	while (i < tokens.size())
	{
		if (tokens[i] == ".")
		{
			tokens.erase(tokens.begin() + static_cast<std::vector<std::string>::difference_type>(i));
			continue;
		}
		if (tokens[i] == "..")
		{
			if (i == 0)
			{
				return ("");
			}
			tokens.erase(tokens.begin() + static_cast<std::vector<std::string>::difference_type>(i));
			i--;
			tokens.erase(tokens.begin() + static_cast<std::vector<std::string>::difference_type>(i));
			continue;
		}
		i++;
	}
	for (std::string const &s : tokens)
	{
		res += s;
		res += "/";
	}
	return (res);
}

static bool resource_in_route(std::string const &resource, std::string const &route)
{
	const size_t res = resource.find(route.c_str(), 0, route.size());

	return (res == 0);
}

static ReturnType handle_normal(std::string &resource, t_c_route const &route, t_c_individual_server_config const &config)
{
	std::string       headers;
	std::string const current_time = get_current_time_as_string();
	ssize_t           file_size;
    std::string target_file;
	int         fd;

	resource = normalize_resource(resource);
	if (resource_in_route(resource, route.get_path()) == false)
	{
		return (handle_error(403, config)); // forbidden
	}
	target_file = get_new_location(resource, route);
	if (access(target_file.c_str(), R_OK) != 0)
	{
		if (errno == ENOENT)
		{
			return (handle_error(404, config)); // not found
		}
		if (errno == EACCES)
		{
			return (handle_error(404, config)); // forbidden
		}
		return (handle_error(500, config)); // internal server error
	}
	fd = open(resource.c_str(), O_RDONLY);
	if (fd == -1 || current_time.empty() == true)
	{
		return (handle_error(500, config)); // internal server error
	}
	file_size = get_file_size(fd);
	headers = std::string("HTTP/1.1 301 Moved Permanently\n\r") + "Server: webserv/0.1\n\r" + "Date: " + current_time +
		"\n\r" + "Content-Type: text/html\n\r" + "Content-Length: " + std::to_string(file_size) + "\n\r" +
		"Connection: close" + "\n\r\n\r";
	if (file_size == -1 || current_time.empty() == true)
	{
		return (ReturnType(-1, std::string(""), NO_CHILD));
	}
	return (ReturnType(fd, headers, NO_CHILD));
}

static ReturnType handle_request_internal(std::string const &request, t_c_individual_server_config const &config)
{
	size_t                     i;
	std::string                method;
	std::string                resource;
	std::string                protocol;
	std::string                body;
	std::pair<t_c_route, bool> resource_rules;

	i = 0;
	method = get_method(request, i);
	resource = get_resource(request, i);
	protocol = get_protocol(request, i);
	if (method != "GET" && method != "POST" && method != "DELETE")
	{
		return (handle_error(501, config)); // not implemented
	}
	if (resource[0] != '/')
	{
		return (handle_error(400, config)); // bad request
	}
	if (protocol != "HTTP/1.1")
	{
		return (handle_error(505, config)); // protocol not supported
	}
	body = get_body(request, i);
	resource_rules = get_resource_rules(resource, config.get_router()->get_routes());
	if (resource_rules.second == false)
	{
		return (handle_error(404, config)); // not found
	}
	if (method_allowed(method, resource_rules.first.get_resource()) == false)
	{
		return (handle_error(403, config)); // forbidden
	}
	if (resource_rules.first.get_resource().get_is_redirect() == true)
	{
		return (handle_redirect(resource, resource_rules.first));
	}
	if (resource_rules.first.get_resource().get_is_cgi())
	{
		//return (handle_cgi(resource, resource_rules.first));
	}
	return (handle_normal(resource, resource_rules.first, config));
}

ReturnType handle_request(std::string const &request, t_c_individual_server_config const &config)
{
	try
	{
		return (handle_request_internal(request, config));
	}
	catch (...)
	{
		return (handle_error(500, config)); // internal server error
	}
}

#pragma GCC diagnostic pop
