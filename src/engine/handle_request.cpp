/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Filename: handle_request.cpp                                             */
/*   Author:   Peru Riezu <riezumunozperu@gmail.com>                          */
/*   github:   https://github.com/priezu-m                                    */
/*   Licence:  GPLv3                                                          */
/*   Created:  2024/06/07 14:41:43                                            */
/*   Updated:  2024/07/02 19:48:16                                            */
/*                                                                            */
/* ************************************************************************** */

#include "engine.hpp"
#include <cctype>
#include <cerrno>
#include <cstddef>
#include <ctime>
#include <fcntl.h>
#include <functional>
#include <iterator>
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
	size_t const beginning = request.find("\r\n\r\n", i) + 4;
	size_t const end = request.size();

	return (std::string(request.begin() + static_cast<std::string::difference_type>(beginning),
						request.begin() + static_cast<std::string::difference_type>(end)));
}

static std::pair<t_c_route, bool> get_resource_rules(std::string                             resource,
													 std::set<t_c_route, std::less<> > const &routes)
{
	size_t                                     i;
	std::set<t_c_route, std::less<> >::iterator it;

	if (resource.back() != '/')
	{
		resource.append("/a");
	}
	else
	{
		resource.push_back('a');
	}
	while (resource.empty() == false)
	{
		i = resource.find_last_of('/');
		resource.erase(i + 1, resource.size() - (i + 1));
		it = routes.find(resource);
		if (it != routes.end())
		{
			return (std::pair<t_c_route, bool>(*it, true));
		}
		resource.pop_back();
	}
	return (std::pair<t_c_route, bool>(t_c_route(), false));
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

static ReturnType handle_request_internal(std::string const &request, t_c_individual_server_config const &config,
										  struct in_addr ip)
{
	size_t                     i;
	std::string                method;
	std::string                resource;
	std::string                protocol;
	std::string                body;
	std::pair<t_c_route, bool> resource_rules;

	i = 0;
	method = get_method(request, i);
	if (method != "GET" && method != "POST" && method != "DELETE")
	{
		return (handle_error(501, config)); // not implemented
	}
	resource = get_resource(request, i);
	resource = normalize_resource(resource);
	if (resource[0] != '/')
	{
		return (handle_error(400, config)); // bad request
	}
	protocol = get_protocol(request, i);
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
		return (handle_cgi(resource, resource_rules.first, config, body, method, ip));
	}
	return (handle_normal(resource, resource_rules.first, config));
}

ReturnType handle_request(std::string const &request, t_c_individual_server_config const &config, struct in_addr ip)
{
	try
	{
		return (handle_request_internal(request, config, ip));
	}
	catch (...)
	{
		return (handle_error(500, config)); // internal server error
	}
}

#pragma GCC diagnostic pop
