/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Filename: t_c_router.cpp                                                 */
/*   Author:   Peru Riezu <riezumunozperu@gmail.com>                          */
/*   github:   https://github.com/priezu-m                                    */
/*   Licence:  GPLv3                                                          */
/*   Created:  2024/05/07 18:17:27                                            */
/*   Updated:  2024/05/16 16:19:50                                            */
/*                                                                            */
/* ************************************************************************** */

#include "../config.hpp"
#include "t_c_router.hpp"
#include <stdexcept>
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

t_c_router::t_c_router(std::vector<t_c_route> const  &routes_param)
	: routes(routes_param)
{
	//todo: sort and validate
}

t_c_router::t_c_router(t_c_router const &copy)
	: routes(copy.routes)
{
}

t_c_router::~t_c_router(void)
{
}

bool t_c_router::operator==(t_c_router const &comparator) const
{
	if (this == &comparator)
	{
		return (true);
	}
	if (routes != comparator.get_routes())
	{
		return (false);
	}
	return (true);
}

std::vector<t_c_route> const &t_c_router::get_routes(void) const
{
	return (routes);
}

std::string t_c_router::to_string(void) const
{
	std::string string;

	for (size_t i = 0; i < routes.size(); i++)
	{
		//todo: use t_c_route::to_string() to tidy below code
		string += "\n\t\t[" + std::to_string(i) + "]: {" + "path: " + routes[i].get_path() +
				  ", resource: " + routes[i].get_resource().to_string() + "} ";
	}
	return (string);
}

#pragma GCC diagnostic pop
