/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Filename: get_ports.cpp                                                  */
/*   Author:   Peru Riezu <riezumunozperu@gmail.com>                          */
/*   github:   https://github.com/priezu-m                                    */
/*   Licence:  GPLv3                                                          */
/*   Created:  2024/05/28 03:27:38                                            */
/*   Updated:  2024/06/06 17:57:09                                            */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.hpp"
#include <arpa/inet.h>
#include <cstddef>
#include <cstdint>
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

static uint16_t get_port(t_c_token const &token, char const *config_file)
{
	uint16_t res;
	size_t   i;

	res = 0;
	i = 0;
	while (token.get_token()[i] != '\0')
	{
		if (isdigit(token.get_token()[i]) == 0)
		{
			throw(std::invalid_argument(std::string(config_file) + ":" + token.get_position().to_string() +
										" : error: expected number, found: " + token.get_token() + '\n'));
		}
		if ((UINT16_MAX / 10 < res) || ((UINT16_MAX - (token.get_token()[i] - '0')) < res * 10))
		{
			throw(std::invalid_argument(std::string(config_file) + ":" + token.get_position().to_string() +
										" : error: number in bigger than the maximum port number \n"));
		}
		res *= 10;
		res += token.get_token()[i] - '0';
		i++;
	}
	return (htons(res));
}

static void last_checks(std::vector<t_c_token> const &tokens, size_t &i, char const *config_file,
						std::vector<uint16_t> const &ports, t_c_position const &position)
{
	if (i == tokens.size())
	{
		throw(std::invalid_argument(std::string(config_file) +
									": error, expected a semicolon, to end the ports attribute at " +
									position.to_string() + ", but found end of file\n"));
	}
	if (ports.empty() == true)
	{
		throw(std::invalid_argument(std::string(config_file) + ":" + position.to_string() +
									": error, ports attribute defines nothing\n"));
	}
}

void get_ports(t_c_server_constructor_params &params, std::vector<t_c_token> const &tokens, size_t &i,
			   char const *config_file)
{
	t_c_position const    position = tokens[i].get_position();
	std::vector<uint16_t> ports;
	bool                  comma_found;

	i++;
	if (params.ports_position.is_valid() == true)
	{
		throw(std::invalid_argument(std::string(config_file) + ":" + position.to_string() +
									" : error: redefinition of ports attribute previusly defined at: " +
									params.ports_position.to_string() + '\n'));
	}
	comma_found = false;
	while (i < tokens.size() && tokens[i].get_token()[0] != ';')
	{
		if (ports.empty() == false && comma_found == false)
		{
			if (tokens[i].get_token() != ",")
			{
				throw(std::invalid_argument(std::string(config_file) + ":" + tokens[i].get_position().to_string() +
											" : error: expected a comma, found: " + tokens[i].get_token() + '\n'));
			}
			comma_found = true;
			i++;
			continue;
		}
		ports.push_back(get_port(tokens[i], config_file));
		comma_found = false;
		i++;
	}
	last_checks(tokens, i, config_file, ports, position);
	params.ports = ports;
	params.ports_position = position;
}

#pragma GCC diagnostic pop
