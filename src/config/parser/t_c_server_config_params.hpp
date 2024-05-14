/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Filename: t_c_server_config_params.hpp                                   */
/*   Author:   Peru Riezu <riezumunozperu@gmail.com>                          */
/*   github:   https://github.com/priezu-m                                    */
/*   Licence:  GPLv3                                                          */
/*   Created:  2024/05/14 04:38:06                                            */
/*   Updated:  2024/05/14 04:40:04                                            */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../t_c_global_config/t_c_global_config.hpp"

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

struct s_t_c_server_config_params // NOLINT
{
		uint64_t                  client_body_size_limit = UINT64_MAX;
		t_c_router               *router = nullptr;
		std::vector<std::string> *hosts = new std::vector<std::string>;
		std::string              *resource_is_a_directory_page = new std::string("default_error_pages/403");
		t_c_default_error_pages  *default_error_pages = new t_c_default_error_pages();
		std::vector<uint16_t>     ports_val;

		struct s_t_c_router_parms
		{
				std::vector<std::string>  paths;
				std::vector<t_c_resource> resources;
		} router_params;

		struct s_t_c_default_error_pages_params // NOLINT
		{
				std::string http_version_not_supported = "default_error_pages/505";
				std::string not_implemeted = "default_error_pages/501";
				std::string uri_too_long = "default_error_pages/414";
				std::string content_too_large = "default_error_pages/413";
				std::string length_requiered = "default_error_pages/411";
				std::string request_timeout = "default_error_pages/408";
				std::string not_found = "default_error_pages/404";
				std::string forbidden = "default_error_pages/403";
				std::string bad_request = "default_error_pages/400";
				bool        http_version_not_supported_set = false;
				bool        not_implemeted_set = false;
				bool        uri_too_long_set = false;
				bool        content_too_large_set = false;
				bool        length_requiered_set = false;
				bool        request_timeout_set = false;
				bool        not_found_set = false;
				bool        forbidden_set = false;
				bool        bad_request_set = false;
		} default_error_pages_param;

		bool client_body_size_limit_set = false;
		bool directory_traversal = false;
		bool directory_traversal_set = false;
		bool resource_is_a_directory_page_set = false;
};

#pragma GCC diagnostic pop
