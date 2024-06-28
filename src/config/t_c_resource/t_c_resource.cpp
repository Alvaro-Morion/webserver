/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Filename: t_c_resource.cpp                                               */
/*   Author:   Peru Riezu <riezumunozperu@gmail.com>                          */
/*   github:   https://github.com/priezu-m                                    */
/*   Licence:  GPLv3                                                          */
/*   Created:  2024/05/07 18:30:24                                            */
/*   Updated:  2024/06/28 11:49:50                                            */
/*                                                                            */
/* ************************************************************************** */

#include "../config.hpp"
#include <stdexcept>
#include <string>
#include <unistd.h>

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

t_c_resource::t_c_resource(std::string const &root_param, std::string const &file_is_a_directory_page_param,
						   bool is_redirect_param, bool directory_listing_param, bool post_allowed_param,
						   bool delet_allowed_param, bool get_allowed_param, bool is_cgi_param)
	: root(root_param), file_is_a_directory_page(file_is_a_directory_page_param), is_redirect(is_redirect_param),
	  directory_listing(directory_listing_param), post_allowed(post_allowed_param), delet_allowed(delet_allowed_param),
	  get_allowed(get_allowed_param), is_cgi(is_cgi_param)
{
}

t_c_resource::t_c_resource(t_c_resource const &copy)
	: root(copy.root), file_is_a_directory_page(copy.file_is_a_directory_page), is_redirect(copy.is_redirect),
	  directory_listing(copy.directory_listing), post_allowed(copy.post_allowed), delet_allowed(copy.delet_allowed),
	  get_allowed(copy.get_allowed), is_cgi(copy.is_cgi)
{
}

t_c_resource::t_c_resource(void) : is_redirect(true), is_cgi(true)
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

	if (root != comparator.get_root() || file_is_a_directory_page != comparator.get_file_is_a_directory_page() ||
		is_redirect != comparator.get_is_redirect() || directory_listing != comparator.get_direcory_listing() ||
		post_allowed != comparator.get_post_allowed() || delet_allowed != comparator.get_delet_allowed() ||
		get_allowed != comparator.get_get_allowed() || is_cgi != comparator.get_is_cgi())
	{
		return (false);
	}
	return (true);
}

std::string const &t_c_resource::get_root(void) const
{
	return (root);
}

bool t_c_resource::get_is_redirect(void) const
{
	return (is_redirect);
}

std::string const &t_c_resource::get_file_is_a_directory_page(void) const
{
	return (file_is_a_directory_page);
}

bool t_c_resource::get_direcory_listing(void) const
{
	return (directory_listing);
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
	return ("{root: " + root + ", file_is_a_directory_page: " + file_is_a_directory_page +
			",\n\t\t\t is_redirect: " + ((is_redirect == true) ? "true" : "fasle") +
			", directory_listing: " + ((directory_listing == true) ? "true" : "fasle") +
			", post allowed: " + ((post_allowed == true) ? "true" : "fasle") +
			", delet allowed: " + ((delet_allowed == true) ? "true" : "fasle") + ", get allowed: " +
			((get_allowed == true) ? "true" : "fasle") + ", is cgi: " + ((is_cgi == true) ? "true}" : "fasle}"));
}

t_c_resource const &t_c_resource::operator=(t_c_resource const &copy)
{
	root = copy.get_root();
	file_is_a_directory_page = copy.get_file_is_a_directory_page();
	is_redirect = copy.get_is_redirect();
	directory_listing = copy.get_direcory_listing();
	post_allowed = copy.get_post_allowed();
	delet_allowed = copy.get_delet_allowed();
	get_allowed = copy.get_get_allowed();
	is_cgi = copy.get_is_cgi();
	return (*this);
}

#pragma GCC diagnostic pop
