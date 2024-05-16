/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Filename: get_router.cpp                                                 */
/*   Author:   Peru Riezu <riezumunozperu@gmail.com>                          */
/*   github:   https://github.com/priezu-m                                    */
/*   Licence:  GPLv3                                                          */
/*   Created:  2024/05/15 06:41:37                                            */
/*   Updated:  2024/05/15 07:45:08                                            */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.hpp"
#include <stdexcept>
#include <string.h>

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
	if (server_config[i] != '{')
	{
		throw(std::invalid_argument(std::string("error: on line: ") + std::to_string(line) + ", colum: " +
									std::to_string(colum) + ", '" + server_config[i] + "', found when expecting '{'"));
	}
	i++;
	colum++;
	while (server_config[i] != '}')
	{
		skip_spaces(server_config, i, line, colum);
		if (strncmp(server_config.c_str() + i, "route", 5) != 0)
		{
			throw(std::invalid_argument(std::string("error: on line: ") + std::to_string(line) + ", colum: " +
										std::to_string(colum) + ", unrecognized token found while expecting \"route\""));
		}
		insert_route(params.router_params, get_route(server_config, i, line, colum));
		skip_spaces(server_config, i, line, colum);
	}
}

void get_router(s_t_c_server_config_params &params, std::string &server_config, size_t &line,
								size_t &colum, size_t &i)
{
	if (params.router != nullptr)
	{
		throw(std::invalid_argument(std::string("error: on line: ") + std::to_string(line) +
									", colum: " + std::to_string(colum) +
									", redefinition of the router attribute"));
	}
	i += std::string("router").size();
	colum += std::string("router").size();
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
