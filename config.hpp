/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Filename: config.hpp                                                     */
/*   Author:   Peru Riezu <riezumunozperu@gmail.com>                          */
/*   github:   https://github.com/priezu-m                                    */
/*   Licence:  GPLv3                                                          */
/*   Created:  2024/05/06 17:32:04                                            */
/*   Updated:  2024/05/09 02:41:45                                            */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <cstdint>
#include <string>
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

class t_c_resource
{
	private:
		std::string root; // path to the directory in the filesystem of the origin server considered to be the root of
						  // the path of the requested resource, it can also be a file case in wich it will always
						  // respond whit the same file regardless of the requested resource
		std::string redirect; // empty string for no redirect, if not empty it will not reply whit a file but whit a:
							  // 301 permanently moved to "redirect"
		bool post_allowed;
		bool delet_allowed;
		bool get_allowed;
		bool is_cgi;

	public:
		explicit t_c_resource(std::string const &root_param, std::string const &redirect_param, bool post_allowed_param,
							  bool delet_allowed_param, bool get_allowed_param, bool is_cgi_param);
		t_c_resource(t_c_resource const &copy);
		~t_c_resource(void);
		bool               operator==(t_c_resource const &comparator) const;

		std::string const &get_root(void) const;
		std::string const &get_redirect(void) const; // empty string for no redirect
		bool               get_post_allowed(void) const;
		bool               get_delet_allowed(void) const;
		bool               get_get_allowed(void) const;
		bool               get_is_cgi(void) const;

		std::string        to_string(void) const;
};

class t_c_router
{
	private:
		std::vector<std::string>  paths; // only paths will be considered valid uris
		std::vector<t_c_resource> resources;

	public:
		explicit t_c_router(std::vector<std::string> const  &paths_param,
							std::vector<t_c_resource> const &resources_param);
		t_c_router(t_c_router const &copy);
		~t_c_router(void);
		bool                             operator==(t_c_router const &comparator) const;

		std::vector<std::string> const  &get_paths(void) const;
		std::vector<t_c_resource> const &get_resources(void) const;

		std::string                      to_string(void) const;
};

class t_c_default_error_pages
{
	private:
		std::string http_version_not_supported = "default_error_pages/505"; // 505
		std::string not_implemeted = "default_error_pages/501";             // 501
		std::string uri_too_long = "default_error_pages/414";               // 414
		std::string content_too_large = "default_error_pages/413";          // 413
		std::string length_requiered = "default_error_pages/411";           // 411
		std::string request_timeout = "default_error_pages/408";            // 408
		std::string not_found = "default_error_pages/404";                  // 404
		std::string forbidden = "default_error_pages/403";                  // 403
		std::string bad_request = "default_error_pages/400";                // 400

	public:
		explicit t_c_default_error_pages(std::string const &http_version_not_supported_param,
										 std::string const &not_implemeted_param, std::string const &uri_too_long_param,
										 std::string const &content_too_large_param,
										 std::string const &length_requiered_param,
										 std::string const &request_timeout_param, std::string const &not_found_param,
										 std::string const &forbidden_param, std::string const &bad_request_param);
		t_c_default_error_pages(t_c_default_error_pages const &copy);
		t_c_default_error_pages(void);
		~t_c_default_error_pages(void);
		bool               operator==(t_c_default_error_pages const &comparator) const;

		std::string const &get_http_version_not_supported(void) const;
		std::string const &get_not_implemeted(void) const;
		std::string const &get_uri_too_long(void) const;
		std::string const &get_content_too_large(void) const;
		std::string const &get_length_requiered(void) const;
		std::string const &get_request_timeout(void) const;
		std::string const &get_not_found(void) const;
		std::string const &get_forbidden(void) const;
		std::string const &get_bad_request(void) const;

		std::string        to_string(void) const;
};

class t_c_server_config // binds to objects recived in construction
{
	private:
		std::vector<std::string>       host_names;
		std::vector<uint16_t>          ports; // must be in network format (big endian)
		t_c_router const              *router;
		t_c_default_error_pages const *default_error_pages;
		std::string const             *file_is_a_directory_page; // page to be served in case file is a directory
																 // assing empty string to allow directory listing
		uint64_t client_body_size_limit = UINT64_MAX;            // UINT64_MAX to disable

	public:
		explicit t_c_server_config(std::vector<std::string> const &host_names_param,
								   std::vector<uint16_t> const &ports_param, t_c_router const *router_param,
								   t_c_default_error_pages const *default_error_pages_param,
								   std::string const             *file_is_a_directory_page_param,
								   uint64_t                       client_body_size_limit_param);
		t_c_server_config(t_c_server_config const &copy);
		~t_c_server_config(void);

		std::vector<std::string> const &get_host_names(void) const;
		std::vector<uint16_t> const    &get_ports(void) const;
		t_c_router const               *get_router(void) const;
		t_c_default_error_pages const  *get_default_error_pages(void) const;
		std::string const              *get_file_is_a_directory_page(void) const;
		uint64_t                        get_client_body_size_limit(void) const;
};

class t_c_individual_server_config // perfoms no validity checks on construction, binds to objects recived in
								   // construction
{
	private:
		std::string const             *host_name;
		uint16_t                       port;
		t_c_router const              *router;
		t_c_default_error_pages const *default_error_pages;
		std::string const             *file_is_a_directory_page; // page to be served in case file is a directory
													 // construct whit empty string to allow directory listing
		uint64_t client_body_size_limit = UINT64_MAX; // UINT64_MAX to disable

	public:
		explicit t_c_individual_server_config(std::string const *host_name_param, uint16_t port_param,
											  t_c_router const              *router_param,
											  t_c_default_error_pages const *default_error_pages_param,
											  std::string const             *file_is_a_directory_page_param,
											  uint64_t                       client_body_size_limit_param);
		t_c_individual_server_config(t_c_individual_server_config const &copy);
		~t_c_individual_server_config(void);
		t_c_individual_server_config const &operator=(t_c_individual_server_config const &copy);

		std::string const                  *get_host_name(void) const;
		uint16_t                            get_port(void) const;
		t_c_router const                   *get_router(void) const;
		t_c_default_error_pages const      *get_default_error_pages(void) const;
		std::string const                  *get_file_is_a_directory_page(void) const;
		uint64_t                            get_client_body_size_limit(void) const;

		std::string                         to_string(void) const;
};

class t_c_global_config
{
	private:
		std::vector<t_c_individual_server_config> servers; // alfabetically sorted whit reagrds to the host names
														   // no duplicated responsabilityes are allowed

	public:
		explicit t_c_global_config(std::vector<t_c_server_config> const &servers_param);
		~t_c_global_config(void);

		std::vector<t_c_individual_server_config> const &get_servers(void) const;

		std::string                                      to_string(void) const;
};

#pragma GCC diagnostic pop
