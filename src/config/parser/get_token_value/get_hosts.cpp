/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Filename: get_hosts.cpp                                                  */
/*   Author:   Peru Riezu <riezumunozperu@gmail.com>                          */
/*   github:   https://github.com/priezu-m                                    */
/*   Licence:  GPLv3                                                          */
/*   Created:  2024/05/28 03:29:23                                            */
/*   Updated:  2024/05/30 21:04:17                                            */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.hpp"
#include <cstddef>
#include <iostream>
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

static bool was_not_parsed(std::vector<t_c_token> const &tokens, size_t &i, t_c_position const &position,
						   int &error_count, std::vector<std::string> *hosts, t_c_server_constructor_params &params,
						   char const *config_file)
{
	if (i == tokens.size())
	{
		std::cout << std::string(config_file) + ": error, expected a semicolon, to end the hosts attribute at " +
						 position.to_string() + ", but found end of file\n";
		error_count++;
		return (true);
	}
	if (hosts->empty() == true)
	{
		std::cout << std::string(config_file) + " " + position.to_string() +
						 ": error, hosts attribute defines nothing\n";
		delete params.ports_position;
		delete hosts;
		params.ports_position = nullptr;
		error_count++;
		return (true);
	}
	return (false);
}

void get_hosts(t_c_server_constructor_params &params, std::vector<t_c_token> const &tokens, size_t &i,
			   char const *config_file, int &error_count)
{
	t_c_position const        position = tokens[i].get_position();
	std::vector<std::string> *hosts;
	bool                      comma_found;

	i++;
	if (params.host_names_position != nullptr)
	{
		std::cout << std::string(config_file) + ": " + tokens[i].get_position().to_string() +
						 " : error: redefinition of hosts attribute previusly defined at: " + position.to_string() +
						 '\n';
		error_count++;
		throw(std::invalid_argument(""));
	}
	params.ports_position = new t_c_position(position);
	hosts = new std::vector<std::string>();
	comma_found = false;
	while (i < tokens.size() && tokens[i].get_token() != ";")
	{
		if (hosts->empty() == false && comma_found == false)
		{
			if (tokens[i].get_token() != ",")
			{
				std::cout << std::string(config_file) + ": " + tokens[i].get_position().to_string() +
								 " : error: expected a comma, found: " + tokens[i].get_token() + '\n';
				i--;
				error_count++;
			}
			comma_found = true;
			i++;
			continue;
		}
		if (is_valid_hostname(tokens[i].get_token()) == false)
		{
			std::cout << std::string(config_file) + ": " + tokens[i].to_string() +
							 " : error: token is not a valid hostname\n";
			i++;
			error_count++;
			continue;
		}
		hosts->push_back(tokens[i].get_token());
		comma_found = false;
		i++;
	}
	if (was_not_parsed(tokens, i, position, error_count, hosts, params, config_file) == true)
	{
		return;
	}
	params.host_names = hosts;
}

#pragma GCC diagnostic pop
