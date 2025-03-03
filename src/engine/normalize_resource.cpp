/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Filename: normalize_resource.cpp                                         */
/*   Author:   Peru Riezu <riezumunozperu@gmail.com>                          */
/*   github:   https://github.com/priezu-m                                    */
/*   Licence:  GPLv3                                                          */
/*   Created:  2024/06/12 16:18:19                                            */
/*   Updated:  2024/07/21 00:40:32                                            */
/*                                                                            */
/* ************************************************************************** */

#include "engine.hpp"
#include <cstddef>

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

static std::vector<std::string> split(std::string const &s)
{
	size_t                   pos_start = 0;
	size_t                   pos_end;
	std::string              token;
	std::vector<std::string> res;

	pos_end = s.find('/', pos_start);
	while (pos_end != std::string::npos)
	{
		token = s.substr(pos_start, pos_end - pos_start);
		pos_start = pos_end + 1;
		if (token.empty() == false)
		{
			res.push_back(token);
		}
		pos_end = s.find('/', pos_start);
	}
	if (s.substr(pos_start).empty() == false)
	{
		res.push_back(s.substr(pos_start));
	}
	return res;
}

static int_fast8_t get_hex_char_value(char c)
{
	if (c >= 'a' && c <= 'f')
	{
		return (static_cast<int_fast8_t>(c - 'a' + 10));
	}
	if (c >= 'A' && c <= 'F')
	{
		return (static_cast<int_fast8_t>(c - 'A' + 10));
	}
	if (c >= '0')
	{
		return (static_cast<int_fast8_t>(c - '0'));
	}
	return (-1);
}

static void decode_uri(std::string &uri)
{
	size_t i;

	i = 0;
	while (i < uri.size())
	{
		if (uri[i] == '%')
		{
			if (i + 2 < uri.size())
			{
				int_fast8_t c1 = static_cast<int_fast8_t>(get_hex_char_value(uri[i + 1]) * 16);
				int_fast8_t c2 = get_hex_char_value(uri[i + 2]);

				if (c1 == -1 || c2 == -1)
				{
					uri = "";
					return;
				}
				uri.erase(i, 3);
				uri.insert(i, 1, static_cast<char>(c1 + c2));
			}
			else
			{
				uri = "";
				return;
			}
		}
		i++;
	}
}

std::string normalize_resource(std::string resource)
{
	std::vector<std::string> tokens;
	size_t                   i;
	bool const               ends_in_bar = (resource[resource.size() - 1] == '/');

	if (resource.find('#') != std::string::npos ||
		resource.find_first_not_of(
			"%:/?#[]@!$&'()*+,;=-._~abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789") !=
			std::string::npos)
	{
		resource = "";
		return (resource);
	}
	tokens = split(resource);
	i = 0;
	while (i < tokens.size())
	{
		if (tokens[i] == ".")
		{
			tokens.erase(tokens.begin() + static_cast<std::vector<std::string>::difference_type>(i));
			continue;
		}
		if (tokens[i] == "..")
		{
			if (i == 0)
			{
				resource = "";
				return (resource);
			}
			tokens.erase(tokens.begin() + static_cast<std::vector<std::string>::difference_type>(i));
			i--;
			tokens.erase(tokens.begin() + static_cast<std::vector<std::string>::difference_type>(i));
			continue;
		}
		i++;
	}
	resource = "/";
//	for (std::string const &s : tokens)
	for (size_t j = 0; j < tokens.size(); j++)
	{
		resource += tokens[j];
		resource += "/";
	}
	if (ends_in_bar == false)
	{
		resource.erase(resource.end() - 1);
	}
	i = resource.find('?');
	if (i != std::string::npos)
	{
		resource[i] = -'?';
	}
	decode_uri(resource);
	return (resource);
}

#pragma GCC diagnostic pop
