/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Filename: get_hosts.cpp                                                  */
/*   Author:   Peru Riezu <riezumunozperu@gmail.com>                          */
/*   github:   https://github.com/priezu-m                                    */
/*   Licence:  GPLv3                                                          */
/*   Created:  2024/05/28 03:29:23                                            */
/*   Updated:  2024/06/02 01:52:14                                            */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.hpp"
#include <cstddef>
#include <iostream>
#include <stdexcept>
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

static void last_checks(std::vector<t_c_token> const &tokens, size_t &i, t_c_position const &position,
						std::vector<std::string> *hosts, char const *config_file)
{
	if (i == tokens.size())
	{
		throw(std::invalid_argument(std::string(config_file) +
									": error, expected a semicolon, to end the hosts attribute at " +
									position.to_string() + ", but found end of file\n"));
	}
	if (hosts->empty() == true)
	{
		throw(std::invalid_argument(std::string(config_file) + " " + position.to_string() +
									": error, hosts attribute defines nothing\n"));
	}
}

void get_hosts(t_c_server_constructor_params &params, std::vector<t_c_token> const &tokens, size_t &i,
			   char const *config_file)
{
	t_c_position const        position = tokens[i].get_position();
	std::vector<std::string> *hosts;
	bool                      comma_found;

	i++;
	if (params.host_names_position.is_valid() == true)
	{
		throw(std::invalid_argument(
			std::string(config_file) + ": " + tokens[i].get_position().to_string() +
			" : error: redefinition of hosts attribute previusly defined at: " + position.to_string() + '\n'));
	}
	params.host_names_position = position;
	hosts = new std::vector<std::string>();
	comma_found = false;
	while (i < tokens.size() && tokens[i].get_token()[0] != ';')
	{
		if (hosts->empty() == false && comma_found == false)
		{
			if (tokens[i].get_token()[0] != ',')
			{
				throw(std::invalid_argument(std::string(config_file) + ": " + tokens[i].get_position().to_string() +
											" : error: expected a comma, found: " + tokens[i].get_token() + '\n'));
			}
			comma_found = true;
			i++;
			continue;
		}
		if (is_valid_hostname(tokens[i].get_token()) == false)
		{
			throw(std::invalid_argument(std::string(config_file) + ": " + tokens[i].to_string() +
										" : error: token is not a valid hostname\n"));
		}
		hosts->push_back(tokens[i].get_token());
		comma_found = false;
		i++;
	}
	last_checks(tokens, i, position, hosts, config_file);
	params.host_names = hosts;
}

#pragma GCC diagnostic pop
