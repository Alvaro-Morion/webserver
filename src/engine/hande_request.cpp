/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Filename: hande_request.cpp                                              */
/*   Author:   Peru Riezu <riezumunozperu@gmail.com>                          */
/*   github:   https://github.com/priezu-m                                    */
/*   Licence:  GPLv3                                                          */
/*   Created:  2024/06/07 14:41:43                                            */
/*   Updated:  2024/06/07 16:46:00                                            */
/*                                                                            */
/* ************************************************************************** */

#include "engine.hpp"
#include <cstddef>
#include <set>
#include <string>

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
	const size_t beginning = request.find_first_not_of(" \t", 0);
	i = request.find_first_of(" \t", beginning);

	return (std::string(request.begin() + static_cast<std::string::difference_type>(beginning), request.begin() + static_cast<std::string::difference_type>(i)));
}

static std::string get_resource(std::string const &request, size_t &i)
{
	const size_t beginning = request.find_first_not_of(" \t", i);
	i = request.find_first_of(" \t", beginning);

	return (std::string(request.begin() + static_cast<std::string::difference_type>(beginning), request.begin() + static_cast<std::string::difference_type>(i)));
	return (std::string(request.begin() + beginning, request.begin() + static_cast<std::string::difference_type>(i)));
}

static std::string get_protocol(std::string const &request, size_t &i)
{
	const size_t beginning = request.find_first_not_of(" \t", i);
	i = request.find_first_of(" \t\r", beginning);

	return (std::string(request.begin() + static_cast<std::string::difference_type>(beginning), request.begin() + static_cast<std::string::difference_type>(i)));
}

static std::string get_body(std::string const &request, size_t &i)
{
	const size_t beginning = request.find("\r\n\r\n", i);
	const size_t end = request.size();
	
	return (std::string(request.begin() + static_cast<std::string::difference_type>(beginning), request.begin() + static_cast<std::string::difference_type>(end)));
}

static ReturnType handle_request_internal(std::string const &request, t_c_individual_server_config const &config)
{
	size_t       i;
	std::string  method;
	std::string  resource;
	std::string  protocol;
	std::string  body;
	std::set<t_c_route, std::less<>>::const_iterator maped_resource;

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
	if (config.get_router()->get_routes().empty() == true)
	{
		return (handle_error(404, config)); // not found
	}
	maped_resource = config.get_router()->get_routes().upper_bound(resource);
	maped_resource--;
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
