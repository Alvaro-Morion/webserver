/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Filename: get_ports.cpp                                                  */
/*   Author:   Peru Riezu <riezumunozperu@gmail.com>                          */
/*   github:   https://github.com/priezu-m                                    */
/*   Licence:  GPLv3                                                          */
/*   Created:  2024/05/13 03:16:12                                            */
/*   Updated:  2024/05/14 04:41:15                                            */
/*                                                                            */
/* ************************************************************************** */

#include <cctype>
#include <cstddef>
#include <cstdint>
#include <stdexcept>

#include "../parser.hpp"

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
	uint16_t res = 0;

	if (std::isdigit(server_config[i]) == 0)
	{
		throw(std::invalid_argument(std::string("error: on line: ") + std::to_string(line) +
									", colum: " + std::to_string(colum) + " '" + server_config[i] +
									"' found, when expecting a number"));
	}
	while (std::isdigit(server_config[i]) != 0)
	{
		if ((UINT16_MAX / 10 < res) || (UINT16_MAX - (server_config[i] - '0') < res))
		{
			throw(std::invalid_argument(std::string("error: on line: ") + std::to_string(line) +
									", colum: " + std::to_string(colum) + ", number is too big"));
		}
		res *= 10;
		res += server_config[i] - '0';
		i++;
		colum++;
	}
	if (server_config[i] != ',' && server_config[i] != ';' && std::isspace(server_config[i]) == 0)
	{
		if ((UINT16_MAX / 10 < res) || (UINT16_MAX - (server_config[i] - '0') < res))
		{
			throw(std::invalid_argument(std::string("error: on line: ") + std::to_string(line) +
									", colum: " + std::to_string(colum) + ", unexpected token found in number"));
		}
	}
	params.ports_val.push_back(res);
}

void get_ports(s_t_c_server_config_params &params, std::string &server_config, size_t &line, size_t &colum, size_t &i)
{
	if (params.ports_val.empty() != true)
	{
		throw(std::invalid_argument(std::string("error: on line: ") + std::to_string(line) +
									", colum: " + std::to_string(colum) + ", redefinition of ports attribute"));
	}
	i += std::string("ports").size();
	skip_spaces(server_config, i, line, colum);
	while (i < server_config.size() || server_config[i] != ';')
	{
		get_element(params, server_config, i, line, colum);
		skip_spaces(server_config, i, line, colum);
		if (server_config[i] == ',')
		{
			skip_spaces(server_config, i, line, colum);
		}
		else if (server_config[i] != ';')
		{
			throw(std::invalid_argument(std::string("error: on line: ") + std::to_string(line) +
										", colum: " + std::to_string(colum) + " '" + server_config[i] +
										"' found, when expecting ';' or ','"));
		}
	}
	if (server_config[i] != ';')
	{
		throw(std::invalid_argument(std::string("error: on line: ") + std::to_string(line) + ", colum: " +
									std::to_string(colum) + ", unexpected end of file while looking for ';'"));
	}
	i++;
	colum++;
	if (params.ports_val.empty() == true)
	{
		throw(std::invalid_argument(std::string("error: on line: ") + std::to_string(line) +
									", colum: " + std::to_string(colum) + ", ports attribute defines no ports"));
	}
}

#pragma GCC diagnostic pop
