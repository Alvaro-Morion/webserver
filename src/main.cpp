/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Filename: main.cpp                                                       */
/*   Author:   Peru Riezu <riezumunozperu@gmail.com>                          */
/*   github:   https://github.com/priezu-m                                    */
/*   Licence:  GPLv3                                                          */
/*   Created:  2024/05/06 19:18:55                                            */
/*   Updated:  2024/07/20 17:17:20                                            */
/*                                                                            */
/* ************************************************************************** */

#include "config/config.hpp"
#include "engine/engine.hpp"
#include "io/io.hpp"
#include <csignal>
#include <cstdlib>
#include <iostream>
#include <stdint.h>
#include <string>
#include <unistd.h>
#include <vector>

#define DEFAULT_CONFIG "example_configs/config1"

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
				   "\t./webserver [CONFIGURATION_FILE]\n";

	std::cout << usage;
}

int main(int argc, char **argv)
{
	t_c_global_config *config; // NOLINT

	if (argc > 2)
	{
		print_usage();
		return (EXIT_FAILURE);
	}
	try
	{
		if (argc == 2)
		{
			config = get_config(argv[1]);
		}
		else
		{
			config = get_config(DEFAULT_CONFIG);
		}
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		_exit(EXIT_FAILURE);
	}
	signal(SIGPIPE, SIG_IGN);
	std::cout << config->to_string();
	Server server(config);
	return (EXIT_FAILURE);
}

#pragma GCC diagnostic pop
