/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Filename: t_c_route.cpp                                                  */
/*   Author:   Peru Riezu <riezumunozperu@gmail.com>                          */
/*   github:   https://github.com/priezu-m                                    */
/*   Licence:  GPLv3                                                          */
/*   Created:  2024/05/16 16:25:01                                            */
/*   Updated:  2024/05/16 16:27:34                                            */
/*                                                                            */
/* ************************************************************************** */

#include "t_c_route.hpp"

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

t_c_route::t_c_route(std::string const &path_param, t_c_resource const &resource_param)
	: path(path_param), resource(resource_param)
{
	if (path[i] != '/')
	{
		
	}
}

#pragma GCC diagnostic pop
