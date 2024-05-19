/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Filename: t_c_global_config.cpp                                          */
/*   Author:   Peru Riezu <riezumunozperu@gmail.com>                          */
/*   github:   https://github.com/priezu-m                                    */
/*   Licence:  GPLv3                                                          */
/*   Created:  2024/05/07 19:13:58                                            */
/*   Updated:  2024/05/18 17:48:32                                            */
/*                                                                            */
/* ************************************************************************** */

#include "../config.hpp"
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <string>
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

static std::vector<t_c_individual_server_config> ungroup(std::vector<t_c_server_config> const &servers)
{
	std::vector<t_c_individual_server_config> ungrouped;

	for (t_c_server_config const &server_confing : servers)
	{
		for (std::string const &host_name : *server_confing.get_host_names())
		{
			for (uint16_t port : server_confing.get_ports())
			{
				t_c_individual_server_config new_server(
					&host_name, port, server_confing.get_router(), server_confing.get_default_error_pages(),
					server_confing.get_resource_is_a_directory_page(), server_confing.get_client_body_size_limit());
				ungrouped.push_back(new_server);
			}
		}
	}
	return (ungrouped);
}

static void sort(std::vector<t_c_individual_server_config> &servers)
{
	size_t i = 1;
	size_t min;
	size_t max;
	size_t mid;
	size_t insert_in_place;

	while (i < servers.size())
	{
		min = 0;
		max = i - 1;
		mid = 0;
		while (min < max)
		{
			mid = (min + max) / 2;
			if (servers[i] < servers[mid])
			{
				if (max == mid)
				{
					max--;
				}
				else
				{
					max = mid;
				}
			}
			else if (servers[i] > servers[mid])
			{
				if (min == mid)
				{
					min++;
				}
				else
				{
					min = mid;
				}
			}
			else
			{
				min = mid;
				break;
			}
		}
		if (servers[i] < servers[min])
		{
			insert_in_place = 0;
		}
		else if (servers[i] > servers[min])
		{
			insert_in_place = 1;
		}
		else if (servers[min] == servers[i])
		{
			throw(std::invalid_argument(*servers[min].get_host_name() + ":" + std::to_string(servers[min].get_port()) +
										" is duplicated"));
		}
		else
		{
			servers.erase(servers.begin() + static_cast<std::vector<t_c_individual_server_config>::difference_type>(i));
			continue;
		}
		servers.insert(servers.begin() + static_cast<std::vector<t_c_individual_server_config>::difference_type>(
											 min + insert_in_place),
					   servers[i]);
		servers.erase(servers.begin() + static_cast<std::vector<t_c_individual_server_config>::difference_type>(i + 1));
		i++;
	}
}

t_c_global_config::t_c_global_config(std::vector<t_c_server_config> const &servers_param)
{
	servers = ungroup(servers_param);
	sort(servers);
	for (t_c_individual_server_config const &config : servers)
	{
		ports.insert(config.get_port());
	}
}

t_c_global_config::~t_c_global_config(void)
{
}

std::vector<t_c_individual_server_config> const &t_c_global_config::get_servers(void) const
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

	for (size_t i = 0; i < servers.size(); i++)
	{
		string += '[' + std::to_string(i) + "]:\n" + servers[i].to_string() + '\n';
	}
	return (string);
}



#pragma GCC diagnostic pop
