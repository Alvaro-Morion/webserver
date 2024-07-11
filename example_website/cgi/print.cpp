#include <iostream>
#include <istream>
#include <string>
#include <sstream>
#include<fstream>
#include <filesystem>
#include <stdio.h>
#include <unistd.h>

std::string urlDecode(const std::string& str) {
    std::stringstream decoded;
    char ch;
    int i = 0;
    while (i < str.length()) {
        if (str[i] == '%') {
            int hexValue;
            std::stringstream ss;
            ss << std::hex << str.substr(i + 1, 2);
            ss >> hexValue;
            ch = static_cast<char>(hexValue);
            i += 2;
        } else if (str[i] == '+') {
            ch = ' ';
        } else {
            ch = str[i];
        }
        decoded << ch;
        i++;
    }
    return decoded.str();
}

void insertComment(const std::string& body) {
    std::ifstream fileIn("./example_website/assets/data.html");
    if (!fileIn.is_open()) {
        std::cerr << "Error opening file for reading." << std::endl;
        return;
    }

    std::stringstream buffer;
    buffer << fileIn.rdbuf(); // Read the entire file into a stringstream
    fileIn.close();

    std::string fileContent = buffer.str();
    size_t lastPtagPos = fileContent.rfind("</p>");
    size_t bodyPos = fileContent.find("</body>");

    if (lastPtagPos != std::string::npos && bodyPos != std::string::npos) {
        fileContent.insert(lastPtagPos + 4, "<p>" + body + "</p>"); // Insert the new body object
    }
    else {

        fileContent.insert(bodyPos - 2 , "<p>" + body + "</p>"); // Insert the new body object
    }

    std::ofstream fileOut("./example_website/assets/data.html");
    if (!fileOut.is_open()) {
        std::cerr << "Error opening file for writing." << std::endl;
        return;
    }

    fileOut << fileContent; // Write the modified content back to the file
    fileOut.close();
}


int main() {
    std::string input;
    std::getline(std::cin, input);
    std::fstream file;


    // Parse the input string
    std::istringstream iss(input);
    std::string name, email, message;
    char delimiter;
    std::stringstream body;

    // Extract values
    std::getline(iss, name, '=');
    std::getline(iss, name, '&');
    std::getline(iss, email, '=');
    std::getline(iss, email, '&');
    std::getline(iss, message, '=');
    std::getline(iss, message, '&');





//    body << "Name: " << name << std::endl;
//    body << "Email: " << email << std::endl;
    body << "Message: " << urlDecode( message) << std::endl;
    insertComment(body.str());
//    insertComment( "testest");
//    std::cout << "HTTP/1.1 200 OK" << std::endl;
//    std::cout << "Content-Length: " << body.str().length() << std::endl;
	std::cout << std::string("HTTP/1.1 301 Moved Permanently\r\n") + "Server: webserv/0.1\r\n" +
		"Location: " + "/index.html#comments" + "\r\n" + "Connection: close" + "\r\n\r\n";


    std::cout << std::endl;
    std::cout << body.str() ;

    return 0;
}


