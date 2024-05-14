/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Filename: t_c_resource.cpp                                               */
/*   Author:   Peru Riezu <riezumunozperu@gmail.com>                          */
/*   github:   https://github.com/priezu-m                                    */
/*   Licence:  GPLv3                                                          */
/*   Created:  2024/05/07 18:30:24                                            */
/*   Updated:  2024/05/14 04:06:36                                            */
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

t_c_resource::t_c_resource(std::string const &root_param, std::string const &redirect_param, bool post_allowed_param,
						   bool delet_allowed_param, bool get_allowed_param, bool is_cgi_param)
	: root(root_param), redirect(redirect_param), post_allowed(post_allowed_param), delet_allowed(delet_allowed_param),
	  get_allowed(get_allowed_param), is_cgi(is_cgi_param)
{
	if (root.empty() == redirect.empty())
	{
		throw(std::invalid_argument("a resource must either be a redirection or a path too a directory or file"));
	}
	if (post_allowed == true && is_cgi == false)
	{
		throw(std::invalid_argument("A resource whit a post method must be a cgi"));
	}
	// todo: test if redirect is a valid uri, throw if not
	// todo: test if root is a syntatcically valid path, throw if not
}

t_c_resource::t_c_resource(t_c_resource const &copy)
	: root(copy.root), redirect(copy.redirect), post_allowed(copy.post_allowed), delet_allowed(copy.delet_allowed),
	  get_allowed(copy.get_allowed), is_cgi(copy.is_cgi)
{
}

t_c_resource::~t_c_resource(void)
{
}

bool t_c_resource::operator==(t_c_resource const &comparator) const
{
	if (this == &comparator)
	{
		return (true);
	}
	if (delet_allowed != comparator.get_delet_allowed() || get_allowed != comparator.get_get_allowed() ||
		post_allowed != comparator.get_post_allowed() || is_cgi != comparator.get_is_cgi() ||
		root != comparator.get_root() || redirect != comparator.get_redirect())
	{
		return (false);
	}
	return (true);
}

std::string const &t_c_resource::get_root(void) const
{
	return (root);
}

std::string const &t_c_resource::get_redirect(void) const
{
	return (redirect);
}

bool t_c_resource::get_post_allowed(void) const
{
	return (post_allowed);
}

bool t_c_resource::get_delet_allowed(void) const
{
	return (delet_allowed);
}

bool t_c_resource::get_get_allowed(void) const
{
	return (get_allowed);
}

bool t_c_resource::get_is_cgi(void) const
{
	return (is_cgi);
}

std::string t_c_resource::to_string() const
{
	return ("{root: " + root + ", redirect: " + redirect +
			",\n\t\t\t post allowed: " + ((post_allowed == true) ? "true" : "fasle") +
			", delet allowed: " + ((delet_allowed == true) ? "true" : "fasle") + ", get allowed: " +
			((get_allowed == true) ? "true" : "fasle") + ", is cgi: " + ((is_cgi == true) ? "true}" : "fasle}"));
}

#pragma GCC diagnostic pop
