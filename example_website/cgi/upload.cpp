#include <iostream>
#include <fstream>
#include <vector>

std::string extractFilename(const std::string& input)
{
    std::string filenameKey = "filename=\"";
    std::size_t startPos = input.find(filenameKey);
    if (startPos == std::string::npos) {
        return ""; // Filename key not found
    }
    
    startPos += filenameKey.length();
    std::size_t endPos = input.find("\"", startPos);
    if (endPos == std::string::npos) {
        return ""; // Closing quote not found
    }
    
    return input.substr(startPos, endPos - startPos);
}

void deleteLastLine(const std::string& filePath) {
    std::ifstream inputFile(filePath);
    std::vector<std::string> lines;
    std::string line;

    // Read the lines from the file and store them in a vector
    while (std::getline(inputFile, line)) {
        lines.push_back(line);
    }

    inputFile.close();
    std::cout << "hello " << lines[1] << std::endl;

    // Reopen the file in write mode to overwrite its contents
    std::ofstream outputFile("./upload/" + extractFilename(lines[1]), std::ios::trunc);
	size_t i = 4;
    // Write all lines except the last one back to the file
    while (i < lines.size() - 2) {
        outputFile << lines[i++] << std::endl;
    }
    outputFile << lines[i].substr(0, lines[i].size() - 1) << std::endl;

    outputFile.close();

//    std::cout << "Last line deleted successfully from file: " << filePath << std::endl;
}

int main() {
    // Set the path where you want to save the uploaded image
    std::string upload_dir = "./upload/";

    // Read the image data from standard input
    std::cin >> std::noskipws;
    std::ofstream image_file(upload_dir + "uploaded_image.png", std::ios::binary);
    image_file << std::cin.rdbuf();
    image_file.close();

    // Print a simple HTTP response to indicate the upload was successful
	std::cout << std::string("HTTP/1.1 200\r\n") + "Server: webserv/0.1\r\n" + "\r\n" + "Connection: close" + "\r\n\r\n";
    std::cout << "<html><body>";
    std::cout << "<h1>Image Upload Successful</h1>";
    std::cout << "<p>The image has been saved as: " << upload_dir << "uploaded_image.jpg</p>";
    std::cout << "</body></html>";
    deleteLastLine("./upload/uploaded_image.png");

    return 0;
}

