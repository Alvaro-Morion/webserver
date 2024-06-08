/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Filename: t_c_route.cpp                                                  */
/*   Author:   Peru Riezu <riezumunozperu@gmail.com>                          */
/*   github:   https://github.com/priezu-m                                    */
/*   Licence:  GPLv3                                                          */
/*   Created:  2024/05/16 16:25:01                                            */
/*   Updated:  2024/06/08 11:53:19                                            */
/*                                                                            */
/* ************************************************************************** */

#include "t_c_route.hpp"
#include <iostream>
#include <stdexcept>

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
	if (path[path.size() - 1] != '/')
	{
		throw(std::invalid_argument("path must end whit a '/'"));
	}
}

t_c_route::t_c_route(void)
{
}

t_c_route::t_c_route(t_c_route const &copy) : path(copy.path), resource(copy.resource)
{
}

t_c_route::~t_c_route(void)
{
}

t_c_route const &t_c_route::operator=(t_c_route const &copy)
{
	path = copy.path;
	resource = copy.resource;
	return (*this);
}

bool t_c_route::operator==(t_c_route const &comparator) const
{
	if (path == comparator.path && resource == comparator.get_resource())
	{
		return (true);
	}
	return (false);
}

std::string const &t_c_route::get_path(void) const
{
	return (path);
}

t_c_resource const &t_c_route::get_resource(void) const
{
	return (resource);
}

std::string t_c_route::to_string(void) const
{
	return (std::string("path: ") + path + ", resource: " + resource.to_string());
}

bool t_c_route::operator>(t_c_route const &comparator) const
{
	return (path > comparator.path);
}

bool t_c_route::operator<(t_c_route const &comparator) const
{
	return (path > comparator.path);
}

bool operator<(std::string const &comparand, t_c_route const &comparator)
{
	return (comparand < comparator.to_string());
}

#pragma GCC diagnostic pop
