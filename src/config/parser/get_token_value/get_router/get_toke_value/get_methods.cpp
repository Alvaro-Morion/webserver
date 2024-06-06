/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Filename: get_methods.cpp                                                */
/*   Author:   Peru Riezu <riezumunozperu@gmail.com>                          */
/*   github:   https://github.com/priezu-m                                    */
/*   Licence:  GPLv3                                                          */
/*   Created:  2024/06/03 12:55:56                                            */
/*   Updated:  2024/06/04 19:05:55                                            */
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

static void get_method(t_c_resource_constructor_params &params, std::vector<t_c_token> &tokens, size_t &i,
					   char const *config_file)
{
	if (tokens[i].get_token() == "GET")
	{
		if (params.get_allowed == true)
		{
			throw(std::invalid_argument(std::string(config_file) + ":" + tokens[i].get_position().to_string() +
										": error: GET method listed more than once\n"));
		}
		params.get_allowed = true;
		return;
	}
	if (tokens[i].get_token() == "POST")
	{
		if (params.post_allowed == true)
		{
			throw(std::invalid_argument(std::string(config_file) + ":" + tokens[i].get_position().to_string() +
										": error: POST method listed more than onece\n"));
		}
		params.post_allowed = true;
		return;
	}
	if (tokens[i].get_token() == "DELETE")
	{
		if (params.delet_allowed == true)
		{
			throw(std::invalid_argument(std::string(config_file) + ":" + tokens[i].get_position().to_string() +
										": error: DELETE method listed more than onece\n"));
		}
		params.delet_allowed = true;
		return;
	}
	throw(std::invalid_argument(std::string(config_file) + ":" + tokens[i].get_position().to_string() +
								": error: error unrecognized method  " + tokens[i].get_token() +
								", only recognized methods are GET, POST, and DELETE\n"));
}

void get_methods(t_c_resource_constructor_params &params, std::vector<t_c_token> &tokens, size_t &i,
				 char const *config_file)
{
	t_c_position const position = tokens[i].get_position();
	bool               comma_found;
	bool               first_iteraition;

	if (params.methods_position.is_valid())
	{
		throw(std::invalid_argument(std::string(config_file) + ": " + position.to_string() +
									" : error: redefinition of methods attribute previusly defined at: " +
									params.methods_position.to_string() + '\n'));
	}
	i++;
	params.methods_position = position;
	comma_found = false;
	first_iteraition = true;
	while (i < tokens.size() && tokens[i].get_token()[0] != ';')
	{
		if (first_iteraition == false && comma_found == false)
		{
			if (tokens[i].get_token() != ",")
			{
				throw(std::invalid_argument(std::string(config_file) + ": " + tokens[i].get_position().to_string() +
											": error: expected a comma, found: " + tokens[i].get_token() + '\n'));
			}
			comma_found = true;
			i++;
			continue;
		}
		get_method(params, tokens, i, config_file);
		first_iteraition = false;
		comma_found = false;
		i++;
	}
	if (i == tokens.size())
	{
		throw(std::invalid_argument(std::string(config_file) +
									": error, expected a semicolon, to end the methods attribute at " +
									position.to_string() + ", but found end of file\n"));
	}
}

#pragma GCC diagnostic pop
