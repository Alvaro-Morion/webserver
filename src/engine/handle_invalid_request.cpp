/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Filename: handle_invalid_request.cpp                                     */
/*   Author:   Peru Riezu <riezumunozperu@gmail.com>                          */
/*   github:   https://github.com/priezu-m                                    */
/*   Licence:  GPLv3                                                          */
/*   Created:  2024/06/07 11:53:58                                            */
/*   Updated:  2024/06/07 13:34:58                                            */
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

ReturnType handle_invalid_request(void)
{
	int const fd = open("default_error_pages/400", O_RDONLY);

	return (ReturnType(fd, NO_CHILD));
}

#pragma GCC diagnostic pop
