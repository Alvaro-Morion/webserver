/**
 * @author      : ahammoud (ahammoud@$HOSTNAME)
 * @file        : main
 * @created     : Thursday Jul 11, 2024 16:33:54 CEST
 */

#include <iostream>
#include <sstream>
#include <string>

int main()
{

	std::string std_input;
	std::stringstream stream;
while (getline(std::cin, std_input)) {
  stream << std_input << std::endl;
}

	std::cout << std::string("HTTP/1.1 200\r\n") + "Server: webserv/0.1\r\n" + "\r\n" + "Connection: close" + "\r\n\r\n";


    std::cout << std::endl;
    std::cout << stream.str() ;



return 0;
	}

