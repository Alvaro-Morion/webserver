/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Filename: t_c_global_config.cpp                                          */
/*   Author:   Peru Riezu <riezumunozperu@gmail.com>                          */
/*   github:   https://github.com/priezu-m                                    */
/*   Licence:  GPLv3                                                          */
/*   Created:  2024/05/07 19:13:58                                            */
/*   Updated:  2024/07/21 01:06:41                                            */
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

t_c_global_config::t_c_global_config(std::set<t_c_individual_server_config, std::less<> > const &servers_param)
	: servers(servers_param)
{
	if (servers.empty() == true)
	{
		throw(std::invalid_argument("set of configured servers may not be empty"));
	}
	for (std::set<t_c_individual_server_config, std::less<> >::iterator it = servers.begin(); it != servers.end(); it++)
	{
		ports.insert(it->get_port());
	}
}

t_c_global_config::~t_c_global_config(void)
{
}

std::set<t_c_individual_server_config, std::less<> > const &t_c_global_config::get_servers(void) const
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
	for (std::set<t_c_individual_server_config, std::less<> >::iterator it = servers.begin(); it != servers.end(); it++)
	{
		string += '[' + ::to_string(i) + "]:\n" + it->to_string() + '\n';
		i++;
	}
	return (string);
}

#pragma GCC diagnostic pop
