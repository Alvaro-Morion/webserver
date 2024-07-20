/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Filename: get_error_page.cpp                                             */
/*   Author:   Peru Riezu <riezumunozperu@gmail.com>                          */
/*   github:   https://github.com/priezu-m                                    */
/*   Licence:  GPLv3                                                          */
/*   Created:  2024/05/28 17:58:15                                            */
/*   Updated:  2024/07/21 00:54:53                                            */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.hpp"
#include <cstddef>
#include <fcntl.h>
#include <iostream>
#include <stdexcept>
#include <unistd.h>
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

void remove_quotes_if_present(std::vector<t_c_token> &tokens, size_t &i, char const *config_file)
{
	if (tokens[i].get_token()[0] == '\"')
	{
		tokens[i].get_token_non_const().erase(0, 1);
		tokens[i].get_position_non_const().add_one_to_colum();
		if ((tokens[i].get_token().empty() == true) ||
			(tokens[i].get_token()[tokens[i].get_token().size() - 1] != '\"'))
		{
			throw(std::invalid_argument(std::string(config_file) + ": error, expected a \", to end match \" found at " +
										tokens[i].get_position().to_string() + "\n"));
		}
		tokens[i].get_token_non_const().erase(tokens[i].get_token().size() - 1, 1);
	}
}

static void get_error_page_internal(std::pair<std::string, std::pair<std::string *, t_c_position *> > &elem,
									std::vector<t_c_token> &tokens, size_t &i, char const *config_file,
									t_c_position const &position)
{
	if (elem.second.second->is_valid() == true)
	{
		throw(std::invalid_argument(std::string(config_file) + ":" + tokens[i - 1].get_position().to_string() +
									" : error: redefinition of " + tokens[i - 1].get_token() +
									" attribute previusly defined at: " + (*elem.second.second).to_string() + '\n'));
	}
	if (i == tokens.size())
	{
		throw(std::invalid_argument(std::string(config_file) + ":" + position.to_string() + " : error: after" +
									tokens[i - 1].to_string() + ", expected filename but found end of file\n"));
	}
	remove_quotes_if_present(tokens, i, config_file);
	if (access(tokens[i].get_token().c_str(), R_OK) == -1)
	{
		throw(std::invalid_argument(std::string(config_file) + ":" + tokens[i].get_position().to_string() +
									" : error: " + tokens[i].to_string() + ", does not exist or cannot be read\n"));
	}
	i++;
	if (i == tokens.size())
	{
		throw(std::invalid_argument(std::string(config_file) + ": error, expected a semicolon, to end the " +
									tokens[i - 2].get_token() + " attribute defined in " + position.to_string() +
									", but found end of file\n"));
	}
	if (tokens[i].get_token()[0] != ';')
	{
		throw(std::invalid_argument(std::string(config_file) + ": error, expected a semicolon, to end the " +
									tokens[i - 2].get_token() + " attribute at " + position.to_string() +
									", but found: " + tokens[i].get_token() + "\n"));
	}
	*elem.second.first = tokens[i - 1].get_token();
	*elem.second.second = position;
}

void get_error_page(t_c_server_constructor_params &params, std::vector<t_c_token> &tokens, size_t &i,
					char const *config_file)
{
	t_c_position                                                     position = tokens[i].get_position();
	std::pair<std::string, std::pair<std::string *, t_c_position *> > table[] = {
		{"http_version_not_supported",
		 {&params.error_params.http_version_not_supported, &params.error_params.http_version_not_supported_position}          },
		{			"not_implemeted",     {&params.error_params.not_implemeted, &params.error_params.not_implemeted_position}},
		{     "internal_server_error",
		 {&params.error_params.internal_server_error, &params.error_params.internal_server_error_position}                    },
		{			  "uri_too_long",         {&params.error_params.uri_too_long, &params.error_params.uri_too_long_position}},
		{		 "content_too_large",
		 {&params.error_params.content_too_large, &params.error_params.content_too_large_position}                            },
		{		  "length_requiered", {&params.error_params.length_requiered, &params.error_params.length_requiered_position}},
		{		   "request_timeout",   {&params.error_params.request_timeout, &params.error_params.request_timeout_position}},
		{				 "not_found",               {&params.error_params.not_found, &params.error_params.not_found_position}},
		{				 "forbidden",               {&params.error_params.forbidden, &params.error_params.forbidden_position}},
		{			   "bad_request",           {&params.error_params.bad_request, &params.error_params.bad_request_position}}
    };

//	for (std::pair<std::string, std::pair<std::string *, t_c_position *> > &elem : table)
	for (size_t j = 0; j < (sizeof(table) / sizeof(table[0])); j++)
	{
		if (table[j].first == tokens[i].get_token())
		{
			i++;
			get_error_page_internal(table[j], tokens, i, config_file, position);
			return;
		}
	}
	throw(std::invalid_argument(std::string(config_file) + ":" + position.to_string() +
								": error, unrecognized token: " + tokens[i].get_token() + "\n"));
}

#pragma GCC diagnostic pop
