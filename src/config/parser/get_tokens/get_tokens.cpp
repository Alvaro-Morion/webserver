/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Filename: get_tokens.cpp                                                 */
/*   Author:   Peru Riezu <riezumunozperu@gmail.com>                          */
/*   github:   https://github.com/priezu-m                                    */
/*   Licence:  GPLv3                                                          */
/*   Created:  2024/05/27 07:58:56                                            */
/*   Updated:  2024/07/21 01:01:35                                            */
/*                                                                            */
/* ************************************************************************** */

#include "get_tokens.hpp"
#include <cstddef>
#include <fcntl.h>
#include <iostream>
#include <stdexcept>
#include <string.h>
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

static int open_throw_if_fail(char const *file, int mode)
{
	int fd = open(file, mode);

	if (fd == -1)
	{
		throw(std::invalid_argument(std::string(file) + " does not exist or cannot be read"));
	}
	return (fd);
}

static ssize_t read_throw_if_fail(int fd, char *buffer, size_t read_size)
{
	ssize_t read_ret = read(fd, buffer, read_size);

	if (read_ret == -1)
	{
		throw(std::system_error(std::error_code(errno, std::system_category()), "error when reading the config file"));
	}
	return (read_ret);
}

static void close_throw_if_fail(int fd)
{
	if (close(fd) == -1)
	{
		throw(
			std::system_error(std::error_code(errno, std::system_category()), "system error when closing config file"));
	}
}

static void insert_token_if_needed(e_token_type &token_type, std::vector<t_c_token> &tokens, std::string &current_token,
								   size_t &beginning_column, size_t &beginning_row, size_t &column, size_t &row)
{
	if (token_type != e_token_type::none)
	{
		if (token_type == e_token_type::quoted)
		{
			current_token[0] = '\"';
		}
		tokens.push_back(t_c_token(t_c_position(beginning_column, beginning_row), current_token));
		current_token.erase();
	}
	token_type = e_token_type::none;
	beginning_column = column;
	beginning_row = row;
}

static void remove_comments_and_normalize(std::vector<t_c_token> &tokens, char const *config_file)
{
	size_t i;
	size_t j;

	i = 0;
	while (i < tokens.size())
	{
		if (tokens[i].get_token()[0] == '#')
		{
			tokens.erase(tokens.begin() + static_cast<std::vector<t_c_token>::difference_type>(i));
			continue;
		}
		j = 0;
		while (j < tokens[i].get_token().size())
		{
			if (tokens[i].get_token()[j] == '\\')
			{
				if ((j == tokens[i].get_token().size()) ||
					(tokens[i].get_token()[j] != '\\' && tokens[i].get_token()[j] != '\"'))
				{
					throw(std::invalid_argument(
						std::string(config_file) + ":" + to_string(tokens[i].get_position().get_row()) + ":" +
						to_string(tokens[i].get_position().get_colum() + j) +
						": error: \\ munst be always followed by \\ of \" or preceded by a \\ that is not itself"
						"preceded by a \\ recursively\n"));
				}
				tokens[i].get_token_non_const().erase(j, 1);
			}
			else
			{
				j++;
			}
		}
		i++;
	}
}

std::vector<t_c_token> get_tokens(char const *config_file)
{
	std::vector<t_c_token> tokens;
	std::string            current_token;
	const int              read_buffer_size = 1000; // must be atleast 2
	char                   read_buffer[read_buffer_size + 1];
	int                    config_file_fd = open_throw_if_fail(config_file, O_RDONLY);
	ssize_t                read_ret;
	size_t                 column;
	size_t                 row;
	size_t                 beginning_column;
	size_t                 beginning_row;
	size_t                 token_end;
	e_token_type           token_type;

	read_ret = read_throw_if_fail(config_file_fd, read_buffer, read_buffer_size);
	read_buffer[read_ret] = '\0';
	column = 1;
	row = 1;
	beginning_column = column;
	beginning_row = row;
	token_type = e_token_type::none;
	while (read_ret != 0)
	{
		if (token_type == e_token_type::none)
		{
			token_type = get_token_type(read_buffer);
		}
		token_end = find_token_end(read_buffer, token_type, column, row);
		if (token_type != e_token_type::none)
		{
			current_token.append(&read_buffer[0], token_end);
		}
		memmove(read_buffer, read_buffer + token_end, read_buffer_size - std::max(token_end, 1UL) + 1);
		read_ret -= static_cast<ssize_t>(token_end);
		if (read_buffer[0] == '\0') // same as read_ret == 0
		{
			if (token_type == e_token_type::separator ||
				(token_type == e_token_type::quoted && current_token[current_token.size() - 1] == '\"'))
			{
				insert_token_if_needed(token_type, tokens, current_token, beginning_column, beginning_row, column, row);
			}
			read_ret = read_throw_if_fail(config_file_fd, read_buffer, read_buffer_size);
			read_buffer[read_ret] = '\0';
		}
		else
		{
			insert_token_if_needed(token_type, tokens, current_token, beginning_column, beginning_row, column, row);
		}
	}
	if (current_token.empty() == false && token_type != e_token_type::none)
	{
		tokens.push_back(t_c_token(t_c_position(beginning_column, beginning_row), current_token));
		current_token.erase();
	}
	close_throw_if_fail(config_file_fd);
	remove_comments_and_normalize(tokens, config_file);
	return (tokens);
}

#pragma GCC diagnostic pop
