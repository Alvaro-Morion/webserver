/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Filename: main.cpp                                                       */
/*   Author:   Peru Riezu <riezumunozperu@gmail.com>                          */
/*   github:   https://github.com/priezu-m                                    */
/*   Licence:  GPLv3                                                          */
/*   Created:  2024/05/06 19:18:55                                            */
/*   Updated:  2024/06/07 16:44:37                                            */
/*                                                                            */
/* ************************************************************************** */

#include "config/config.hpp"
#include "engine/engine.hpp"
#include "io/io.hpp"
#include <csignal>
#include <cstdint>
#include <cstdlib>
#include <iostream>
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
	// t_c_global_config config; //For test

	t_c_global_config *config;

	if (argc > 2)
	{
		print_usage();
		return (EXIT_FAILURE);
	}
	if (argc == 2)
	{
		config = get_config(argv[1]);
	}
	else
	{
		config = get_config(DEFAULT_CONFIG);
	}
	std::cout << config->to_string();
//	pause();
	signal(SIGPIPE, SIG_IGN);
	handle_request("GET /hola HTTP/1.1\r\n\r\n", *config->get_servers().begin());

//	Server server(config);
	// Server server(&config); //For test
	return (EXIT_FAILURE);
}

#pragma GCC diagnostic pop
