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
        Server Serv;
    public:
        int count;
        std::ofstream *Out;
        InFile *In;
        int fd;
        Client& operator=(const Client &obj);
        Client(int);
        ~Client();
        Client(const Client &obj);
        void   handleRequest(fd_set *, fd_set *);
        void   ParseRequest(std::string &);
        void    openFileSendHeader();   
        void    PostMethode(Client& obj);
};

