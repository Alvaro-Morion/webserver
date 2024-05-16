/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Filename: get_hosts.cpp                                                  */
/*   Author:   Peru Riezu <riezumunozperu@gmail.com>                          */
/*   github:   https://github.com/priezu-m                                    */
/*   Licence:  GPLv3                                                          */
/*   Created:  2024/05/14 06:40:36                                            */
/*   Updated:  2024/05/15 06:21:26                                            */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.hpp"
#include <algorithm>
#include <cstddef>
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

std::string::iterator find_double_quote(std::string &str, size_t i)
{
	while (i < str.size())
	{
		if (is_double_quote(str, i) == true)
		{
			return (str.begin() + static_cast<std::string::difference_type>(i));
		}
		i++;
	}
	return (str.end());
}

void resolve_scape_sequences(std::string &str, size_t line, size_t colum)
{
	for (size_t i = 0; i < str.size(); colum++)
	{
		if (str[i] == '\\')
		{
			if (str[i + 1] == '"' || str[i + 1] == '\\')
			{
				str.erase(i);
			}
			else
			{
				throw(std::invalid_argument(std::string("error: on line: ") + std::to_string(line) +
											", colum: " + std::to_string(colum) +
											", character '\\' can only be followed by '\"' or '\\'"));
			}
		}
		else
		{
			i++;
		}
	}
}

static void get_element(s_t_c_server_config_params &params, std::string &server_config, size_t &i, size_t &line,
						size_t &colum)
{
	if (is_double_quote(server_config, i) == false)
	{
		throw(std::invalid_argument(std::string("error: on line: ") + std::to_string(line) +
									", colum: " + std::to_string(colum) + ", expected '\"'"));
	}
	i++;
	colum++;
	std::string::iterator host_name_end = find_double_quote(server_config, i);
	if (host_name_end == server_config.end())
	{
		throw(std::invalid_argument(std::string("error: on line: ") + std::to_string(line) +
									", colum: " + std::to_string(colum - 1) + ", no matching '\"' found"));
	}
	std::string host_name(server_config.begin() + static_cast<std::string::difference_type>(i), host_name_end);
	resolve_scape_sequences(host_name, line, colum);
	if (is_valid_hostname(host_name) == false)
	{
		throw(std::invalid_argument(
			std::string("error: on line: ") + std::to_string(line) + ", colum: " +
			std::to_string(
				colum - static_cast<size_t>(
							(host_name_end - (server_config.begin() + static_cast<std::string::difference_type>(i))))) +
			", invalid host name"));
	}
	params.hosts->push_back(host_name);
}

void get_hosts(s_t_c_server_config_params &params, std::string &server_config, size_t &line, size_t &colum, size_t &i)
{
	if (params.hosts->empty() != true)
	{
		throw(std::invalid_argument(std::string("error: on line: ") + std::to_string(line) +
									", colum: " + std::to_string(colum) + ", redefinition of hosts attribute"));
	}
	i += std::string("hosts").size();
	colum += std::string("hosts").size();
	skip_spaces(server_config, i, line, colum);
	while (i < server_config.size() || server_config[i] != ';')
	{
		get_element(params, server_config, i, line, colum);
		skip_spaces(server_config, i, line, colum);
		if (server_config[i] == ',')
		{
			i++;
			colum++;
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
	if (params.hosts->empty() == true)
	{
		throw(std::invalid_argument(std::string("error: on line: ") + std::to_string(line) +
									", colum: " + std::to_string(colum) + ", ports attribute defines no ports"));
	}
}

#pragma GCC diagnostic pop
