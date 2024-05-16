/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Filename: get_client_body_size_limit.cpp                                 */
/*   Author:   Peru Riezu <riezumunozperu@gmail.com>                          */
/*   github:   https://github.com/priezu-m                                    */
/*   Licence:  GPLv3                                                          */
/*   Created:  2024/05/15 06:35:56                                            */
/*   Updated:  2024/05/15 06:40:38                                            */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.hpp"
#include <cstdint>
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
	uint64_t res = 0;

	if (std::isdigit(server_config[i]) == 0)
	{
		throw(std::invalid_argument(std::string("error: on line: ") + std::to_string(line) +
									", colum: " + std::to_string(colum) + " '" + server_config[i] +
									"' found, when expecting a number"));
	}
	while (std::isdigit(server_config[i]) != 0)
	{
		if ((UINT16_MAX / 10 < res) || (UINT64_MAX - static_cast<uint64_t>(server_config[i] - '0') < res))
		{
			throw(std::invalid_argument(std::string("error: on line: ") + std::to_string(line) +
										", colum: " + std::to_string(colum) + ", number is too big"));
		}
		res *= 10;
		res += static_cast<uint64_t>(server_config[i] - '0');
		i++;
		colum++;
	}
	if (server_config[i] != ',' && server_config[i] != ';' && std::isspace(server_config[i]) == 0)
	{
		if ((UINT16_MAX / 10 < res) || (UINT64_MAX - static_cast<uint64_t>(server_config[i] - '0') < res))
		{
			throw(std::invalid_argument(std::string("error: on line: ") + std::to_string(line) +
										", colum: " + std::to_string(colum) + ", unexpected token found in number"));
		}
	}
	params.client_body_size_limit = res;
}
void get_client_body_size_limit(s_t_c_server_config_params &params, std::string &server_config, size_t &line,
								size_t &colum, size_t &i)
{
	if (params.client_body_size_limit_set == true)
	{
		throw(std::invalid_argument(std::string("error: on line: ") + std::to_string(line) +
									", colum: " + std::to_string(colum) +
									", redefinition of client_body_size_limit attribute"));
	}
	params.client_body_size_limit_set = true;
	i += std::string("client_body_size_limit").size();
	colum += std::string("client_body_size_limit").size();
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
