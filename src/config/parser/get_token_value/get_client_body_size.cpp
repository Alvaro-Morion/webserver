/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Filename: get_client_body_size.cpp                                       */
/*   Author:   Peru Riezu <riezumunozperu@gmail.com>                          */
/*   github:   https://github.com/priezu-m                                    */
/*   Licence:  GPLv3                                                          */
/*   Created:  2024/05/30 20:45:24                                            */
/*   Updated:  2024/05/30 21:36:47                                            */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.hpp"
#include <iostream>
#include <stdexcept>

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

static uint64_t get_value(t_c_token const &token, char const *config_file, int &error_count)
{
	uint64_t res;
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
			throw(std::invalid_argument(""));
		}
		if ((UINT64_MAX - 1 / 10 < res) ||
			((UINT64_MAX - 1 - static_cast<unsigned>(token.get_token()[i] - '0')) < res * 10))
		{
			std::cout << std::string(config_file) + ": " + token.get_position().to_string() +
							 " : error: value must be less than 2^64-1 \n";
			error_count++;
			throw(std::invalid_argument(""));
		}
		res *= 10;
		res += static_cast<unsigned>(token.get_token()[i] - '0');
		i++;
	}
	return (res);
}

void get_client_body_size(t_c_server_constructor_params &params, std::vector<t_c_token> const &tokens, size_t &i,
						  char const *config_file, int &error_count)
{
	const t_c_position position = tokens[i].get_position();
	uint64_t           res;

	i++;
	if (i == tokens.size())
	{
		std::cout << std::string(config_file) +
						 ": error, expected a number, to give value to the client_body_size_limit"
						 " attribute at " +
						 position.to_string() + ", but found end of file\n";
		error_count++;
		return;
	}
	try
	{
		res = get_value(tokens[i], config_file, error_count);
	}
	catch (std::invalid_argument const &)
	{
		i++;
		return ;
	}
	i++;
	if (i == tokens.size())
	{
		std::cout << std::string(config_file) +
						 ": error, expected a semicolon, to end the client_body_size_limit"
						 " attribute at " +
						 position.to_string() + ", but found end of file\n";
		error_count++;
		return;
	}
	if (tokens[i].get_token()[0] != ';')
	{
		std::cout << std::string(config_file) + tokens[i].get_position().to_string() +
						 ": error, expected a semicolon, to end the client_body_size_limit attribute at " +
						 position.to_string() + ", but found: " + tokens[i].get_token() + '\n';
		return;
	}
	params.client_body_size_limit_position = new t_c_position(position);
	params.client_body_size_limit = res;
}

#pragma GCC diagnostic pop
