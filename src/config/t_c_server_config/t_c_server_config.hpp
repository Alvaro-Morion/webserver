/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Filename: t_c_server_config.hpp                                          */
/*   Author:   Peru Riezu <riezumunozperu@gmail.com>                          */
/*   github:   https://github.com/priezu-m                                    */
/*   Licence:  GPLv3                                                          */
/*   Created:  2024/05/14 03:58:30                                            */
/*   Updated:  2024/05/14 04:27:04                                            */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../t_c_default_error_pages/t_c_default_error_pages.hpp"

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

class t_c_server_config // binds to objects recived in construction
{
	private:
		std::vector<std::string>      *host_names;
		std::vector<uint16_t>          ports; // must be in network format (big endian)
		t_c_router const              *router;
		t_c_default_error_pages const *default_error_pages;
		std::string const             *resource_is_a_directory_page; // page to be served in case file is a directory
																	 // assing empty string to allow directory listing
		uint64_t client_body_size_limit = UINT64_MAX;                // UINT64_MAX to disable

	public:
		explicit t_c_server_config(std::vector<std::string> *host_names_param, std::vector<uint16_t> const &ports_param,
								   t_c_router const              *router_param,
								   t_c_default_error_pages const *default_error_pages_param,
								   std::string const             *resource_is_a_directory_page_param,
								   uint64_t                       client_body_size_limit_param);
		t_c_server_config(t_c_server_config const &copy);
		~t_c_server_config(void);

		std::vector<std::string> const *get_host_names(void) const;
		std::vector<uint16_t> const    &get_ports(void) const;
		t_c_router const               *get_router(void) const;
		t_c_default_error_pages const  *get_default_error_pages(void) const;
		std::string const              *get_resource_is_a_directory_page(void) const;
		uint64_t                        get_client_body_size_limit(void) const;
};

#pragma GCC diagnostic pop
