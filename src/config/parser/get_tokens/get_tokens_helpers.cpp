/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Filename: get_tokens_helpers.cpp                                         */
/*   Author:   Peru Riezu <riezumunozperu@gmail.com>                          */
/*   github:   https://github.com/priezu-m                                    */
/*   Licence:  GPLv3                                                          */
/*   Created:  2024/05/27 08:02:34                                            */
/*   Updated:  2024/06/06 16:00:13                                            */
/*                                                                            */
/* ************************************************************************** */

#include "get_tokens.hpp"
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

size_t find_char_after_unscaped_quotation(char const *str, size_t &colum, size_t &row)
{
	size_t res;
	size_t back_slash_count;
	size_t i;

	res = 0;
	while (str[res] != '\0')
	{
		if (str[res] == '\"')
		{
			if (res == 0)
			{
				return (res + 1);
			}
			back_slash_count = 0;
			i = res;
			while (i > 0)
			{
				if (str[i - 1] != '\\')
				{
					break;
				}
				back_slash_count++;
				i--;
			}
			if (back_slash_count % 2 == 1 || back_slash_count == 0)
			{
				colum++;
				return (res + 1);
			}
		}
		if (str[res] == '\t')
		{
			colum += 4;
		}
		else if (str[res] == '\n')
		{
			row++;
			colum = 1;
		}
		else
		{
			colum++;
		}
		res++;
	}
	return (res);
}

size_t find_nl(char const *str, size_t &colum, __attribute((unused)) size_t &row)
{
	size_t res;

	res = 0;
	while (str[res] != '\0')
	{
		if (str[res] == ('\n'))
		{
			return (res);
		}
		if (str[res] == '\t')
		{
			colum += 4;
		}
		else
		{
			colum++;
		}
		res++;
	}
	return (res);
}

size_t find_space_unscaped_quote_pound_or_separator(char const *str, size_t &colum, size_t &row)
{
	size_t const original_colum = colum;
	size_t const original_row = row;
	size_t const unscaped_quote_pos = find_char_after_unscaped_quotation(str, colum, row);
	size_t       res;
	size_t       back_slash_count;
	size_t       i;

	colum = original_colum;
	row = original_row;
	res = 0;
	while (res < unscaped_quote_pos)
	{
		if (std::isspace(str[res]) != 0)
		{
			return (res);
		}
		if (str[res] == '{' || str[res] == '}' || str[res] == ',' || str[res] == ';' || str[res] == '#')
		{
			back_slash_count = 0;
			i = res;
			while (i > 0)
			{
				if (str[i - 1] != '\\')
				{
					break;
				}
				back_slash_count++;
				i--;
			}
			if (back_slash_count % 2 == 1)
			{
				return (res);
			}
			return (res);
		}
		colum++;
		res++;
	}
	if (unscaped_quote_pos > 0 && str[unscaped_quote_pos - 1] == '\"')
	{
		return (unscaped_quote_pos - 1);
	}
	return (unscaped_quote_pos);
}

size_t find_first_non_space(char const *str, size_t &colum, size_t &row)
{
	size_t res;

	res = 0;
	while (str[res] != '\0')
	{
		if (std::isspace(str[res]) == 0)
		{
			return (res);
		}
		if (str[res] == '\t')
		{
			colum += 4;
		}
		else if (str[res] == '\n')
		{
			row++;
			colum = 1;
		}
		else
		{
			colum++;
		}
		res++;
	}
	return (res);
}

size_t find_token_end(char const *str, e_token_type token_type, size_t &colum, size_t &row)
{
	switch (token_type) // NOLINT
	{
		case e_token_type::quoted:
		{
			return (find_char_after_unscaped_quotation(str, colum, row));
		}
		case e_token_type::comment:
		{
			return (find_nl(str, colum, row));
		}
		case e_token_type::normal:
		{
			return (find_space_unscaped_quote_pound_or_separator(str, colum, row));
		}
		case e_token_type::none:
		{
			return (find_first_non_space(str, colum, row));
		}
		case e_token_type::separator:
		{
			colum++;
			return (1);
		}
	}
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
}

#pragma GCC diagnostic pop

e_token_type get_token_type(char *buffer)
{
	if (buffer[0] == '#')
	{
		return (e_token_type::comment);
	}
	if (buffer[0] == '\"')
	{
		buffer[0] = 'a'; // any non null character
		return (e_token_type::quoted);
	}
	if (std::isspace(buffer[0]) != 0)
	{
		return (e_token_type::none);
	}
	if (buffer[0] == '}' || buffer[0] == '{' || buffer[0] == ',' || buffer[0] == ';')
	{
		return (e_token_type::separator);
	}
	return (e_token_type::normal);
}

#pragma GCC diagnostic pop
