/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Filename: handle_error.cpp                                               */
/*   Author:   Peru Riezu <riezumunozperu@gmail.com>                          */
/*   github:   https://github.com/priezu-m                                    */
/*   Licence:  GPLv3                                                          */
/*   Created:  2024/06/07 13:33:33                                            */
/*   Updated:  2024/06/07 14:34:20                                            */
/*                                                                            */
/* ************************************************************************** */

#include "engine.hpp"
#include <fcntl.h>

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

ReturnType handle_error(int error_code, t_c_individual_server_config const &config)
{
	switch (error_code) // NOLINT
	{
		case 505:
			return (ReturnType(open(config.get_error_pages()->get_http_version_not_supported().c_str(), O_RDONLY), NO_CHILD));
		case 501:
			return (ReturnType(open(config.get_error_pages()->get_not_implemeted().c_str(), O_RDONLY), NO_CHILD));
		case 500:
			return (ReturnType(open(config.get_error_pages()->get_internal_server_error().c_str(), O_RDONLY), NO_CHILD));
		case 414:
			return (ReturnType(open(config.get_error_pages()->get_uri_too_long().c_str(), O_RDONLY), NO_CHILD));
		case 413:
			return (ReturnType(open(config.get_error_pages()->get_content_too_large().c_str(), O_RDONLY), NO_CHILD));
		case 411:
			return (ReturnType(open(config.get_error_pages()->get_length_requiered().c_str(), O_RDONLY), NO_CHILD));
		case 408:
			return (ReturnType(open(config.get_error_pages()->get_request_timeout().c_str(), O_RDONLY), NO_CHILD));
		case 404:
			return (ReturnType(open(config.get_error_pages()->get_not_found().c_str(), O_RDONLY), NO_CHILD));
		case 403:
			return (ReturnType(open(config.get_error_pages()->get_forbidden().c_str(), O_RDONLY), NO_CHILD));
		case 400:
			return (ReturnType(open(config.get_error_pages()->get_bad_request().c_str(), O_RDONLY), NO_CHILD));
	}
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
}
#pragma GCC diagnostic pop

#pragma GCC diagnostic pop
