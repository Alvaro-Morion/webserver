#include <stddef.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
#include "../config/config.hpp"

/*class ReturnType
{
    private:
        int fd;
        pid_t child; // 0 != cgi
    
    public:
        ReturnType(int fd, pid_t pid = 0);
        ReturnType(std::string response, pid_t pid = 0);

        int     getFd(void);
        pid_t   getChild(void);

        bool    is_cgi(ReturnType &reponse);


};

ReturnType handle_invalid_request(void);
ReturnType handle_error(int error_code, t_c_individual_server_config const &config);
ReturnType handle_request(std::string const &request, t_c_individual_server_config &config);*/