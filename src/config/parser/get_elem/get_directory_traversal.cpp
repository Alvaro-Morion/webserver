/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Filename: get_directory_traversal.cpp                                    */
/*   Author:   Peru Riezu <riezumunozperu@gmail.com>                          */
/*   github:   https://github.com/priezu-m                                    */
/*   Licence:  GPLv3                                                          */
/*   Created:  2024/05/15 04:37:42                                            */
/*   Updated:  2024/05/15 06:19:03                                            */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.hpp"
#include "get_elem.hpp"
#include <cctype>
#include <cstring>
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

static void get_element(s_t_c_server_config_params &params, std::string &server_config, size_t &i, size_t &line,
						size_t &colum)
{
	if (strncmp(server_config.c_str() + i, "off", 3) == 0)
	{
		i += 3;
		colum += 3;
		if (isspace(server_config[i]) == 0 && server_config[i] != ';')
		{
			throw(std::invalid_argument(std::string("error: on line: ") + std::to_string(line) +
										", colum: " + std::to_string(colum - 1) +
										", directory_traversal can only be set to on or off"));
		}
		params.directory_traversal = false;
	}
	else if (strncmp(server_config.c_str() + i, "on", 2) == 0)
	{
		i += 2;
		colum += 2;
		if (isspace(server_config[i]) == 0 && server_config[i] != ';')
		{
			throw(std::invalid_argument(std::string("error: on line: ") + std::to_string(line) +
										", colum: " + std::to_string(colum - 1) +
										", directory_traversal can only be set to on or off"));
		}
		params.directory_traversal = true;
	}
	else
	{
		throw(std::invalid_argument(std::string("error: on line: ") + std::to_string(line) + ", colum: " +
									std::to_string(colum - 1) + ", directory_traversal can only be set to on or off"));
	}
}

void get_directory_traversal(s_t_c_server_config_params &params, std::string &server_config, size_t &line,
							 size_t &colum, size_t &i)
{
	if (params.directory_traversal_set == true)
	{
		throw(std::invalid_argument(std::string("error: on line: ") + std::to_string(line) + ", colum: " +
									std::to_string(colum) + ", redefinition of directory_traversal attribute"));
	}
	params.directory_traversal_set = true;
	i += std::string("directory_traversal").size();
	colum += std::string("directory_traversal").size();
	skip_spaces(server_config, i, line, colum);
	get_element(params, server_config, i, line, colum);
	skip_spaces(server_config, i, line, colum);
	if (server_config[i] != ';')
	{
		throw(std::invalid_argument(std::string("error: on line: ") + std::to_string(line) + ", colum: " +
									std::to_string(colum) + ", unexpected end of file while looking for ';'"));
	}
	i++;
	colum++;
}

#pragma GCC diagnostic pop
