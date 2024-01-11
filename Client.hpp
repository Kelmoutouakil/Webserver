#pragma once

#include "WebServer.hpp" 
#include "InFile.hpp"

class Client
{
        bool rd;
        bool wr;
        std::map <std::string, std::string> request;
        std::string root;
        std::string M_U_V[3];
        InFile InFile;
        std::ofstream OutFile;
        char buffer[BUFFER_SIZE];
        Client& operator=(const Client &obj);
    public:
        int fd;
        Client();
        ~Client();
        Client(const Client &obj);
        bool IsR_Wr();
        bool IsR_Rd();
        void   handleRequest();
        void   ParseRequest(std::string &);
        void    getMethode();
        void    setUp();
};

