/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Filename: get_root.cpp                                                   */
/*   Author:   Peru Riezu <riezumunozperu@gmail.com>                          */
/*   github:   https://github.com/priezu-m                                    */
/*   Licence:  GPLv3                                                          */
/*   Created:  2024/06/02 08:36:42                                            */
/*   Updated:  2024/06/06 17:57:36                                            */
/*                                                                            */
/* ************************************************************************** */

#include "../get_router.hpp"
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

void get_root(t_c_resource_constructor_params &params, std::vector<t_c_token> &tokens, size_t &i,
			  char const *config_file)
{
	t_c_position const position = tokens[i].get_position();

	i++;
	if (params.root_position.is_valid() == true)
	{
		throw(std::invalid_argument(std::string(config_file) + ":" + position.to_string() +
									" : error: redefinition of root attribute previusly defined at: " +
									params.root_position.to_string() + '\n'));
	}
	if (i == tokens.size())
	{
		throw(std::invalid_argument(std::string(config_file) + ": error, expected a value for root atribute at " +
									position.to_string() + ", but found end of file\n"));
	}
	remove_quotes_if_present(tokens, i, config_file);
	params.root = tokens[i].get_token();
	params.root_position = position;
	i++;
	if (i == tokens.size())
	{
		throw(std::invalid_argument(std::string(config_file) +
									": error, expected a semicolon, to end the root"
									" attribute at " +
									position.to_string() + ", but found end of file\n"));
	}
	if (tokens[i].get_token()[0] != ';')
	{
		throw(std::invalid_argument(std::string(config_file) + ":" + tokens[i].get_position().to_string() +
									": error, expected a semicolon, to end the root attribute at " +
									position.to_string() + ", but found: " + tokens[i].get_token() + '\n'));
	}
}

#pragma GCC diagnostic pop
