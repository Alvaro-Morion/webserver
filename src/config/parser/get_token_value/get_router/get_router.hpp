/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Filename: get_router.hpp                                                 */
/*   Author:   Peru Riezu <riezumunozperu@gmail.com>                          */
/*   github:   https://github.com/priezu-m                                    */
/*   Licence:  GPLv3                                                          */
/*   Created:  2024/05/30 20:50:46                                            */
/*   Updated:  2024/06/02 07:05:40                                            */
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
	std::string   root;
	t_c_position  root_position;
	std::string   redirect;
	t_c_position  redirect_position;
	std::string   file_is_a_directory_page = "default_error_pages/403";
	t_c_position  file_is_a_directory_page_position;
	bool          directory_listing = false;
	t_c_position  directory_listing_position;
	bool          post_allowed = false;
	t_c_position  post_allowed_position;
	bool          delet_allowed = false;
	t_c_position  delet_allowed_positioin;
	bool          get_allowed = true;
	t_c_position  get_allowed_position;
	bool          is_cgi = false;
	t_c_position  is_cgi_position;
};

void	get_root(t_c_resource_constructor_params &params, std::vector<t_c_token> &tokens, size_t &i,
			char const *config_file);
void	get_redirect(t_c_resource_constructor_params &params, std::vector<t_c_token> &tokens, size_t &i,
			char const *config_file);
void	get_methods(t_c_resource_constructor_params &params, std::vector<t_c_token> &tokens, size_t &i,
			char const *config_file);
void	get_is_cgi(t_c_resource_constructor_params &params, std::vector<t_c_token> &tokens, size_t &i,
			char const *config_file);
void	get_resource_is_a_directory_error_page(t_c_resource_constructor_params &params, std::vector<t_c_token> &tokens,
			size_t &i, char const *config_file);
void	get_direcory_listing(t_c_resource_constructor_params &params, std::vector<t_c_token> &tokens,
			size_t &i, char const *config_file);

#pragma GCC diagnostic pop
