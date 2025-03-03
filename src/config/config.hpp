/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Filename: config.hpp                                                     */
/*   Author:   Peru Riezu <riezumunozperu@gmail.com>                          */
/*   github:   https://github.com/priezu-m                                    */
/*   Licence:  GPLv3                                                          */
/*   Created:  2024/05/06 17:32:04                                            */
/*   Updated:  2024/07/20 23:52:21                                            */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "parser/parser.hpp"
#include "t_c_error_pages/t_c_error_pages.hpp"
#include "t_c_global_config/t_c_global_config.hpp"
#include "t_c_individual_server_config/t_c_individual_server_config.hpp"
#include "t_c_resource/t_c_resource.hpp"
#include "t_c_route/t_c_route.hpp"
#include "t_c_router/t_c_router.hpp"
#include "t_c_server_config/t_c_server_config.hpp"
#include <cstdint>

std::string to_string(uintmax_t n);
std::string to_string(intmax_t n);
std::string to_string(uint16_t n);
