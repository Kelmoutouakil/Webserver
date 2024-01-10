#pragma once

#include "WebServer.hpp" 

class Info
{
        std::string root;
        std::string M_U_V[3];
        std::ifstream InFile;
        std::ofstream OutFile;
        std::vector<std::string> request;
        char buffer[2];
        Info& operator=(const Info &obj);
    public:
        int fd;
        Info(const Info &obj);
        Info();
        ~Info();
        void   handleRequest();
        void   readRequest();
        void    getMethode();
        void    setUp(int);
};

