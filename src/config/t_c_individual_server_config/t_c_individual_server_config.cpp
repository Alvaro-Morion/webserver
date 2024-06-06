/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Filename: t_c_individual_server_config.cpp                               */
/*   Author:   Peru Riezu <riezumunozperu@gmail.com>                          */
/*   github:   https://github.com/priezu-m                                    */
/*   Licence:  GPLv3                                                          */
/*   Created:  2024/05/08 03:01:06                                            */
/*   Updated:  2024/06/06 10:32:47                                            */
/*                                                                            */
/* ************************************************************************** */

#include "t_c_individual_server_config.hpp"
#include "../config.hpp"
#include <arpa/inet.h>
#include <cstdint>
#include <stdexcept>
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

t_c_individual_server_config::t_c_individual_server_config(std::string const *host_name_param, uint16_t port_param,
														   t_c_router const              *router_param,
														   t_c_default_error_pages const *default_error_pages_param,
														   uint64_t                       client_body_size_limit_param)
	: host_name(host_name_param), port(port_param), router(router_param),
	  default_error_pages(default_error_pages_param), client_body_size_limit(client_body_size_limit_param)
{
	if (is_valid_hostname(*host_name) == false)
	{
		throw(std::invalid_argument(*host_name + " is not a valid host_name")); // parameters invalid
	}
}

t_c_individual_server_config::t_c_individual_server_config(std::string host_name, int port)
{
	default_error_pages = new t_c_default_error_pages;
	this->host_name = new std::string(host_name);
	this->port = port;
	client_body_size_limit = 0;
}

t_c_individual_server_config::t_c_individual_server_config(t_c_individual_server_config const &copy)
	: host_name(copy.host_name), port(copy.port), router(copy.router), default_error_pages(copy.default_error_pages),
	  client_body_size_limit(copy.client_body_size_limit)
{
}

t_c_individual_server_config::~t_c_individual_server_config(void)
{
}

t_c_individual_server_config const &t_c_individual_server_config::operator=(t_c_individual_server_config const &copy)
{
	if (this == &copy)
	{
		return (*this);
	}
	client_body_size_limit = copy.get_client_body_size_limit();
	default_error_pages = copy.get_default_error_pages();
	host_name = copy.get_host_name();
	port = copy.get_port();
	router = copy.get_router();
	return (*this);
}

std::string const *t_c_individual_server_config::get_host_name(void) const
{
	return (host_name);
}

uint16_t t_c_individual_server_config::get_port(void) const
{
	return (port);
}

t_c_router const *t_c_individual_server_config::get_router(void) const
{
	return (router);
}

t_c_default_error_pages const *t_c_individual_server_config::get_default_error_pages(void) const
{
	return (default_error_pages);
}

uint64_t t_c_individual_server_config::get_client_body_size_limit(void) const
{
	return (client_body_size_limit);
}

std::string t_c_individual_server_config::to_string(void) const
{
	return ("\thost name: " + *host_name + "\n\tport: " + std::to_string(ntohs(port)) +
			"\n\trouter: " + router->to_string() + "\n\terror pages:\n" + default_error_pages->to_string() +
			"\n\tclient body size limit: " +
			((client_body_size_limit == UINT64_MAX) ? "no limit" : std::to_string(client_body_size_limit)) + '\n');
}

bool t_c_individual_server_config::operator==(t_c_individual_server_config const &comparator) const
{
	if (this == &comparator)
	{
		return (true);
	}
	if ((*host_name == *comparator.host_name) && (port == comparator.port) &&
		(default_error_pages == comparator.default_error_pages) &&
		(client_body_size_limit == comparator.client_body_size_limit) && (router == comparator.router))
	{
		return (true);
	}
	return (false);
}

bool t_c_individual_server_config::operator==(t_c_individual_server_config::t_c_light_key const &comparator) const
{
	if ((*host_name == *comparator.get_host_name()) && (port == comparator.get_port()))
	{
		return (true);
	}
	return (false);
}

bool t_c_individual_server_config::operator>(t_c_individual_server_config const &comparator) const
{
	if ((*host_name > *comparator.host_name) || ((*host_name == *comparator.host_name) && (port > comparator.port)))
	{
		return (true);
	}
	return (false);
}

bool t_c_individual_server_config::operator>(t_c_individual_server_config::t_c_light_key const &comparator) const
{
	if ((*host_name > *comparator.get_host_name()) ||
		((*host_name == *comparator.get_host_name()) && (port > comparator.get_port())))
	{
		return (true);
	}
	return (false);
}

bool t_c_individual_server_config::operator<(t_c_individual_server_config const &comparator) const
{
	if ((*host_name < *comparator.host_name) || ((*host_name == *comparator.host_name) && (port < comparator.port)))
	{
		return (true);
	}
	return (false);
}

bool t_c_individual_server_config::operator<(t_c_individual_server_config::t_c_light_key const &comparator) const
{
	if ((*host_name < *comparator.get_host_name()) ||
		((*host_name == *comparator.get_host_name()) && (port < comparator.get_port())))
	{
		return (true);
	}
	return (false);
}

t_c_individual_server_config::t_c_light_key::t_c_light_key(std::string const *host_name_param, uint16_t port_param)
	: host_name(host_name_param), port(port_param)
{
	if (is_valid_hostname(*host_name) == false)
	{
		throw(std::invalid_argument(*host_name + " is not a valid host_name")); // parameters invalid
	}
}

t_c_individual_server_config::t_c_light_key::~t_c_light_key(void)
{
}

std::string const *t_c_individual_server_config::t_c_light_key::get_host_name(void) const
{
	return (host_name);
}

uint16_t t_c_individual_server_config::t_c_light_key::get_port(void) const
{
	return (port);
}

bool t_c_individual_server_config::t_c_light_key::operator==(t_c_individual_server_config const &comparator) const
{
	if (*host_name == *comparator.get_host_name() && port == comparator.get_port())
	{
		return (true);
	}
	return (false);
}

bool t_c_individual_server_config::t_c_light_key::operator>(t_c_individual_server_config const &comparator) const
{
	if (*host_name > *comparator.get_host_name() ||
		(*host_name == *comparator.get_host_name() && port > comparator.get_port()))
	{
		return (true);
	}
	return (false);
}

bool t_c_individual_server_config::t_c_light_key::operator<(t_c_individual_server_config const &comparator) const
{
	if (*host_name < *comparator.get_host_name() ||
		(*host_name == *comparator.get_host_name() && port < comparator.get_port()))
	{
		return (true);
	}
	return (false);
}

#pragma GCC diagnostic pop
