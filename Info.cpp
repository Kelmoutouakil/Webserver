#include "Info.hpp"
#include "Webserver.hpp"

Info::Info(int fd_sock) 
{
    fd = fd_sock;
    std::string buff;
    std::string line;
    int k;
    char buffer[BUFFER_SIZE] = "";
    while (true)
    {
        k = read(fd_sock, buffer, sizeof(buffer) - 1);
        buffer[k] = 0;
        buff = buffer;
        line += buffer;
        if (line.find("\r\n\r\n") != std::string::npos || k <= 0)
            break;
    }
    while(true)
    {
        request.push_back(line.substr(0,line.find("\r\n")));
        if (line.find("\r\n") != std::string::npos && !std::strcmp(&line.c_str()[line.find("\r\n")], "\r\n\r\n"))
            break;
        line = line.substr(line.find("\r\n") + 2, line.length());
    }
}

Info::~Info() {}

Info::Info(const Info &obj)
{
    (void)obj;
}

Info& Info::operator=(const Info &obj)
{
    (void)obj;
    return *this;
}

void Info::handle_request() 
{
    std::string word;

    std::cout << "Received request:\n\33[0;34m" <<std::endl;
    for (size_t i = 0; i <  request.size();i++)
        std::cout << request[i] << std::endl;
    std::stringstream head(request.front());
    head >> word;
    std::cout << word << std::endl;
    // if (word == "GET")
    //     getMethode(fd, head);
    std::cout << "\33[0m"; 
}