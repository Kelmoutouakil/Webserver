#pragma once

#include "WebServer.hpp" 
#include "inFile.hpp"

class Info
{
        bool rd;
        bool wr;
        std::vector<std::string> request;
        std::string root;
        std::string M_U_V[3];
        inFile InFile;
        std::ofstream OutFile;
        char buffer[BUFFER_SIZE];
        Info& operator=(const Info &obj);
    public:
        int fd;
        Info();
        ~Info();
        Info(const Info &obj);
        bool IsR_Wr();
        bool IsR_Rd();
        void   handleRequest();
        void   readRequest();
        void    getMethode();
        void    setUp();
};

