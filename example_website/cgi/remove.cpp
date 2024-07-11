/**
 * @author      : ahammoud (ahammoud@$HOSTNAME)
 * @file        : main
 * @created     : Tuesday Jul 09, 2024 14:51:55 CEST
 */
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

int main()
{
    
    std::ifstream fileIn("./example_website/assets/data.html");
    if (!fileIn.is_open()) {
        std::cerr << "Error opening file for reading." << std::endl;
        return 0;
    }

    std::stringstream buffer;
    buffer << fileIn.rdbuf(); 
    fileIn.close();

    std::string fileContent = buffer.str();
    size_t lastPtagPos = fileContent.rfind("<p>");
    size_t lastPtagEndPos = fileContent.rfind("</p>");

    if (lastPtagPos != std::string::npos && lastPtagEndPos != std::string::npos){
	    
    	fileContent.erase(lastPtagPos, lastPtagEndPos - lastPtagPos);
    }

    std::ofstream fileOut("./example_website/assets/data.html");
    if (!fileOut.is_open()) {
        std::cerr << "Error opening file for writing." << std::endl;
        return 0;
    }

    fileOut << fileContent; // Write the modified content back to the file
    fileOut.close();
    std::cout << std::string("HTTP/1.1 303 See Other\r\n") + "Server: webserv/0.1\r\n" +
		"Location: " + "/index.html" + "\r\n" + "Connection: close" + "\r\n\r\n";



    return 0;
}

