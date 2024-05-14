/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Filename: parse_server_config.cpp                                        */
/*   Author:   Peru Riezu <riezumunozperu@gmail.com>                          */
/*   github:   https://github.com/priezu-m                                    */
/*   Licence:  GPLv3                                                          */
/*   Created:  2024/05/10 16:29:43                                            */
/*   Updated:  2024/05/14 04:36:20                                            */
/*                                                                            */
/* ************************************************************************** */

#include <cctype>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

#include "parser.hpp"

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

typedef enum class e_c_token_type : int
{
	ports = 0,
	hosts,
	directory_traversal,
	resource_is_a_directory_page,
	client_body_size_limit,
	router,
	invalid,
	error_page_http_version_not_supported,
	error_page_not_implemeted,
	error_page_uri_too_long,
	error_page_content_too_large,
	error_page_length_requiered,
	error_page_request_timeout,
	error_page_not_found,
	error_page_forbidden,
	error_page_bad_request,
	error_page_internal_server_error
} t_e_c_token_type;

void skip_spaces(std::string const &server_config, size_t &i, size_t &line, size_t &colum)
{
	while (std::isspace(server_config[i]) != 0)
	{
		colum++;
		if (server_config[i] == '\n')
		{
			colum = 0;
			line++;
		}
		i++;
	}
}

static t_e_c_token_type get_token_type(char const *str)
{
	std::string      keys[] = {std::string("ports"),
							   std::string("hosts"),
							   std::string("directory_traversal"),
							   std::string("resource_is_a_directory_page"),
							   std::string("client_body_size_limit"),
							   std::string("router"),
							   std::string("invalid"),
							   std::string("error_page_http_version_not_supported"),
							   std::string("error_page_not_implemeted"),
							   std::string("error_page_uri_too_long"),
							   std::string("error_page_content_too_large"),
							   std::string("error_page_length_requiered"),
							   std::string("error_page_request_timeout"),
							   std::string("error_page_not_found"),
							   std::string("error_page_forbidden"),
							   std::string("error_page_bad_request"),
							   std::string("error_page_internal_server_error")};
	t_e_c_token_type values[] = {t_e_c_token_type::ports,
								 t_e_c_token_type::hosts,
								 t_e_c_token_type::directory_traversal,
								 t_e_c_token_type::resource_is_a_directory_page,
								 t_e_c_token_type::client_body_size_limit,
								 t_e_c_token_type::router,
								 t_e_c_token_type::invalid,
								 t_e_c_token_type::error_page_http_version_not_supported,
								 t_e_c_token_type::error_page_not_implemeted,
								 t_e_c_token_type::error_page_uri_too_long,
								 t_e_c_token_type::error_page_content_too_large,
								 t_e_c_token_type::error_page_length_requiered,
								 t_e_c_token_type::error_page_request_timeout,
								 t_e_c_token_type::error_page_not_found,
								 t_e_c_token_type::error_page_forbidden,
								 t_e_c_token_type::error_page_bad_request,
								 t_e_c_token_type::error_page_internal_server_error};

	for (size_t i = 0; i < sizeof(keys) / sizeof(keys[0]); i++)
	{
		if (strncmp(str, keys[i].c_str(), keys[i].size()) == 0)
		{
			if (std::isspace(str[keys[i].size()]) != 0 || str[keys[i].size()] == ';')
			{
				return (values[i]);
			}
			return (t_e_c_token_type::invalid);
		}
	}
	return (t_e_c_token_type::invalid);
}

t_c_server_config parse_server_config(std::string &server_config, size_t &line, size_t &colum)
{
	s_t_c_server_config_params params;
	size_t                     i;
	void (*(call[]))(s_t_c_server_config_params &params, std::string &server_config, size_t &line, size_t &colum,
					 size_t &i) = {get_ports,
								   get_hosts,
								   get_directory_traversal,
								   get_resource_is_a_directory_page,
								   get_client_body_size_limit,
								   get_router,
								   get_invalid,
								   get_error_page_http_version_not_supported,
								   get_error_page_not_implemeted,
								   get_error_page_uri_too_long,
								   get_error_page_content_too_large,
								   get_error_page_length_requiered,
								   get_error_page_request_timeout,
								   get_error_page_not_found,
								   get_error_page_forbidden,
								   get_error_page_bad_request,
								   get_error_page_bad_request};

	colum++;
	i = 1;
	while (i != server_config.size() - 2)
	{
		if (std::isspace(server_config[i]) != 0)
		{
			skip_spaces(server_config, i, line, colum);
		}
		else
		{
			call[static_cast<int>(get_token_type(server_config.c_str() + i))](params, server_config, line, colum, i);
		}
	}
	return (t_c_server_config(params.hosts, params.ports_val, params.router, params.default_error_pages,
							  params.resource_is_a_directory_page, params.client_body_size_limit));
}

#pragma GCC diagnostic pop
