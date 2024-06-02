/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Filename: t_c_global_config.cpp                                          */
/*   Author:   Peru Riezu <riezumunozperu@gmail.com>                          */
/*   github:   https://github.com/priezu-m                                    */
/*   Licence:  GPLv3                                                          */
/*   Created:  2024/05/07 19:13:58                                            */
/*   Updated:  2024/05/27 05:16:04                                            */
/*                                                                            */
/* ************************************************************************** */

#include "../config.hpp"
#include <cstddef>
#include <cstdint>
#include <functional>
#include <set>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

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

/*static void ungroup_and_add(std::set<t_c_individual_server_config, std::less<>> &servers,
							std::vector<t_c_server_config> const                &new_servers)
{
	for (t_c_server_config const &server_confing : new_servers)
	{
		for (std::string const &host_name : *server_confing.get_host_names())
		{
			for (uint16_t port : server_confing.get_ports())
			{
				t_c_individual_server_config new_server(
					&host_name, port, server_confing.get_router(), server_confing.get_default_error_pages(),
					server_confing.get_resource_is_a_directory_page(), server_confing.get_client_body_size_limit());

				std::set<t_c_individual_server_config, std::less<>>::iterator it =
					servers.find(t_c_individual_server_config::t_c_light_key(&host_name, port));
				if (it == servers.end())
				{
					servers.insert(new_server);
				}
				else
				{
					if (*it != new_server)
					{
						throw(std::invalid_argument(*it->get_host_name() + ":" + std::to_string(it->get_port()) +
													" is duplicated"));
					}
				}
			}
		}
	}
}*/

t_c_global_config::t_c_global_config(std::set<t_c_individual_server_config, std::less<>> const &servers_param)
	: servers(servers_param)
{
	if (servers.empty() == true)
	{
		throw(std::invalid_argument("set of configured servers may not be empty"));
	}
	for (t_c_individual_server_config const &config : servers)
	{
		ports.insert(config.get_port());
	}
}

t_c_global_config::t_c_global_config(void)
{
	std::string host = "localhost";
	this->ports.insert(8080);
	this->servers.insert(t_c_individual_server_config("localhost", 8080));
}

t_c_global_config::~t_c_global_config(void)
{
}

std::set<t_c_individual_server_config, std::less<>> const &t_c_global_config::get_servers(void) const
{
	return (servers);
}

std::set<uint16_t> const &t_c_global_config::get_ports() const
{
	return (ports);
}

std::string t_c_global_config::to_string() const
{
	std::string string;
	size_t      i;

	i = 1;
	for (t_c_individual_server_config const &server : servers)
	{
		string += '[' + std::to_string(i) + "]:\n" + server.to_string() + '\n';
		i++;
	}
	return (string);
}

#pragma GCC diagnostic pop
