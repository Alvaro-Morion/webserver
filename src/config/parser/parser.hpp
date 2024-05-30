/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   Filename: parser.hpp                                                     */
/*   Author:   Peru Riezu <riezumunozperu@gmail.com>                          */
/*   github:   https://github.com/priezu-m                                    */
/*   Licence:  GPLv3                                                          */
/*   Created:  2024/05/24 18:43:50                                            */
/*   Updated:  2024/05/30 20:48:34                                            */
/*                                                                            */
/* ************************************************************************** */

#include "../t_c_global_config/t_c_global_config.hpp"
#include <cstddef>
#include <string>

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

class t_c_position
{
	public:
		size_t colum;
		size_t row;

		t_c_position(size_t colum_param, size_t row_param)
			: colum(colum_param), row(row_param)
		{
		}
		t_c_position(t_c_position const &copy)
			: colum(copy.colum), row(copy.row)
		{
		}
		~t_c_position(void)
		{
		}

		t_c_position const &operator=(t_c_position const &copy)
		{
			colum = copy.colum;
			row = copy.row;
			return (*this);
		}

		size_t    get_colum(void) const
		{
			return (colum);
		}
		size_t    get_row(void) const
		{
			return (row);
		}
		void        add_one_to_colum(void)
		{
			colum++;
		}
		std::string to_string(void) const
		{
			return (std::to_string(row) + ":" + std::to_string(colum));
		}
};

class t_c_token
{
	private:
		t_c_position position;
		std::string  token;

	public:
		t_c_token(t_c_position const &position_param, std::string const &token_param)
			: position(position_param), token(token_param)
		{
		}

		t_c_token(t_c_token const &copy) : position(copy.position), token(copy.token)
		{
		}

		~t_c_token(void)
		{
		}

		t_c_token const &operator=(t_c_token const &copy)
		{
			position = copy.position;
			token = copy.token;
			return (*this);
		}

		t_c_position &get_position_non_const(void)
		{
			return (position);
		}
		std::string &get_token_non_const(void)
		{
			return (token);
		}
		t_c_position const &get_position(void) const
		{
			return (position);
		}
		std::string const &get_token(void) const
		{
			return (token);
		}
		std::string to_string(void) const
		{
			return (position.to_string() + ' ' + token);
		}
};

struct t_c_server_constructor_params
{
	std::vector<std::string> *host_names = nullptr;
	t_c_position              *host_names_position = nullptr;
	std::vector<uint16_t>     ports; // must be in network format (big endian)
	t_c_position              *ports_position = nullptr;
	t_c_router const         *router = nullptr;
	t_c_position              *router_position = nullptr;
	std::string               http_version_not_supported;
	t_c_position              *http_version_not_supported_position = nullptr;
	std::string               not_implemeted;
	t_c_position              *not_implemeted_position = nullptr;
	std::string               internal_server_error;
	t_c_position              *internal_server_error_position = nullptr;
	std::string               uri_too_long;
	t_c_position              *uri_too_long_position = nullptr;
	std::string               content_too_large;
	t_c_position              *content_too_large_position = nullptr;
	std::string               length_requiered;
	t_c_position              *length_requiered_position = nullptr;
	std::string               request_timeout;
	t_c_position              *request_timeout_position = nullptr;
	std::string               not_found;
	t_c_position              *not_found_position = nullptr;
	std::string               forbidden;
	t_c_position              *forbidden_position = nullptr;
	std::string               bad_request;
	t_c_position              *bad_request_position = nullptr;
	std::string const        *resource_is_a_directory_page = nullptr; // page to be served in case file
																	  // is a directory assing empty string
																	  // to allow directory listing
	t_c_position *resource_is_a_directory_page_position = nullptr;
	uint64_t     client_body_size_limit = UINT64_MAX; // UINT64_MAX to disable
	t_c_position *client_body_size_limit_position = nullptr;
};

class t_c_server_config_token : public t_c_server_config
{
	private:
		t_c_position position;

	public:
		t_c_server_config_token(t_c_server_config const &server_config_param, t_c_position const &position_param)
			: t_c_server_config(server_config_param), position(position_param)
		{
		}

		t_c_server_config const &get_server_config(void) const
		{
			return (*this);
		}

		t_c_position const &get_position(void) const
		{
			return (position);
		}
};

class t_c_individual_server_config_token : public t_c_individual_server_config
{
	private:
		t_c_position position;

	public:
		t_c_individual_server_config_token(t_c_individual_server_config const &server_config_param,
										   t_c_position const                 &position_param)
			: t_c_individual_server_config(server_config_param), position(position_param)
		{
		}

		t_c_individual_server_config const &get_server_config(void) const
		{
			return (*this);
		}

		t_c_position const &get_position(void) const
		{
			return (position);
		}
};

t_c_global_config     *get_config(char const *config_file);
std::vector<t_c_token> get_tokens(char const *config_file, int &error_count);
void get_ports(t_c_server_constructor_params &params, std::vector<t_c_token> const &tokens, size_t &i,
		char const *config_file, int &error_count);
void get_hosts(t_c_server_constructor_params &params, std::vector<t_c_token> const &tokens, size_t &i,
		char const *config_file, int &error_count);
void get_error_page(t_c_server_constructor_params &params, std::vector<t_c_token> &tokens, size_t &i,
		char const *config_file, int &error_count);
void get_client_body_size(t_c_server_constructor_params &params, std::vector<t_c_token> const &tokens, size_t &i,
		char const *config_file, int &error_count);
void get_router(t_c_server_constructor_params &params, std::vector<t_c_token> const &tokens, size_t &i,
		char const *config_file, int &error_count);

#pragma GCC diagnostic pop
