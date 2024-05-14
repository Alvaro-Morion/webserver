/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Filename: main.cpp                                                       */
/*   Author:   Peru Riezu <riezumunozperu@gmail.com>                          */
/*   github:   https://github.com/priezu-m                                    */
/*   Licence:  GPLv3                                                          */
/*   Created:  2024/05/06 19:18:55                                            */
/*   Updated:  2024/05/14 03:52:20                                            */
/*                                                                            */
/* ************************************************************************** */

#include "config/config.hpp"
#include <cstdint>
#include <cstdlib>
#include <iostream>
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

static void print_usage(void)
{
	char usage[] = "Usage:\n"
				   "\t./webserver CONFIGURATION_FILE\n";

	std::cout << usage;
}

int main(int argc, char **argv)
{
	t_c_global_config *config;

	if (argc != 2)
	{
		print_usage();
		return (EXIT_FAILURE);
	}
	config = get_config(argv[1]);
	return (EXIT_FAILURE);
}

#pragma GCC diagnostic pop
