#include <iostream>
#include <fstream>

int main() {
    // Set the path where you want to save the uploaded image
    std::string upload_dir = "./";

    // Read the image data from standard input
    std::cin >> std::noskipws;
    std::ofstream image_file(upload_dir + "uploaded_image.jpg", std::ios::binary);
    image_file << std::cin.rdbuf();
    image_file.close();

    // Print a simple HTTP response to indicate the upload was successful
    std::cout << "Content-Type: text/html\n\n";
    std::cout << "<html><body>";
    std::cout << "<h1>Image Upload Successful</h1>";
    std::cout << "<p>The image has been saved as: " << upload_dir << "uploaded_image.jpg</p>";
    std::cout << "</body></html>";

    return 0;
}
