/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Filename: t_c_server_config.cpp                                          */
/*   Author:   Peru Riezu <riezumunozperu@gmail.com>                          */
/*   github:   https://github.com/priezu-m                                    */
/*   Licence:  GPLv3                                                          */
/*   Created:  2024/05/06 19:19:46                                            */
/*   Updated:  2024/05/15 06:20:25                                            */
/*                                                                            */
/* ************************************************************************** */

#include "../config.hpp"
#include <algorithm>
#include <arpa/inet.h>
#include <cctype>
#include <cstddef>
#include <cstdint>
#include <netinet/in.h>
#include <stdexcept>
#include <string>
#include <sys/socket.h>
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
#pragma GCC diagnostic ignored "-Wc99-extensions"
;

bool is_valid_hostname(std::string const &host_name)
{
	size_t          dot_pos;
	size_t          lable_begin;
	struct in_addr  addr;
	struct in6_addr addr6;

	if (inet_pton(AF_INET, host_name.c_str(), &addr) == 1 || inet_pton(AF_INET6, host_name.c_str(), &addr6) == 1)
	{
		return (true);
	}
	if ((host_name.size() > 253) ||
		(host_name.find_first_not_of("abcdefghijklmnopqrstuvwxyz0123456789-.") != std::string::npos) ||
		host_name.empty() == true)
	{
		return (false);
	}
	dot_pos = std::string::npos;
	while (dot_pos != host_name.size() - 1)
	{
		lable_begin = dot_pos + 1;
		if (dot_pos == std::string::npos)
		{
			lable_begin = 0;
		}
		dot_pos = host_name.find('.', lable_begin);
		if (dot_pos == std::string::npos)
		{
			dot_pos = host_name.size() - 1;
		}
		if ((dot_pos == lable_begin || (dot_pos - lable_begin) > 62) || (host_name[dot_pos - 1] == '-') ||
			(host_name[lable_begin] == '-'))
		{
			return (false);
		}
	}
	return (true);
}

t_c_server_config::t_c_server_config(std::vector<std::string>    *host_names_param,
									 std::vector<uint16_t> const &ports_param, t_c_router const *router_param,
									 t_c_default_error_pages const *default_error_pages_param,
									 std::string const             *resource_is_a_directory_page_param,
									 uint64_t                       client_body_size_limit_param)
	: host_names(host_names_param), ports(ports_param), router(router_param),
	  default_error_pages(default_error_pages_param), resource_is_a_directory_page(resource_is_a_directory_page_param),
	  client_body_size_limit(client_body_size_limit_param)
{
	if (host_names->empty() == true || ports.empty() == true)
	{
		throw(
			std::invalid_argument("atleast a host name and port musts be provided for a server")); // parameters invalid
	}
	for (std::string &host_name : *host_names)
	{
		std::transform(host_name.begin(), host_name.end(), host_name.begin(),
					   [](unsigned char c)
					   {
						   return std::tolower(c);
					   });
		if (is_valid_hostname(host_name) == false)
		{
			throw(std::invalid_argument(host_name + " is not a valid host_name")); // parameters invalid
		}
	}
	if ((resource_is_a_directory_page->empty() == false) && access(resource_is_a_directory_page->c_str(), R_OK) == -1)
	{
		throw(*resource_is_a_directory_page_param +
			  " cannot be open for reading or does not exits"); // parameters invalid
	}
}

t_c_server_config::t_c_server_config(t_c_server_config const &copy)
	: host_names(copy.host_names), ports(copy.ports), router(copy.router),
	  default_error_pages(copy.default_error_pages), resource_is_a_directory_page(copy.resource_is_a_directory_page),
	  client_body_size_limit(copy.client_body_size_limit)
{
}

t_c_server_config::~t_c_server_config(void)
{
}

std::vector<std::string> const *t_c_server_config::get_host_names(void) const
{
	return (host_names);
}

std::vector<uint16_t> const &t_c_server_config::get_ports(void) const
{
	return (ports);
}

t_c_router const *t_c_server_config::get_router(void) const
{
	return (router);
}

t_c_default_error_pages const *t_c_server_config::get_default_error_pages(void) const
{
	return (default_error_pages);
}

std::string const *t_c_server_config::get_resource_is_a_directory_page(void) const
{
	return (resource_is_a_directory_page);
}

uint64_t t_c_server_config::get_client_body_size_limit(void) const
{
	return (client_body_size_limit);
}

#pragma GCC diagnostic pop
