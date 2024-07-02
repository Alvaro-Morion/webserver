/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Filename: t_c_resource.hpp                                               */
/*   Author:   Peru Riezu <riezumunozperu@gmail.com>                          */
/*   github:   https://github.com/priezu-m                                    */
/*   Licence:  GPLv3                                                          */
/*   Created:  2024/05/14 03:52:48                                            */
/*   Updated:  2024/07/02 19:26:14                                            */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>

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

class t_c_resource
{
	private:
		std::string root;
		std::string file_is_a_directory_page;
		bool        is_redirect;
		bool        directory_listing;
		bool        post_allowed;
		bool        delet_allowed;
		bool        get_allowed;
		bool        is_cgi;

	public:
		explicit t_c_resource(std::string const &root_param, std::string const &file_is_a_directory_page_param,
							  bool is_redirect_param, bool directory_listing_param, bool post_allowed_param,
							  bool delet_allowed_param, bool get_allowed_param, bool is_cgi_param);
		t_c_resource(t_c_resource const &copy);
		t_c_resource(void);
		~t_c_resource(void);
		bool                operator==(t_c_resource const &comparator) const;
		t_c_resource const &operator=(t_c_resource const &copy);

		std::string const  &get_root(void) const;
		std::string const  &get_file_is_a_directory_page(void) const;
		bool                get_is_redirect(void) const;
		bool                get_direcory_listing(void) const;
		bool                get_post_allowed(void) const;
		bool                get_delet_allowed(void) const;
		bool                get_get_allowed(void) const;
		bool                get_is_cgi(void) const;

		std::string         to_string(void) const;
};

#pragma GCC diagnostic pop
