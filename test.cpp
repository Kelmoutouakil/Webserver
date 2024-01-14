#include <iostream>
#include <string>
#include <fstream>

void ChunckedMethod(std::string body) {
    std::ofstream Out("output.txt", std::ios::out | std::ios::binary);  // Open a file for binary output

    if (!Out.is_open()) {
        std::cerr << "Error opening the output file." << std::endl;
        return;
    }

    int i = 0;
    std::string line;
    size_t chunkSize;
    size_t len = body.length();
    // std::cout<< "body : " << body;
    //size_t totalSize = 30;` 
    // std::cout << "hello\n";
    while (len > 0 ) 
    {
        i = 0;
        while (body[i] && body[i] != '\r')
            line.push_back(body[i++]);
            std::cout << "line : "<< line << "\n";
        i += 2;
        chunkSize = std::stoi(line, nullptr, 16);
        // std::cout << chunkSize << "here";
        if (chunkSize == 0)
            break;
        // std::cout << body.substr(i, chunkSize)<< "<   -----\n";
        Out<< body.substr(i,chunkSize);;
        body.erase(0, i + chunkSize);
        len -= i + chunkSize;
       // totalSize -= i + chunkSize;
    }

    Out.close();  // Close the file after writing
}
int main() {
    // Example chunked body
    std::string exampleBody = "1A\r\nThis is the first chunk\r\n"
                              "1E\r\nThis is the second chunk\r\n"
                              "0\r\n";

    // Call the ChunkedMethod with the example body
    ChunckedMethod(exampleBody);

    std::cout << "Test completed successfully." << std::endl;

    return 0;
}