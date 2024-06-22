/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Filename: get_router.cpp                                                 */
/*   Author:   Peru Riezu <riezumunozperu@gmail.com>                          */
/*   github:   https://github.com/priezu-m                                    */
/*   Licence:  GPLv3                                                          */
/*   Created:  2024/06/03 08:33:11                                            */
/*   Updated:  2024/06/14 17:35:59                                            */
/*                                                                            */
/* ************************************************************************** */

#include "get_router.hpp"
#include <cstddef>
#include <functional>
#include <stdexcept>
#include <string>
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

static t_c_resource add_defaults_and_construct_resource(t_c_resource_constructor_params &params,
														t_c_position const &position, char const *config_file)
{
	if ((params.is_cgi_position.is_valid() == true && params.is_redirect_position.is_valid() == true) &&
		(params.is_cgi == true && params.is_redirect == true))
	{
		throw(std::invalid_argument(std::string(config_file) + ": error: in resource defined at: " +
									position.to_string() + ": is_cgi and is_redirect set to true sinmultaniusly\n"));
	}
	if (params.directory_listing == true && params.file_is_a_directory_page_position.is_valid() == true)
	{
		throw(std::invalid_argument(std::string(config_file) +
									": error: in resource defined at: " + position.to_string() +
									": when is_redirect is set to true file_is_a_directory_page is never used\n"));
	}
	if (params.root_position.is_valid() == false)
	{
		throw(std::invalid_argument(std::string(config_file) + ": error: in resource defined at: " +
									position.to_string() + ": no root attribute defined\n"));
	}
	if (params.methods_position.is_valid() == false)
	{
		params.get_allowed = true;
	}
	return (t_c_resource(params.root, params.file_is_a_directory_page, params.is_redirect, params.directory_listing,
						 params.post_allowed, params.delet_allowed, params.get_allowed, params.is_cgi));
}

static void get_token_value(std::vector<t_c_token> &tokens, size_t &i, t_c_resource_constructor_params &params,
							char const *config_file)
{
	if (tokens[i].get_token() == "root")
	{
		get_root(params, tokens, i, config_file);
	}
	else if (tokens[i].get_token() == "is_redirect")
	{
		get_is_redirect(params, tokens, i, config_file);
	}
	else if (tokens[i].get_token() == "methods")
	{
		get_methods(params, tokens, i, config_file);
	}
	else if (tokens[i].get_token() == "directory_listing")
	{
		get_direcory_listing(params, tokens, i, config_file);
	}
	else if (tokens[i].get_token() == "resource_is_a_directory_error_page")
	{
		get_file_is_a_directory_page(params, tokens, i, config_file);
	}
	else if (tokens[i].get_token() == "is_cgi")
	{
		get_is_cgi(params, tokens, i, config_file);
	}
	else
	{
		throw(std::invalid_argument(std::string(config_file) + ":" + tokens[i].get_position().to_string() +
									": error: error unrecognized token  " + tokens[i].get_token() + '\n'));
	}
	if (i < tokens.size())
	{
		i++;
	}
}

static t_c_resource get_resource(std::vector<t_c_token> &tokens, size_t &i, char const *config_file)
{
	t_c_resource_constructor_params params;
	t_c_position                    position;
	t_c_position                    opening_key_position;

	if (i == tokens.size())
	{
		throw(std::invalid_argument(std::string(config_file) + ": error: expected resource but found end of file\n"));
	}
	if (tokens[i].get_token() != "resource")
	{
		throw(std::invalid_argument(std::string(config_file) + ":" + tokens[i].get_position().to_string() +
									": error: expected resource but found: " + tokens[i].get_token() + '\n'));
	}
	position = tokens[i].get_position();
	i++;
	if (i == tokens.size())
	{
		throw(std::invalid_argument(std::string(config_file) +
									": error: expected value for path attribute but found end of file\n"));
	}
	if (tokens[i].get_token()[0] != '{')
	{
		throw(std::invalid_argument(
			std::string(config_file) + ":" + tokens[i].get_position().to_string() +
			": error: expected {, to open resource attribute but found: " + tokens[i].get_token() + '\n'));
	}
	opening_key_position = tokens[i].get_position();
	i++;
	while (i < tokens.size() && tokens[i].get_token()[0] != '}')
	{
		get_token_value(tokens, i, params, config_file);
	}
	if (i == tokens.size())
	{
		throw(std::invalid_argument(std::string(config_file) + ": error, expected }, to match { at " +
									opening_key_position.to_string() + ", but found end of file\n"));
	}
	return (add_defaults_and_construct_resource(params, position, config_file));
}

