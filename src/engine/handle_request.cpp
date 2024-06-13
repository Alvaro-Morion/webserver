/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Filename: handle_request.cpp                                             */
/*   Author:   Peru Riezu <riezumunozperu@gmail.com>                          */
/*   github:   https://github.com/priezu-m                                    */
/*   Licence:  GPLv3                                                          */
/*   Created:  2024/06/07 14:41:43                                            */
/*   Updated:  2024/06/12 16:51:31                                            */
/*                                                                            */
/* ************************************************************************** */

#include "engine.hpp"
#include <cctype>
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

static ReturnType handle_request_internal(std::string const &request, t_c_individual_server_config const &config, struct in_addr ip)
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
	normalize_resource(resource);
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
		// std::string CONTENT_LENGTH = std::to_string(body.size());
		// std::string GATEWAY_INTERFACE = "1.1";
		// std::string PATH_INFO = "/";
		// std::string PATH_TRANSLATED = remove_filename(get_new_location(...));
		// std::string QUERY_STRING = get_query_string(...);
		// std::string REMOTE_ADDR = ip_to_string(ip) there is a standar function for this, use it instead
		// std::string REQUEST_METHOD = method
		// std::string SCRIPT_NAME = get_filename(get_new_location(...));
		// std::string SERVER_NAME = *config.host_name
		// std::string SERVER_PORT = config.port
		// std::string SERVER_PROTOCOL = "HTTP/1.1"
		// std::string SERVER_SOFTWARE = "webserv/0.1"
		// int memfd = //create memfd and populate it whit the contents of body
		//
		// return (handle_cgi(resource, resource_rules.first));
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
