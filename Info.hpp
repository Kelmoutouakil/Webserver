#pragma once

#include "WebServer.hpp" 

class Info
{
        std::string M_U_V[3];
        std::ifstream InFile;
        std::ofstream OutFile;
        std::vector<std::string> request;
        char buffer[BUFFER_SIZE];
        int fd;
    public:
        Info();
        Info(int);
        ~Info();
        Info(const Info &obj);
        Info& operator=(const Info &obj);
        void   handleRequest();
        void   readRequest();
        void    getMethode();
        void    setUp();
};

Info::Info()
{

}

Info::Info(int fds)
{
    std::string reque;
    char buffer[BUFFER_SIZE];
    int r;

    fd = fds;
    while(true)
    {
        r = read(fd, buffer, BUFFER_SIZE - 1);
        if (r <= 0)
        {
            std::cout << "error reading" << std::endl;
            exit(0);
        }
        buffer[r] = '\0';
        reque += buffer;
        if (reque.find("\r\n\r\n") != std::string::npos)
            break;
    }
    while(true)
    {
        if (reque.find("\r\n") != reque.find("\r\n\r"))
        {
            request.push_back(reque.substr(0,reque.find("\r\n")));
            break;
        }
        request.push_back(reque.substr(0, reque.find("\r\n")));
        reque = reque.substr(reque.find("\r\n"), reque.length());
    }
    this->readRequest();
}

Info::~Info()
{

}

Info::Info(const Info &obj)
{

}

Info& Info::operator=(const Info &obj)
{
    return *this;
}

void   Info::handleRequest()
{

}

void   Info::readRequest()
{
    std::stringstream a(request[0]);
    std::string word;
    int i;
    for(i = 0; i < 3 && a >> word; i++)
        M_U_V[i] = word;
    if (i < 3)
    {
        std::cout << "error header request contain less then 3 words\n";
        exit(0);
    }
    if (M_U_V[0] == "GET")
        this->getMethode();
}

void    Info::getMethode()
{
    std::string header;
    std::string conType("Content-Type: text/html\n");
    header = M_U_V[2] + " 200 OK\n" + conType + "Transfer-Encoding: chunked\n\n";
    write(fd, header.c_str(), header.length());
}

void    Info::setUp()
{

}
