/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Filename: t_c_route.hpp                                                  */
/*   Author:   Peru Riezu <riezumunozperu@gmail.com>                          */
/*   github:   https://github.com/priezu-m                                    */
/*   Licence:  GPLv3                                                          */
/*   Created:  2024/05/16 16:22:19                                            */
/*   Updated:  2024/05/18 02:54:37                                            */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../t_c_resource/t_c_resource.hpp"

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

class t_c_route
{
	private:
		std::string path;
		t_c_resource resource;

	public:
		t_c_route(std::string const &path_param, t_c_resource const &resource_param);
		t_c_route(t_c_route const &copy);
		~t_c_route(void);
		t_c_route const                              &operator=(t_c_route const &copy);
		bool                             operator==(t_c_route const &comparator) const;

		std::string	const &get_path(void) const;
		t_c_resource const &get_resource(void) const;
		std::string                      to_string(void) const;
};

#pragma GCC diagnostic pop
