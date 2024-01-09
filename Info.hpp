#pragma once

#include "WebServer.hpp" 

class Info
{
        std::string root;
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

