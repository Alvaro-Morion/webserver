/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Filename: parser.hpp                                                     */
/*   Author:   Peru Riezu <riezumunozperu@gmail.com>                          */
/*   github:   https://github.com/priezu-m                                    */
/*   Licence:  GPLv3                                                          */
/*   Created:  2024/05/14 04:05:21                                            */
/*   Updated:  2024/05/15 06:21:50                                            */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "get_elem/get_elem.hpp"
#include "t_c_server_config_params.hpp"

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

t_c_global_config     *get_config(char *path);
t_c_server_config      parse_server_config(std::string &server_config, size_t &line, size_t &colum);
void                   skip_spaces(std::string const &server_config, size_t &i, size_t &line, size_t &colum);
bool                   is_double_quote(std::string &content, size_t i);
void                   resolve_scape_sequences(std::string &str, size_t line, size_t colum);
std::string::iterator  find_double_quote(std::string &str, size_t i);

#pragma GCC diagnostic pop
