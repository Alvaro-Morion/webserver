/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Filename: t_c_router.cpp                                                 */
/*   Author:   Peru Riezu <riezumunozperu@gmail.com>                          */
/*   github:   https://github.com/priezu-m                                    */
/*   Licence:  GPLv3                                                          */
/*   Created:  2024/05/07 18:17:27                                            */
/*   Updated:  2024/05/14 04:01:05                                            */
/*                                                                            */
/* ************************************************************************** */

#include "../config.hpp"
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

t_c_router::t_c_router(std::vector<std::string> const &paths_param, std::vector<t_c_resource> const &resources_param)
	: paths(paths_param), resources(resources_param)
{
	if (paths_param.size() != resources_param.size())
	{
		throw(std::invalid_argument("all paths must be routed to a resource,"
									" meaning that the paths and resources vector must have  the same lenght"));
	}
}

t_c_router::t_c_router(t_c_router const &copy) : paths(copy.paths), resources(copy.resources)
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
	if (paths != comparator.get_paths() || resources != comparator.get_resources())
	{
		return (false);
	}
	return (true);
}

std::vector<std::string> const &t_c_router::get_paths(void) const
{
	return (paths);
}

std::vector<t_c_resource> const &t_c_router::get_resources(void) const
{
	return (resources);
}

std::string t_c_router::to_string(void) const
{
	std::string string;

	for (size_t i = 0; i < paths.size(); i++)
	{
		string += "\n\t\t[" + std::to_string(i) + "]: {" + "path: " + paths[i] +
				  ", resource: " + resources[i].to_string() + "} ";
	}
	return (string);
}

#pragma GCC diagnostic pop
