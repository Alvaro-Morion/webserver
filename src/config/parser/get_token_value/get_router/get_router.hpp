/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Filename: get_router.hpp                                                 */
/*   Author:   Peru Riezu <riezumunozperu@gmail.com>                          */
/*   github:   https://github.com/priezu-m                                    */
/*   Licence:  GPLv3                                                          */
/*   Created:  2024/05/30 20:50:46                                            */
/*   Updated:  2024/07/21 00:56:45                                            */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../../parser.hpp"

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

struct t_c_resource_constructor_params
{
		std::string  root;
		t_c_position root_position;
		std::string  file_is_a_directory_page;
		t_c_position file_is_a_directory_page_position;
		bool         is_redirect;
		t_c_position is_redirect_position;
		bool         directory_listing;
		t_c_position directory_listing_position;
		bool         post_allowed;
		bool         delet_allowed;
		bool         get_allowed;
		t_c_position methods_position;
		bool         is_cgi;
		t_c_position is_cgi_position;

		t_c_resource_constructor_params(void)
			: file_is_a_directory_page("default_error_pages/403"), is_redirect(false), directory_listing(false),
			  post_allowed(false), delet_allowed(false), get_allowed(false), is_cgi(false)
		{
		}
};

void get_root(t_c_resource_constructor_params &params, std::vector<t_c_token> &tokens, size_t &i,
			  char const *config_file);
void get_is_redirect(t_c_resource_constructor_params &params, std::vector<t_c_token> &tokens, size_t &i,
					 char const *config_file);
void get_methods(t_c_resource_constructor_params &params, std::vector<t_c_token> &tokens, size_t &i,
				 char const *config_file);
void get_is_cgi(t_c_resource_constructor_params &params, std::vector<t_c_token> &tokens, size_t &i,
				char const *config_file);
void get_file_is_a_directory_page(t_c_resource_constructor_params &params, std::vector<t_c_token> &tokens, size_t &i,
								  char const *config_file);
void get_direcory_listing(t_c_resource_constructor_params &params, std::vector<t_c_token> &tokens, size_t &i,
						  char const *config_file);

#pragma GCC diagnostic pop
