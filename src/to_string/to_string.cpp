/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Filename: to_string.cpp                                                  */
/*   Author:   Peru Riezu <riezumunozperu@gmail.com>                          */
/*   github:   https://github.com/priezu-m                                    */
/*   Licence:  GPLv3                                                          */
/*   Created:  2024/07/20 23:59:36                                            */
/*   Updated:  2024/07/21 00:18:03                                            */
/*                                                                            */
/* ************************************************************************** */

#include <cstdint>
#include <string>
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

static uintmax_t get_magnitude(uintmax_t n)
{
	uintmax_t m = 1;

	while (n > 9)
	{
		n /= 10;
		m *= 10;
	}
	return (m);
}

std::string to_string(uintmax_t n);
std::string to_string(uintmax_t n)
{
	std::string res;
	uintmax_t   magnitude = get_magnitude(n);

	while (magnitude != 0)
	{
		res += static_cast<char>(((n / magnitude) % 10) + '0');
		magnitude /= 10;
	}
	return (res);
}

std::string to_string(intmax_t n);
std::string to_string(intmax_t n)
{
	std::string res;

	if (n < 0)
	{
		return ('-' + to_string(static_cast<uintmax_t>(-n)));
	}
	return (to_string(static_cast<uintmax_t>(n)));
}

std::string to_string(uint16_t n);
std::string to_string(uint16_t n)
{
	return (to_string(static_cast<uintmax_t>(n)));
}

#pragma GCC diagnostic pop
