/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Filename: get_ports.cpp                                                  */
/*   Author:   Peru Riezu <riezumunozperu@gmail.com>                          */
/*   github:   https://github.com/priezu-m                                    */
/*   Licence:  GPLv3                                                          */
/*   Created:  2024/05/28 03:27:38                                            */
/*   Updated:  2024/05/29 20:55:12                                            */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.hpp"
#include <iostream>
#include <arpa/inet.h>

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

static uint16_t get_port(t_c_token const &token, char const *config_file, int &error_count)
{
	uint16_t res;
	size_t   i;

	res = 0;
	i = 0;
	while (token.get_token()[i] != '\0')
	{
		if (isdigit(token.get_token()[i]) == 0)
		{
			std::cout << std::string(config_file) + ": " + token.get_position().to_string() +
						" : error: expected number, found: " + token.get_token() + '\n';
			error_count++;
			return (0);
		}
		if ((UINT16_MAX / 10 < res) || ((UINT16_MAX - (token.get_token()[i] - '0')) < res * 10))
		{
			std::cout << std::string(config_file) + ": " + token.get_position().to_string() +
						" : error: number in bigger than the maximum port number \n";
			error_count++;
			return (0);
		}
		res *= 10;
		res += token.get_token()[i] - '0';
		i++;
	}
	return (htons(res));
}

void get_ports(t_c_server_constructor_params &params, std::vector<t_c_token> const &tokens, size_t &i,
		char const *config_file, int &error_count)
{
	t_c_position const    position = tokens[i].get_position();
	std::vector<uint16_t> ports;
	bool                  comma_found;

	i++;
	if (params.ports_position != nullptr)
	{
			std::cout << std::string(config_file) + ": " + tokens[i].get_position().to_string() +
						" : error: redefinition of ports attribute previusly defined at: " + position.to_string() + '\n';
			error_count++;
			throw (std::invalid_argument(""));
	}
	params.ports_position = new t_c_position(position);
	comma_found = false;
	while (i < tokens.size() && tokens[i].get_token() != ";")
	{
		if (ports.empty() == false && comma_found == false)
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
		ports.push_back(get_port(tokens[i], config_file, error_count));
		comma_found = false;
		i++;
	}
	if (i == tokens.size())
	{
		std::cout << std::string(config_file) + ": error, expected a semicolon, to end the ports attribute at "
			+ position.to_string() + ", but found end of file\n";
		error_count++;
		return;
	}
	if (ports.empty() == true)
	{
		std::cout << std::string(config_file) + " " + position.to_string() + ": error, ports attribute defines nothing\n";
		delete params.ports_position;
		error_count++;
		return;
	}
	params.ports = ports;
}

#pragma GCC diagnostic pop
