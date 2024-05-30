/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Filename: get_config.cpp                                                 */
/*   Author:   Peru Riezu <riezumunozperu@gmail.com>                          */
/*   github:   https://github.com/priezu-m                                    */
/*   Licence:  GPLv3                                                          */
/*   Created:  2024/05/26 00:53:43                                            */
/*   Updated:  2024/05/29 20:56:57                                            */
/*                                                                            */
/* ************************************************************************** */

#include "parser.hpp"
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
#include <iostream>
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
;

static void find_next_server_token(std::vector<t_c_token> const &tokens, size_t &i)
{
	i++;
	while (i < tokens.size())
	{
		if (tokens[i].get_token() == "server")
		{
			return;
		}
		i++;
	}
}

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
			throw (std::invalid_argument(""));
		}
		if ((UINT64_MAX / 10 < res) || ((UINT64_MAX - static_cast<unsigned>(token.get_token()[i] - '0')) < res * 10))
		{
			error_count++;
			throw (std::invalid_argument(""));
		}
		res *= 10;
		res += static_cast<unsigned>(token.get_token()[i] - '0');
		i++;
	}
	return (res);
}
}

static void get_client_body_size(t_c_server_constructor_params &params, std::vector<t_c_token> const &tokens, size_t &i,
		char const *config_file, int &error_count)
{
	const t_c_position position = tokens[i].get_position();
	uint64_t           res;

	i++;
	if (i == tokens.size())
	{
		std::cout << std::string(config_file) + ": error, expected a number, to give value to the client_body_size_limit"
			" attribute at " + position.to_string() + ", but found end of file\n";
		error_count++;
		return;
	}
	res = get_value(tokens[i], config_file, error_count);

	i++;
}

static t_c_server_config_token get_server_config(std::vector<t_c_token> const &tokens, size_t &i, const char *config_file,
		int &error_count)
{
	t_c_server_constructor_params params;
	const int                     original_error_count = error_count;
	const t_c_position            server_token_position = tokens[i].get_position();
	const t_c_position            opening_key_position = tokens[i + 1].get_position();

	i += 2;
	while (i < tokens.size() && tokens[i].get_token() != "}" && error_count < 30)
	{
		if (tokens[i].get_token() == "ports")
		{
			get_ports(params, tokens, i, config_file, error_count);
		}
		else if (tokens[i].get_token() == "hosts")
		{
			get_hosts(params, tokens, i, config_file, error_count);
		}
		else if ((tokens[i].get_token() == "error_page_http_version_not_supported")
		|| (tokens[i].get_token() == "error_page_not_implemeted")
		|| (tokens[i].get_token() == "error_internal_server_error")
		|| (tokens[i].get_token() == "error_page_uri_too_long")
		|| (tokens[i].get_token() == "error_page_content_too_large")
		|| (tokens[i].get_token() == "error_page_length_requiered")
		|| (tokens[i].get_token() == "error_page_request_timeout")
		|| (tokens[i].get_token() == "error_page_not_found")
		|| (tokens[i].get_token() == "error_page_forbidden")
		|| (tokens[i].get_token() == "error_page_bad_request"))
		{
			get_error_page(params, tokens, i, config_file, error_count);
		}
		else if(tokens[i].get_token() == "client_body_size_limit")
		{
			get_client_body_size(params, tokens, i, config_file, error_count);
		}
		else if (tokens[i].get_token() == "router")
		{

		}
		else
		{

		}

		i++;
	}
	if (i == tokens.size())
	{
		std::cout << std::string(config_file) + ": error, expected }, to match { at " + opening_key_position.to_string()
		   + ", but found end of file\n";
		throw (std::invalid_argument(""));
	}
	if (original_error_count != error_count)
	{
		throw (std::invalid_argument(""));
	}
}

t_c_global_config *get_config(char const *config_file)
{
	int                                  error_count = 0;
	std::vector<t_c_token> const         tokens = get_tokens(config_file, error_count);
	std::vector<t_c_server_config_token> server_configs;
	size_t                               i;

	i = 0;
	while (i < tokens.size() && error_count < 30)
	{
		if (tokens[i].get_token() != "server")
		{
			std::cout << std::string(config_file) + ": " + tokens[i].get_position().to_string() +
						" : error: expected server, found: " + tokens[i].get_token() + '\n';
			error_count++;
			find_next_server_token(tokens, i);
			continue;
		}
		if (i + 1 == tokens.size())
		{
			std::cout << std::string(config_file) + ": error, after: " + tokens[i - 1].get_position().to_string() + " " +
				tokens[i - 1].get_token() + ", expected {, but found end of file\n";
			error_count++;
			break;
		}
		if (tokens[i + 1].get_token() != "{")
		{
			std::cout << std::string(config_file) + ": " + tokens[i].get_position().to_string() +
						" : error: expected '{', found: " + tokens[i].get_token() + '\n';
			error_count++;
			find_next_server_token(tokens, i);
			continue;
		}
		try 
		{
			server_configs.push_back(get_server_config(tokens, i, config_file, error_count)); // will update i to refer
																							  // to the closing }
		}
		catch (std::invalid_argument const &)
		{
		}
	}
	if (error_count == 30)
	{
			std::cout << "too many errors emited stoping\n";
	}
	if (error_count != 0)
	{
		throw (std::invalid_argument("invalid configuration file"));
	}
}

#pragma GCC diagnostic pop
