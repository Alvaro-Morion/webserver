#include "MySocket.hpp"

MySocket::MySocket(t_c_individual_server_config config)
{
    this->config = &config;
    this->sockfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, IPPROTO_TCP);
    test_connection(this->sockfd, "Socket: ");
    setAddress(AF_INET, SOCK_STREAM, IPPROTO_TCP, config.get_port(), *config.get_host_name());
    bind_socket();
    test_connection(listen(this->sockfd, 1000), "Listen: "); //no sé que número poner aquí.
}

MySocket::~MySocket()
{
    freeaddrinfo(address);
}

void    MySocket::bind_socket()
{
    struct addrinfo *lp;
    char *error;

    for (lp = this->address; lp != NULL; lp = lp->ai_next)
    {
        if (bind(this->sockfd, lp->ai_addr, lp->ai_addrlen) == 0)
            break;
        error = strerror(errno);
    }
    if (lp == NULL)
    {
        std::cerr << "Bind: " << error << std::endl;
        exit(EXIT_FAILURE);
    }
}

void    MySocket::test_connection(int value, std::string error)
{
    if (value < 0)
    {
        perror(error.c_str());
        exit(EXIT_FAILURE);
    }
}

int MySocket::getSockfd()
{
    return this->sockfd;
}

struct addrinfo *MySocket::getAddress()
{
    return this->address;
}

t_c_individual_server_config *MySocket::getConfig()
{
    return this->config;
}

void MySocket::setAddress(int domain, int type, int protocol, int port, std::string host)
{
    struct addrinfo hint, *res;
    std::stringstream ostream;
    int status;

    memset(&hint, 0, sizeof(hint));
    hint.ai_family = domain;
    hint.ai_socktype = type;
    hint.ai_protocol = protocol;

    ostream << port;

    if((status = getaddrinfo(host.c_str(), ostream.str().c_str(), &hint, &res)) != 0)
    {
        std::cerr << gai_strerror(status) << std::endl;
        exit(EXIT_FAILURE);
    }
    
    this->address = res;
}