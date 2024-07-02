/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Filename: t_c_error_pages.cpp                                            */
/*   Author:   Peru Riezu <riezumunozperu@gmail.com>                          */
/*   github:   https://github.com/priezu-m                                    */
/*   Licence:  GPLv3                                                          */
/*   Created:  2024/05/07 17:41:28                                            */
/*   Updated:  2024/07/02 19:33:02                                            */
/*                                                                            */
/* ************************************************************************** */

#include "../config.hpp"
#include <cstdio>
#include <stdexcept>
#include <string>
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

t_c_error_pages::t_c_error_pages(std::string const &http_version_not_supported_param,
								 std::string const &not_implemeted_param,
								 std::string const &internal_server_error_param, std::string const &uri_too_long_param,
								 std::string const &content_too_large_param, std::string const &length_requiered_param,
								 std::string const &request_timeout_param, std::string const &not_found_param,
								 std::string const &forbidden_param, std::string const &bad_request_param)
	: http_version_not_supported(http_version_not_supported_param), not_implemeted(not_implemeted_param),
	  internal_server_error(internal_server_error_param), uri_too_long(uri_too_long_param),
	  content_too_large(content_too_large_param), length_requiered(length_requiered_param),
	  request_timeout(request_timeout_param), not_found(not_found_param), forbidden(forbidden_param),
	  bad_request(bad_request_param)
{
	if (access(http_version_not_supported.c_str(), R_OK) == -1)
	{
		throw(std::invalid_argument(http_version_not_supported + " does not exist or cannot be read"));
	}
	if (access(not_implemeted.c_str(), R_OK) == -1)
	{
		throw(std::invalid_argument(not_implemeted + " does not exist or cannot be read"));
	}
	if (access(uri_too_long.c_str(), R_OK) == -1)
	{
		throw(std::invalid_argument(uri_too_long + " does not exist or cannot be read"));
	}
	if (access(content_too_large.c_str(), R_OK) == -1)
	{
		throw(std::invalid_argument(content_too_large + " does not exist or cannot be read"));
	}
	if (access(length_requiered.c_str(), R_OK) == -1)
	{
		throw(std::invalid_argument(length_requiered + " does not exist or cannot be read"));
	}
	if (access(request_timeout.c_str(), R_OK) == -1)
	{
		throw(std::invalid_argument(request_timeout + " does not exist or cannot be read"));
	}
	if (access(not_found.c_str(), R_OK) == -1)
	{
		throw(std::invalid_argument(not_found + " does not exist or cannot be read"));
	}
	if (access(forbidden.c_str(), R_OK) == -1)
	{
		throw(std::invalid_argument(forbidden + " does not exist or cannot be read"));
	}
	if (access(bad_request.c_str(), R_OK) == -1)
	{
		throw(std::invalid_argument(bad_request + " does not exist or cannot be read"));
	}
}

t_c_error_pages::t_c_error_pages(t_c_error_pages const &copy)
	: http_version_not_supported(copy.http_version_not_supported), not_implemeted(copy.not_implemeted),
	  uri_too_long(copy.uri_too_long), content_too_large(copy.content_too_large),
	  length_requiered(copy.length_requiered), request_timeout(copy.request_timeout), not_found(copy.not_found),
	  forbidden(copy.forbidden), bad_request(copy.bad_request)
{
}

t_c_error_pages::t_c_error_pages(void):
		http_version_not_supported("default_error_pages/505"),
		not_implemeted("default_error_pages/501"),
		internal_server_error("default_error_pages/500"),
		uri_too_long("default_error_pages/414"),
		content_too_large("default_error_pages/413"),
		length_requiered("default_error_pages/411"),
		request_timeout("default_error_pages/408"),
		not_found("default_error_pages/404"),
		forbidden("default_error_pages/403"),
		bad_request("default_error_pages/400")
{
	if (access(http_version_not_supported.c_str(), R_OK) == -1)
	{
		throw(std::invalid_argument(http_version_not_supported + " does not exist or cannot be read"));
	}
	if (access(not_implemeted.c_str(), R_OK) == -1)
	{
		throw(std::invalid_argument(not_implemeted + " does not exist or cannot be read"));
	}
	if (access(uri_too_long.c_str(), R_OK) == -1)
	{
		throw(std::invalid_argument(uri_too_long + " does not exist or cannot be read"));
	}
	if (access(content_too_large.c_str(), R_OK) == -1)
	{
		throw(std::invalid_argument(content_too_large + " does not exist or cannot be read"));
	}
	if (access(length_requiered.c_str(), R_OK) == -1)
	{
		throw(std::invalid_argument(length_requiered + " does not exist or cannot be read"));
	}
	if (access(request_timeout.c_str(), R_OK) == -1)
	{
		throw(std::invalid_argument(request_timeout + " does not exist or cannot be read"));
	}
	if (access(not_found.c_str(), R_OK) == -1)
	{
		throw(std::invalid_argument(not_found + " does not exist or cannot be read"));
	}
	if (access(forbidden.c_str(), R_OK) == -1)
	{
		throw(std::invalid_argument(forbidden + " does not exist or cannot be read"));
	}
	if (access(bad_request.c_str(), R_OK) == -1)
	{
		throw(std::invalid_argument(bad_request + " does not exist or cannot be read"));
	}
}

bool t_c_error_pages::operator==(t_c_error_pages const &comparator) const
{
	if (this == &comparator)
	{
		return (true);
	}
	if (http_version_not_supported != comparator.get_http_version_not_supported() ||
		not_implemeted != comparator.get_not_implemeted() || uri_too_long != comparator.get_uri_too_long() ||
		content_too_large != comparator.get_content_too_large() ||
		length_requiered != comparator.get_length_requiered() || request_timeout != comparator.get_request_timeout() ||
		not_found != comparator.get_not_found() || forbidden != comparator.get_forbidden() ||
		bad_request != comparator.get_bad_request())
	{
		return (false);
	}
	return (true);
}

t_c_error_pages::~t_c_error_pages(void)
{
}

std::string const &t_c_error_pages::get_http_version_not_supported(void) const
{
	return (http_version_not_supported);
}

std::string const &t_c_error_pages::get_not_implemeted(void) const
{
	return (not_implemeted);
}

std::string const &t_c_error_pages::get_internal_server_error(void) const
{
	return (internal_server_error);
}

std::string const &t_c_error_pages::get_uri_too_long(void) const
{
	return (uri_too_long);
}

std::string const &t_c_error_pages::get_content_too_large(void) const
{
	return (content_too_large);
}

std::string const &t_c_error_pages::get_length_requiered(void) const
{
	return (length_requiered);
}

std::string const &t_c_error_pages::get_request_timeout(void) const
{
	return (request_timeout);
}

std::string const &t_c_error_pages::get_not_found(void) const
{
	return (not_found);
}

std::string const &t_c_error_pages::get_forbidden(void) const
{
	return (forbidden);
}

std::string const &t_c_error_pages::get_bad_request(void) const
{
	return (bad_request);
}

std::string t_c_error_pages::to_string(void) const
{
	return ("\t\thttp version not supported: " + http_version_not_supported + "\n\t\tnot implemeted: " +
			not_implemeted + "\n\t\turi too long: " + uri_too_long + "\n\t\tcontent too large: " + content_too_large +
			"\n\t\tlength requiered: " + length_requiered + "\n\t\trequest timeout: " + request_timeout +
			"\n\t\tnot found: " + not_found + "\n\t\tforbidden: " + forbidden + "\n\t\tbad request: " + bad_request);
}

#pragma GCC diagnostic pop
