/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Filename: get_elem.hpp                                                   */
/*   Author:   Peru Riezu <riezumunozperu@gmail.com>                          */
/*   github:   https://github.com/priezu-m                                    */
/*   Licence:  GPLv3                                                          */
/*   Created:  2024/05/14 04:33:23                                            */
/*   Updated:  2024/05/14 04:40:39                                            */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../t_c_server_config_params.hpp"

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

void get_ports(s_t_c_server_config_params &params, std::string &server_config, size_t &line, size_t &colum, size_t &i);
void get_hosts(s_t_c_server_config_params &params, std::string &server_config, size_t &line, size_t &colum, size_t &i);
void get_directory_traversal(s_t_c_server_config_params &params, std::string &server_config, size_t &line,
							 size_t &colum, size_t &i);
void get_resource_is_a_directory_page(s_t_c_server_config_params &params, std::string &server_config, size_t &line,
									  size_t &colum, size_t &i);
void get_client_body_size_limit(s_t_c_server_config_params &params, std::string &server_config, size_t &line,
								size_t &colum, size_t &i);
void get_router(s_t_c_server_config_params &params, std::string &server_config, size_t &line, size_t &colum, size_t &i);
void get_invalid(s_t_c_server_config_params &params, std::string &server_config, size_t &line, size_t &colum,
				 size_t &i);
void get_error_page_http_version_not_supported(s_t_c_server_config_params &params, std::string &server_config,
											   size_t &line, size_t &colum, size_t &i);
void get_error_page_not_implemeted(s_t_c_server_config_params &params, std::string &server_config, size_t &line,
								   size_t &colum, size_t &i);
void get_error_page_uri_too_long(s_t_c_server_config_params &params, std::string &server_config, size_t &line,
								 size_t &colum, size_t &i);
void get_error_page_content_too_large(s_t_c_server_config_params &params, std::string &server_config, size_t &line,
									  size_t &colum, size_t &i);
void get_error_page_length_requiered(s_t_c_server_config_params &params, std::string &server_config, size_t &line,
									 size_t &colum, size_t &i);
void get_error_page_request_timeout(s_t_c_server_config_params &params, std::string &server_config, size_t &line,
									size_t &colum, size_t &i);
void get_error_page_not_found(s_t_c_server_config_params &params, std::string &server_config, size_t &line,
							  size_t &colum, size_t &i);
void get_error_page_forbidden(s_t_c_server_config_params &params, std::string &server_config, size_t &line,
							  size_t &colum, size_t &i);
void get_error_page_bad_request(s_t_c_server_config_params &params, std::string &server_config, size_t &line,
								size_t &colum, size_t &i);
void get_error_page_internal_server_error(s_t_c_server_config_params &params, std::string &server_config, size_t &line,
										  size_t &colum, size_t &i);

#pragma GCC diagnostic pop
