/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Filename: get_tokens.hpp                                                 */
/*   Author:   Peru Riezu <riezumunozperu@gmail.com>                          */
/*   github:   https://github.com/priezu-m                                    */
/*   Licence:  GPLv3                                                          */
/*   Created:  2024/05/27 08:00:02                                            */
/*   Updated:  2024/07/21 01:02:06                                            */
/*                                                                            */
/* ************************************************************************** */

#pragma once

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

enum class e_token_type
{
	quoted,
	comment,
	normal,
	separator,
	none
};

e_token_type           get_token_type(char *buffer);
size_t                 find_token_end(char const *str, e_token_type token_type, size_t &colum, size_t &row);
size_t                 find_first_non_space(char const *str, size_t &colum, size_t &row);
size_t                 find_space_unscaped_quote_pound_or_separator(char const *str, size_t &colum, size_t &row);
size_t                 find_nl(char const *str, size_t &colum, __attribute((unused)) size_t &row);
size_t                 find_char_after_unscaped_quotation(char const *str, size_t &colum, size_t &row);

#pragma GCC diagnostic pop
