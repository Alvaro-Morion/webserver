/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Filename: t_c_individual_server_config.hpp                               */
/*   Author:   Peru Riezu <riezumunozperu@gmail.com>                          */
/*   github:   https://github.com/priezu-m                                    */
/*   Licence:  GPLv3                                                          */
/*   Created:  2024/05/14 04:00:48                                            */
/*   Updated:  2024/07/02 19:28:50                                            */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../t_c_server_config/t_c_server_config.hpp"
#include <stdint.h>

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

class t_c_individual_server_config // perfoms no validity checks on construction, binds to objects recived in
								   // construction
{
	private:
		std::string const     *host_name;
		uint16_t               port;
		t_c_router const      *router;
		t_c_error_pages const *error_pages;
		uint64_t               client_body_size_limit; // UINT64_MAX to disable

	public:
		class t_c_light_key;
		explicit t_c_individual_server_config(std::string const *host_name_param, uint16_t port_param,
											  t_c_router const *router_param, t_c_error_pages const *error_pages_param,
											  uint64_t client_body_size_limit_param);
		t_c_individual_server_config(t_c_individual_server_config const &copy);
		~t_c_individual_server_config(void);
		t_c_individual_server_config const &operator=(t_c_individual_server_config const &copy);
		bool                                operator==(t_c_individual_server_config const &comparator) const;
		bool                   operator==(t_c_individual_server_config::t_c_light_key const &comparator) const;
		bool                   operator>(t_c_individual_server_config const &comparator) const;
		bool                   operator>(t_c_individual_server_config::t_c_light_key const &comparator) const;
		bool                   operator<(t_c_individual_server_config const &comparator) const;
		bool                   operator<(t_c_individual_server_config::t_c_light_key const &comparator) const;

		std::string const     *get_host_name(void) const;
		uint16_t               get_port(void) const;
		t_c_router const      *get_router(void) const;
		t_c_error_pages const *get_error_pages(void) const;
		uint64_t               get_client_body_size_limit(void) const;

		std::string            to_string(void) const;
};

class t_c_individual_server_config::t_c_light_key
{
	private:
		std::string const *host_name;
		uint16_t           port;

	public:
		t_c_light_key(std::string const *host_name_param, uint16_t port_param);
		~t_c_light_key(void);

		std::string const *get_host_name(void) const;
		uint16_t           get_port(void) const;

		bool               operator==(t_c_individual_server_config const &comparator) const;
		bool               operator>(t_c_individual_server_config const &comparator) const;
		bool               operator<(t_c_individual_server_config const &comparator) const;
};

#pragma GCC diagnostic pop
