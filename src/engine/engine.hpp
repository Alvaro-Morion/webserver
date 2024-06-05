#include "../config/config.hpp"
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stddef.h>

class ReturnType
{
	private:
		FILE *file;
		pid_t child; // 0 != cgi

	public:
		ReturnType(FILE *file, pid_t pid);
		~ReturnType(void);

		FILE *getFile(void);
		pid_t getChild(void);

		bool  is_cgi(void);
};

ReturnType handle_invalid_request(void);
ReturnType handle_error(int error_code, t_c_individual_server_config const &config);
ReturnType handle_request(std::string const &request, t_c_individual_server_config &config);