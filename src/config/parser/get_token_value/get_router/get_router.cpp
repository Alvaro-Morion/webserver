/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Filename: get_router.cpp                                                 */
/*   Author:   Peru Riezu <riezumunozperu@gmail.com>                          */
/*   github:   https://github.com/priezu-m                                    */
/*   Licence:  GPLv3                                                          */
/*   Created:  2024/05/30 20:51:43                                            */
/*   Updated:  2024/05/30 23:23:11                                            */
/*                                                                            */
/* ************************************************************************** */

#include "get_router.hpp"
#include <iostream>

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

void get_router(t_c_server_constructor_params &params, std::vector<t_c_token> const &tokens, size_t &i,
				char const *config_file, int &error_count)
{
	t_c_position const position = tokens[i].get_position();
	int const          original_error_count = error_count;

	i++;
	if (i == tokens.size())
	{
		std::cout << std::string(config_file) +
						 ": error, expected a semicolon, to end the client_body_size_limit"
						 " attribute at " +
						 position.to_string() + ", but found end of file\n";
		error_count++;
		return;
	}
	if (tokens[i].get_token()[0] != '{')
	{
		std::cout << std::string(config_file) + ":" + tokens[i].get_position().to_string() +
						 ": error, expected a semicolon, to end the client_body_size_limit"
						 " attribute at " +
						 position.to_string() + ", but found: " + tokens[i].get_token() + "\n";
		error_count++;
	}
	i++;
	while (i 
	i++;
}

#pragma GCC diagnostic pop
