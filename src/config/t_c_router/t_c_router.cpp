/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Filename: t_c_router.cpp                                                 */
/*   Author:   Peru Riezu <riezumunozperu@gmail.com>                          */
/*   github:   https://github.com/priezu-m                                    */
/*   Licence:  GPLv3                                                          */
/*   Created:  2024/05/07 18:17:27                                            */
/*   Updated:  2024/05/18 17:45:30                                            */
/*                                                                            */
/* ************************************************************************** */

#include "t_c_router.hpp"
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

t_c_router::t_c_router(std::vector<t_c_route> const &routes_param) : routes(routes_param)
{
	size_t i = 1;
	size_t min;
	size_t max;
	size_t mid;
	size_t insert_in_place;

	while (i < routes.size())
	{
		min = 0;
		max = i - 1;
		mid = 0;
		while (min < max)
		{
			mid = (min + max) / 2;
			if (routes[i] < routes[mid])
			{
				if (max == mid)
				{
					max--;
				}
				else
				{
					max = mid;
				}
			}
			else if (routes[i] > routes[mid])
			{
				if (min == mid)
				{
					min++;
				}
				else
				{
					min = mid;
				}
			}
			else
			{
				min = mid;
				break;
			}
		}
		if (routes[i] < routes[min])
		{
			insert_in_place = 0;
		}
		else if (routes[i] > routes[min])
		{
			insert_in_place = 1;
		}
		else if (routes[min] == routes[i])
		{
			throw(std::invalid_argument(routes[min].get_path() + " is duplicated"));
		}
		else
		{
			routes.erase(routes.begin() + static_cast<std::vector<t_c_individual_server_config>::difference_type>(i));
			continue;
		}
		routes.insert(routes.begin() + static_cast<std::vector<t_c_individual_server_config>::difference_type>(
										   min + insert_in_place),
					  routes[i]);
		routes.erase(routes.begin() + static_cast<std::vector<t_c_individual_server_config>::difference_type>(i + 1));
		i++;
	}
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

std::vector<t_c_route> const &t_c_router::get_routes(void) const
{
	return (routes);
}

std::string t_c_router::to_string(void) const
{
	std::string string;

	for (size_t i = 0; i < routes.size(); i++)
	{
		string += "\n\t\t[" + std::to_string(i) + "]: {" + routes[i].to_string() + "} ";
	}
	return (string);
}

#pragma GCC diagnostic pop
