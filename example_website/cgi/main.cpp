#include <iostream>
#include <fstream>
#include <vector>


void deleteLastLine(const std::string& filePath) {
    std::ifstream inputFile(filePath);
    std::vector<std::string> lines;
    std::string line;

    // Read the lines from the file and store them in a vector
    while (std::getline(inputFile, line)) {
        lines.push_back(line);
    }

    inputFile.close();

    // Reopen the file in write mode to overwrite its contents
    std::ofstream outputFile(filePath, std::ios::trunc);
	size_t i = 4;
    // Write all lines except the last one back to the file
    while (i < lines.size() - 2) {
        outputFile << lines[i++] << std::endl;
    }
    outputFile << lines[i].substr(0, lines[i].size() - 1) << std::endl;

    outputFile.close();

    std::cout << "Last line deleted successfully from file: " << filePath << std::endl;
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
    std::cout << "Content-Type: text/html\n\n";
    std::cout << "<html><body>";
    std::cout << "<h1>Image Upload Successful</h1>";
    std::cout << "<p>The image has been saved as: " << upload_dir << "uploaded_image.jpg</p>";
    std::cout << "</body></html>";
    deleteLastLine("./upload/uploaded_image.png");

    return 0;
}

