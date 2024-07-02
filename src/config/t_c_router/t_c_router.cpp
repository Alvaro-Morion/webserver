/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Filename: t_c_router.cpp                                                 */
/*   Author:   Peru Riezu <riezumunozperu@gmail.com>                          */
/*   github:   https://github.com/priezu-m                                    */
/*   Licence:  GPLv3                                                          */
/*   Created:  2024/05/07 18:17:27                                            */
/*   Updated:  2024/07/02 19:48:16                                            */
/*                                                                            */
/* ************************************************************************** */

#include "t_c_router.hpp"
#include "../config.hpp"
#include <stddef.h>
#include <functional>
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

t_c_router::t_c_router(std::set<t_c_route, std::less<> > const &routes_param) : routes(routes_param)
{
}

t_c_router::t_c_router(t_c_router const &copy) : routes(copy.routes)
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

std::set<t_c_route, std::less<> > const &t_c_router::get_routes(void) const
{
	return (routes);
}

std::string t_c_router::to_string(void) const
{
	std::string string;
	size_t      i;

	i = 1;
	for (t_c_route const &route : routes)
	{
		string += "\n\t\t[" + std::to_string(i) + "]: {" + route.to_string() + "} ";
		i++;
	}
	return (string);
}

#pragma GCC diagnostic pop