static std::string get_path(std::vector<t_c_token> &tokens, size_t &i, char const *config_file)
{
	std::string path;

	if (i == tokens.size())
	{
		throw(std::invalid_argument(std::string(config_file) + ": error: expected path but found end of file\n"));
	}
	if (tokens[i].get_token() != "path")
	{
		throw(std::invalid_argument(std::string(config_file) + ":" + tokens[i].get_position().to_string() +
									": error: expected path but found: " + tokens[i].get_token() + '\n'));
	}
	i++;
	if (i == tokens.size())
	{
		throw(std::invalid_argument(std::string(config_file) +
									": error: expected value for path attribute but found end of file\n"));
	}
	remove_quotes_if_present(tokens, i, config_file);
	path = tokens[i].get_token();
	if (path[path.size() - 1] != '/')
	{
		path.append("/");
	}
	i++;
	if (i == tokens.size())
	{
		throw(std::invalid_argument(std::string(config_file) +
									": error: expected ;, to close path attribute but found end of file\n"));
	}
	if (tokens[i].get_token()[0] != ';')
	{
		throw(std::invalid_argument(std::string(config_file) + ":" + tokens[i].get_position().to_string() +
									": error: expected ;, to close path attribute but found: " + tokens[i].get_token() +
									'\n'));
	}
	return (path);
}

static t_c_route_token get_route(std::vector<t_c_token> &tokens, size_t &i, char const *config_file)
{
	t_c_position position;
	t_c_position opening_key_position;
	std::string  path;
	t_c_resource resource;

	if (tokens[i].get_token() != "route")
	{
		throw(std::invalid_argument(std::string(config_file) + ":" + tokens[i].get_position().to_string() +
									": error: expected route, found: " + tokens[i].get_token() + '\n'));
	}
	position = tokens[i].get_position();
	i++;
	if (i == tokens.size())
	{
		throw(std::invalid_argument(std::string(config_file) + ": error: expected {, after route attribute at: " +
									tokens[i - 1].get_position().to_string() + "but found end of file\n"));
	}
	if (tokens[i].get_token()[0] != '{')
	{
		throw(std::invalid_argument(
			std::string(config_file) + ":" + tokens[i].get_position().to_string() +
			": error: expected { after route attribute defined at: " + tokens[i - 1].get_position().to_string() +
			", found: " + tokens[i].get_token() + '\n'));
	}
	opening_key_position = tokens[i].get_position();
	i++;
	path = get_path(tokens, i, config_file);
	i++;
	resource = get_resource(tokens, i, config_file);
	i++;
	if (i == tokens.size())
	{
		throw(std::invalid_argument(std::string(config_file) + ": error, expected }, to match { at " +
									opening_key_position.to_string() + ", but found end of file\n"));
	}
	if (tokens[i].get_token()[0] != '}')
	{

		throw(std::invalid_argument(std::string(config_file) + ": error, expected }, to match { at " +
									opening_key_position.to_string() + ", but found: " + tokens[i].get_token() + '\n'));
	}
	return (t_c_route_token(t_c_route(path, resource), position));
}

void get_router(t_c_server_constructor_params &params, std::vector<t_c_token> &tokens, size_t &i,
				char const *config_file)
{
	t_c_position              opening_key_position;
	std::set<t_c_route_token> routes;
	t_c_route_token           aux;

	i++;
	if (i == tokens.size())
	{
		throw(std::invalid_argument(std::string(config_file) + ": error: expected { after router attribute at: " +
									tokens[i - 1].get_position().to_string() + ", but found end of file\n"));
	}
	opening_key_position = tokens[i].get_position();
	i++;
	while (i < tokens.size() && tokens[i].get_token()[0] != '}')
	{
		aux = get_route(tokens, i, config_file);
		if (routes.insert(aux).second == false)
		{
			throw(std::invalid_argument(std::string(config_file) + ":" + aux.get_position().to_string() +
										": error: route's path was already defined by route at: " +
										routes.insert(aux).first->get_position().to_string() + '\n'));
		}
		if (i < tokens.size())
		{
			i++;
		}
	}
	if (i == tokens.size())
	{
		throw(std::invalid_argument(std::string(config_file) + ": error, expected }, to match { at " +
									opening_key_position.to_string() + ", but found end of file\n"));
	}
	params.router = new t_c_router(std::set<t_c_route, std::less<>>(routes.begin(), routes.end()));
}

#pragma GCC diagnostic pop
