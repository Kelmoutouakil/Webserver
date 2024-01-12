#pragma once
#ifndef BUFFER_SIZE
#define BUFFER_SIZE 501
#endif

//#include "WebServer.hpp"
#include "InFile.hpp"
#include <map>
class Client
{
        std::map <std::string, std::string> header;
        std::string body;
        std::string root;
        std::string M_U_V[3];
        char buffer[BUFFER_SIZE];
    public:
        std::ofstream *OutFile;
        InFile *InFile;
        int fd;
        Client& operator=(const Client &obj);
        Client();
        ~Client();
        Client(const Client &obj);
        void   handleRequest(fd_set *, fd_set *);
        void   ParseRequest(std::string &);
        void    openFileSendHeader();
};

