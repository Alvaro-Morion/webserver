/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Filename: get_file_is_a_directory_error_page.cpp                         */
/*   Author:   Peru Riezu <riezumunozperu@gmail.com>                          */
/*   github:   https://github.com/priezu-m                                    */
/*   Licence:  GPLv3                                                          */
/*   Created:  2024/06/04 12:18:16                                            */
/*   Updated:  2024/06/06 17:59:30                                            */
/*                                                                            */
/* ************************************************************************** */

#include "../get_router.hpp"
#include <fcntl.h>
#include <stdexcept>
#include <unistd.h>

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

void get_file_is_a_directory_page(t_c_resource_constructor_params &params, std::vector<t_c_token> &tokens, size_t &i,
								  char const *config_file)
{
	t_c_position const position = tokens[i].get_position();

	i++;
	if (params.file_is_a_directory_page_position.is_valid() == true)
	{
		throw(
			std::invalid_argument(std::string(config_file) + ":" + position.to_string() +
								  ": error: redefinition of file_is_a_directory_page attribute previusly defined at: " +
								  params.file_is_a_directory_page_position.to_string() + '\n'));
	}
	if (i == tokens.size())
	{
		throw(std::invalid_argument(std::string(config_file) +
									": error, expected a value for file_is_a_directory_page atribute at " +
									position.to_string() + ", but found end of file\n"));
	}
	remove_quotes_if_present(tokens, i, config_file);
	params.file_is_a_directory_page = tokens[i].get_token();
	if (access(tokens[i].get_token().c_str(), R_OK) == -1)
	{
		throw(std::invalid_argument(std::string(config_file) + ":" + position.to_string() +
									": error: file is does not exist or is not readable\n"));
	}
	params.file_is_a_directory_page_position = position;
	i++;
	if (i == tokens.size())
	{
		throw(std::invalid_argument(std::string(config_file) +
									": error, expected a semicolon, to end the file_is_a_directory_page"
									" attribute at " +
									position.to_string() + ", but found end of file\n"));
	}
	if (tokens[i].get_token()[0] != ';')
	{
		throw(std::invalid_argument(std::string(config_file) + ":" + tokens[i].get_position().to_string() +
									": error, expected a semicolon, to end the file_is_a_directory_page attribute at " +
									position.to_string() + ", but found: " + tokens[i].get_token() + '\n'));
	}
}

#pragma GCC diagnostic pop
