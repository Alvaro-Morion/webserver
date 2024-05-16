/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Filename: get_config.cpp                                                 */
/*   Author:   Peru Riezu <riezumunozperu@gmail.com>                          */
/*   github:   https://github.com/priezu-m                                    */
/*   Licence:  GPLv3                                                          */
/*   Created:  2024/05/09 17:09:37                                            */
/*   Updated:  2024/05/15 03:59:54                                            */
/*                                                                            */
/* ************************************************************************** */

#include "parser.hpp"
#include <cctype>
#include <cstddef>
#include <cstring>
#include <fcntl.h>
#include <stdexcept>
#include <string>
#include <system_error>
#include <unistd.h>
#include <vector>

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

static std::string get_file_content(char *path)
{
	std::string content;
	char        buffer[1025];
	ssize_t     return_val;
	int         filedes = open(path, O_RDONLY);

	if (filedes == -1)
	{
		throw(std::system_error(std::error_code{errno, std::system_category()},
								std::string("when triying to open file ") + path + " for reading"));
	}
	return_val = read(filedes, buffer, sizeof(buffer));
	buffer[return_val] = '\0';
	while (return_val != 0)
	{
		content.append(buffer);
		return_val = read(filedes, buffer, sizeof(buffer));
		buffer[return_val] = '\0';
	}
	if (return_val == -1)
	{
		throw(std::system_error(std::error_code{errno, std::system_category()},
								std::string("when triying to read from file ") + path));
	}
	return (content);
}

bool is_double_quote(std::string &content, size_t i)
{
	size_t count;

	if (content[i] != '\"')
	{
		return (false);
	}
	count = 0;
	while (i != 0)
	{
		i--;
		if (content[i] == '\\')
		{
			count++;
		}
		else
		{
			break;
		}
	}
	if (count % 2 == 0)
	{
		return (false);
	}
	return (true);
}

static void remove_comments(std::string &content)
{
	bool in_quotations = false;

	for (size_t i = 0; i < content.size(); i++)
	{
		if (is_double_quote(content, i) == true)
		{
			in_quotations = !in_quotations;
		}
		else if ((content[i] == '/' && in_quotations == false) &&
				 ((i != content.size() - 1) && (content[i + 1] == '/')))
		{
			size_t j;

			j = content.find('\n', i + 2);
			if (j == std::string::npos)
			{
				j = content.size() - 1;
			}
			content.erase(i, j - i);
			i -= j - i;
		}
	}
}

static size_t get_last_key(std::string &content, size_t i)
{
	size_t open_keys = 1;
	bool   in_quotations = false;

	for (i++; i < content.size() && open_keys != 0; i++)
	{
		if (content[i] == '}' && in_quotations == false)
		{
			open_keys--;
		}
		else if (content[i] == '{' && in_quotations == false)
		{
			open_keys++;
		}
		else if (content[i] == '\"')
		{
			in_quotations = !in_quotations;
		}
	}
	if (open_keys != 0)
	{
		throw(std::invalid_argument("unexpected end of file while triying to match previusly opened \"{\""));
	}
	return (i - 1);
}

static std::string get_next_server_config(std::string &content, size_t &line, size_t &colum)
{
	size_t      i;
	size_t      first_key;
	size_t      last_key;
	std::string server_config;

	i = 0;
	while (std::isspace(content[i]) != 0)
	{
		if (content[i] == '\n')
		{
			line++;
			colum = 0;
		}
		else
		{
			colum++;
		}
		i++;
	}
	if (strncmp(&content.c_str()[i], "server", 6) != 0)
	{
		throw(std::invalid_argument(std::string("error: on line: ") + std::to_string(line) +
									", colum: " + std::to_string(colum) + ",expected \"server\""));
	}
	i += 6;
	colum += 6;
	while (std::isspace(content[i]) != 0)
	{
		if (content[i] == '\n')
		{
			line++;
			colum = 0;
		}
		else
		{
			colum++;
		}
		i++;
	}
	if (content[i] != '{')
	{
		throw(std::invalid_argument(std::string("error: on line: ") + std::to_string(line) +
									", colum: " + std::to_string(colum) + ",expected \"{\""));
	}
	first_key = i;
	i++;
	colum++;
	last_key = get_last_key(content, i);
	server_config.append(content.begin() + static_cast<std::string::difference_type>(first_key),
						 content.begin() + static_cast<std::string::difference_type>(last_key));
	content.erase(content.begin(), content.begin() + static_cast<std::string::difference_type>(last_key));
	return (server_config);
}

t_c_global_config *get_config(char *path)
{
	std::string                    content;
	std::string                    server_config;
	std::vector<t_c_server_config> server_configs;
	size_t                         line;
	size_t                         colum;

	content = get_file_content(path);
	remove_comments(content);
	line = 1;
	colum = 1;
	server_config = get_next_server_config(content, line, colum);
	while (server_config.empty() == false)
	{
		server_configs.push_back(parse_server_config(server_config, line, colum));
	}
	if (server_configs.empty() == true)
	{
		throw(std::invalid_argument("configuation file configures nothing"));
	}
	return (new t_c_global_config(server_configs));
}

#pragma GCC diagnostic pop
