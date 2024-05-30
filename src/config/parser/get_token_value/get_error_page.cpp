/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Filename: get_error_page.cpp                                             */
/*   Author:   Peru Riezu <riezumunozperu@gmail.com>                          */
/*   github:   https://github.com/priezu-m                                    */
/*   Licence:  GPLv3                                                          */
/*   Created:  2024/05/28 17:58:15                                            */
/*   Updated:  2024/05/29 20:26:08                                            */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.hpp"
#include <iostream>
#include <fcntl.h>
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

static void get_error_page_internal(std::pair<std::string, std::pair<std::string *, t_c_position **>> &elem,
		std::vector<t_c_token> const &tokens, size_t &i,
		char const *config_file, int &error_count, t_c_position const &position)
{
	if (elem.second.second != nullptr)
	{
		std::cout << std::string(config_file) + ": " + tokens[i].get_position().to_string() +
				" : error: redefinition of " + tokens[i].get_token() + " attribute previusly defined at: "
				+ position.to_string() + '\n';
		error_count++;
		i++;
		return;
	}
	if (i == tokens.size())
	{
		std::cout << std::string(config_file) + ": " + position.to_string() +
			" : error: after" + tokens[i - 1].to_string() + ", expected filename but found end of file\n";
		error_count++;
		return;
	}
	if (access(tokens[i].get_token().c_str(), R_OK) == -1)
	{
		std::cout << std::string(config_file) + ": " + tokens[i].get_position().to_string() +
			" : error: " + tokens[i].to_string() + ", does not exist or cannot be read\n";
		error_count++;
		i++;
		return;
	}
	if (i == tokens.size())
	{
		std::cout << std::string(config_file) + ": error, expected a semicolon, to end the "
		+ tokens[i - 2].get_token() + " attribute at "
		+ position.to_string() + ", but found end of file\n";
		error_count++;
		i++;
		return;
	}
	if (tokens[i].to_string() != ";")
	{
		std::cout << std::string(config_file) + ": error, expected a semicolon, to end the "
		+ tokens[i - 2].get_token() + " attribute at "
		+ position.to_string() + ", but found: " + tokens[i].get_token() + "\n";
		error_count++;
		i++;
		return;
	}
	*elem.second.first = tokens[i - 1].get_token();
	*elem.second.second = new t_c_position(position);
}

void get_error_page(t_c_server_constructor_params &params, std::vector<t_c_token> const &tokens, size_t &i,
		char const *config_file, int &error_count)
{
	t_c_position                                                      position = tokens[i].get_position();
	std::pair<std::string, std::pair<std::string *, t_c_position **>> table[] =
	{
		{"error_page_http_version_not_supported", {&params.http_version_not_supported,
													  &params.http_version_not_supported_position}},
		{"error_page_not_implemeted", {&params.not_implemeted,
										  &params.not_implemeted_position}},
		{"error_internal_server_error", {&params.internal_server_error,
											&params.internal_server_error_position}},
		{"error_page_uri_too_long", {&params.uri_too_long,
										&params.uri_too_long_position}},
		{"error_page_content_too_large", {&params.content_too_large,
											 &params.content_too_large_position}},
		{"error_page_length_requiered", {&params.length_requiered,
											&params.length_requiered_position}},
		{"error_page_request_timeout", {&params.request_timeout,
										   &params.request_timeout_position}},
		{"error_page_not_found", {&params.not_found,
									 &params.not_found_position}},
		{"error_page_forbidden", {&params.forbidden,
									 &params.forbidden_position}},
		{"error_page_bad_request", {&params.bad_request,
									   &params.bad_request_position}}
	};

	for (std::pair<std::string, std::pair<std::string *, t_c_position **>> &elem : table)
	{
		if (elem.first == tokens[i].get_token())
		{
			i++;
			get_error_page_internal(elem, tokens, i, config_file, error_count, position);
			return;
		}
	}
	std::cout << std::string(config_file) + position.to_string() + ": error, unrecognized token: " + tokens[i].get_token()
		+ "\n";
	i++;
	error_count++;
}

#pragma GCC diagnostic pop
