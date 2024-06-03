/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Filename: get_config.cpp                                                 */
/*   Author:   Peru Riezu <riezumunozperu@gmail.com>                          */
/*   github:   https://github.com/priezu-m                                    */
/*   Licence:  GPLv3                                                          */
/*   Created:  2024/05/26 00:53:43                                            */
/*   Updated:  2024/06/02 07:03:12                                            */
/*                                                                            */
/* ************************************************************************** */

#include "parser.hpp"
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unistd.h>
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

static t_c_server_config_token get_server_config(std::vector<t_c_token> &tokens, size_t &i, char const *config_file)
{
	t_c_server_constructor_params params;
	const t_c_position            server_token_position = tokens[i].get_position();
	const t_c_position            opening_key_position = tokens[i + 1].get_position();

	i += 2;
	while (i < tokens.size() && tokens[i].get_token()[0] != '}')
	{
		if (tokens[i].get_token() == "ports")
		{
			get_ports(params, tokens, i, config_file);
		}
		else if (tokens[i].get_token() == "hosts")
		{
			get_hosts(params, tokens, i, config_file);
		}
		else if ((tokens[i].get_token() == "http_version_not_supported") ||
				 (tokens[i].get_token() == "not_implemeted") || (tokens[i].get_token() == "internal_server_error") ||
				 (tokens[i].get_token() == "uri_too_long") || (tokens[i].get_token() == "content_too_large") ||
				 (tokens[i].get_token() == "length_requiered") || (tokens[i].get_token() == "request_timeout") ||
				 (tokens[i].get_token() == "not_found") || (tokens[i].get_token() == "forbidden") ||
				 (tokens[i].get_token() == "bad_request"))
		{
			get_error_page(params, tokens, i, config_file);
		}
		else if (tokens[i].get_token() == "client_body_size_limit")
		{
			get_client_body_size(params, tokens, i, config_file);
		}
		else if (tokens[i].get_token() == "router")
		{
			get_router(params, tokens, i, config_file);
			if (i < tokens.size() && tokens[i].get_token()[0] == '}')
			{
				i++;
			}
		}
		else
		{
			throw(std::invalid_argument(std::string(config_file) + tokens[i].get_position().to_string() +
										": error: error unrecognized token  " + tokens[i].get_token() + '\n'));
		}
		if (i < tokens.size())
		{
			i++;
		}
	}
	if (i == tokens.size())
	{
		throw(std::invalid_argument(std::string(config_file) + ": error, expected }, to match { at " +
									opening_key_position.to_string() + ", but found end of file\n"));
	}
}

t_c_global_config *get_config(char const *config_file)
{
	std::vector<t_c_token>               tokens = get_tokens(config_file);
	std::vector<t_c_server_config_token> server_configs;
	size_t                               i;

	i = 0;
	while (i < tokens.size())
	{
		if (tokens[i].get_token() != "server")
		{
			throw(std::invalid_argument(std::string(config_file) + ": " + tokens[i].get_position().to_string() +
										" : error: expected server, found: " + tokens[i].get_token() + '\n'));
		}
		if (i + 1 == tokens.size())
		{
			throw(std::invalid_argument(std::string(config_file) +
										": error, after: " + tokens[i - 1].get_position().to_string() + " " +
										tokens[i - 1].get_token() + ", expected {, but found end of file\n"));
		}
		if (tokens[i + 1].get_token()[0] != '{')
		{
			throw(std::invalid_argument(std::string(config_file) + ": " + tokens[i].get_position().to_string() +
										" : error: expected '{', found: " + tokens[i].get_token() + '\n'));
		}
		server_configs.push_back(get_server_config(tokens, i, config_file)); // will update i to refer
																			 // to the closing }
		if (i < tokens.size())
		{
			i++;
		}
	}
}

#pragma GCC diagnostic pop
