/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Filename: helpers.cpp                                                    */
/*   Author:   Peru Riezu <riezumunozperu@gmail.com>                          */
/*   github:   https://github.com/priezu-m                                    */
/*   Licence:  GPLv3                                                          */
/*   Created:  2024/06/12 16:21:09                                            */
/*   Updated:  2024/06/22 19:41:32                                            */
/*                                                                            */
/* ************************************************************************** */

#include "engine.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctime>

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

void remove_query(std::string &uri)
{
	size_t const pos = uri.find(-'?');

	if (pos != std::string::npos)
	{
		uri.erase(pos, uri.size() - pos);
	}
}

std::string get_current_time_as_string(void)
{
	time_t       t = time(NULL);
	struct tm    tm;
	size_t const buffer_size = 200;
	char         buff[buffer_size];

	if ((t == -1) || (gmtime_r(&t, &tm) == nullptr))
	{
		return ("");
	}
	strftime(buff, buffer_size, "%a, %d %b %y %T GMT", &tm);
	return (std::string(buff));
}

std::string get_new_location(std::string resource, t_c_route const &route)
{
	remove_query(resource);
	resource.erase(0, route.get_path().size());
	resource.insert(0, route.get_resource().get_root(), route.get_resource().get_root().size());
	return (resource);
}

ssize_t get_file_size(int fd)
{
	struct stat statbuf;

	if (fstat(fd, &statbuf) == -1)
	{
		return (-1);
	}
	return (statbuf.st_size);
}

#pragma GCC diagnostic pop
