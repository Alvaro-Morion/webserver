/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Filename: handle_dir.cpp                                                 */
/*   Author:   Peru Riezu <riezumunozperu@gmail.com>                          */
/*   github:   https://github.com/priezu-m                                    */
/*   Licence:  GPLv3                                                          */
/*   Created:  2024/06/12 16:56:16                                            */
/*   Updated:  2024/06/12 18:56:24                                            */
/*                                                                            */
/* ************************************************************************** */

#include "engine.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
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

static std::string get_relative_directory_name(std::string const &absolute, std::string const &base)
{
	
}

ReturnType handle_dir(std::string &resource, t_c_route const &route, t_c_individual_server_config const &config, struct stat statbuf)
{
	DIR const                      *dirp = opendir(resource.c_str());
	std::string const              relative_directory_name = get_relative_directory_name(resource, route.get_path());
	std::vector<c_directory_entry> directory_entries;

	if (((statbuf.st_mode & S_IRUSR) == 0) && ((statbuf.st_mode & S_IRGRP) == 0))
	{
		return (handle_error(403, config)); // forbidden
	}
	if (route.get_resource().get_direcory_listing() == false)
	{
		handle_error_internal_internal(route.get_resource().get_file_is_a_directory_page(), "HTTP/1.1 403 Forbidden\r\n");
	}
	if (dirp == nullptr)
	{
		return (handle_error(500, config)); // internal server error
	}
	directory_entries = get_directory_entries(dirp);
	
	
}

#pragma GCC diagnostic pop
