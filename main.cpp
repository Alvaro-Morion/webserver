/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Filename: main.cpp                                                       */
/*   Author:   Peru Riezu <riezumunozperu@gmail.com>                          */
/*   github:   https://github.com/priezu-m                                    */
/*   Licence:  GPLv3                                                          */
/*   Created:  2024/05/06 19:18:55                                            */
/*   Updated:  2024/05/09 04:24:54                                            */
/*                                                                            */
/* ************************************************************************** */

#include "config.hpp"
#include <cstdint>
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

int main(void)
{

	std::vector<std::string>  paths1{std::string("")};
	std::vector<std::string>  paths2{std::string("")};
	std::vector<t_c_resource> resources1{t_c_resource("/", "", true, true, true, true)};
	std::vector<t_c_resource> resources2{t_c_resource("/", "", true, true, true, true)};
	t_c_router                router1(paths1, resources1);
	t_c_router                router2(paths2, resources2);
	std::string               file_is_a_directory_page1;
	std::string               file_is_a_directory_page2;
	t_c_default_error_pages   error_pages1;
	t_c_default_error_pages   error_pages2;
	std::vector<std::string>  host_names1{"test.com", "2.test.com", "test.com"};
	std::vector<std::string>  host_names2{"new.com"};
	std::vector<uint16_t>     ports1{
        89,
        69,
    };
	std::vector<uint16_t> ports2{
		1,
		89,
	};
	t_c_server_config server_config1(host_names1, ports1, &router1, &error_pages1, &file_is_a_directory_page1,
									 UINT64_MAX);
	t_c_server_config server_config2(host_names1, ports1, &router1, &error_pages1, &file_is_a_directory_page1,
									 UINT64_MAX);
	std::vector<t_c_server_config> server_configs({server_config1, server_config2});
	t_c_global_config              config(server_configs);

	std::cout << config.to_string();
}

#pragma GCC diagnostic pop
