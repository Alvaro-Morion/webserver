/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Filename: handle_request.cpp                                             */
/*   Author:   Peru Riezu <riezumunozperu@gmail.com>                          */
/*   github:   https://github.com/priezu-m                                    */
/*   Licence:  GPLv3                                                          */
/*   Created:  2024/06/07 14:41:43                                            */
/*   Updated:  2024/06/08 15:32:41                                            */
/*                                                                            */
/* ************************************************************************** */

#include "engine.hpp"
#include <cstddef>
#include <ctime>
#include <functional>
#include <set>
#include <string>
#include <sys/mman.h>


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


static std::pair<t_c_route, bool> get_resource_rules(std::string resource, std::set<t_c_route, std::less<>> const &routes)
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

static std::string get_current_time_as_string(void)
{
	time_t t = time(NULL);
	struct tm tm;
	const size_t buffer_size = 200;
	char   buff[buffer_size];

	if ((t == -1)
		 || (gmtime_r(&t, &tm) == nullptr))
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

static ReturnType handle_redirect(std::string const &resource, t_c_route const &route)
{
	const std::string content = "<html>\n\r" \
								"<head><title>301 Moved Permanently</title></head>\n\r" \
								"<body>\n\r" \
								"<center><h1>301 Moved Permanently</h1></center>\n\r" \
								"<hr><center>webserv/0.1</center>\n\r" \
								"</body>\n\r" \
								"</html>\n\r";
	const std::string current_time = get_current_time_as_string();
	const std::string headers = std::string("HTTP/1.1 301 Moved Permanently\n\r") +
								"Server: webserv/0.1\n\r" +
								"Date: " +
								current_time +
								"\n\r" +
								"Content-Type: text/html\n\r" +
								"Content-Length: "  +
								std::to_string(content.size()) +
								"\n\r" +
								"Location: " +
								get_new_location(resource, route) +
								"\n\r\n\r";
	const int fd = memfd_create("", 0);
	ssize_t   write_ret;
	size_t    i;

	if (fd == -1)
	{
		return (ReturnType(-1, NO_CHILD));
	}
	if (current_time.empty() == true)
	{
		close(fd);
		return (ReturnType(-1, NO_CHILD));
	}
	i = 0;
	while (i < headers.size())
	{
		write_ret = write(fd, headers.c_str() + i, headers.size() - i);
		if (write_ret == -1)
		{
			close(fd);
			return (ReturnType(-1, NO_CHILD));
		}
		i += static_cast<size_t>(write_ret);
	}
	i = 0;
	while (i < content.size())
	{
		write_ret = write(fd, content.c_str() + i, headers.size() - i);
		if (write_ret == -1)
		{
			close(fd);
			return (ReturnType(-1, NO_CHILD));
		}
		i += static_cast<size_t>(write_ret);
	}
	return (ReturnType(fd, NO_CHILD));
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
