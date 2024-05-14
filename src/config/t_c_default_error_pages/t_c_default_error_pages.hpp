/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Filename: t_c_default_error_pages.hpp                                    */
/*   Author:   Peru Riezu <riezumunozperu@gmail.com>                          */
/*   github:   https://github.com/priezu-m                                    */
/*   Licence:  GPLv3                                                          */
/*   Created:  2024/05/14 03:56:06                                            */
/*   Updated:  2024/05/14 04:25:44                                            */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../t_c_router/t_c_router.hpp" // here to add types in the include chain of config.hpp dont remove

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

class t_c_default_error_pages
{
	private:
		std::string http_version_not_supported = "default_error_pages/505"; // 505
		std::string not_implemeted = "default_error_pages/501";             // 501
		std::string internal_server_error = "default_error_pages/500";      // 500
		std::string uri_too_long = "default_error_pages/414";               // 414
		std::string content_too_large = "default_error_pages/413";          // 413
		std::string length_requiered = "default_error_pages/411";           // 411
		std::string request_timeout = "default_error_pages/408";            // 408
		std::string not_found = "default_error_pages/404";                  // 404
		std::string forbidden = "default_error_pages/403";                  // 403
		std::string bad_request = "default_error_pages/400";                // 400

	public:
		explicit t_c_default_error_pages(std::string const &http_version_not_supported_param,
										 std::string const &not_implemeted_param,
										 std::string const &internal_server_error_param,
										 std::string const &uri_too_long_param,
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

#pragma GCC diagnostic pop
