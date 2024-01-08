#include "webServer.hpp"

int countWords(const std::string& input) {
    std::istringstream iss(input);
    int count = 0;
    std::string word;

    while (iss >> word) {
        count++;
    }

    return count;
}