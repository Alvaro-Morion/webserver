/*#include "engine.hpp"

ReturnType  handle_invalid_request(void)
{
    t_c_default_error_pages default_errors;
    std::string head = "HTTP/1.1 400 Bad Request\r\n\r\n";
    std::ifstream error_file(default_errors.get_bad_request());
    std::stringstream str_stream;
    str_stream << head << error_file.rdbuf();
}

ReturnType handle_error(int error_code, t_c_individual_server_config const &config)
{}

ReturnType::ReturnType(int fd, pid_t child = 0)
{
    this->fd = fd;
    this->child = child;
}

ReturnType::ReturnType(std::string response, pid_t child)
{
    this->child = child;
}

int ReturnType::getFd(void)
{
    return fd;
}

pid_t ReturnType::getChild(void)
{
    return child;
}

bool ReturnType::is_cgi(ReturnType &response)
{
    return (response.getChild() != 0);
}*/