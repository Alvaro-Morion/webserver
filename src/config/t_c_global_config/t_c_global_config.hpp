/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Filename: t_c_global_config.hpp                                          */
/*   Author:   Peru Riezu <riezumunozperu@gmail.com>                          */
/*   github:   https://github.com/priezu-m                                    */
/*   Licence:  GPLv3                                                          */
/*   Created:  2024/05/14 04:03:54                                            */
/*   Updated:  2024/05/14 04:27:47                                            */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../t_c_individual_server_config/t_c_individual_server_config.hpp"

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
