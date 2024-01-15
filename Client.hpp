#pragma once
#ifndef BUFFER_SIZE
#define BUFFER_SIZE 501
#endif

//#include "WebServer.hpp"
#include "InFile.hpp"
#include <map>
class Location;
class Server;

class Client
{
        std::string request;
        std::map <std::string, std::string> header;
        std::string body;
        std::string root;
        std::string M_U_V[3];
        char buffer[BUFFER_SIZE];
        Server *Serv;
        Location *location;
        std::string status;
    public:
        int count;
        std::ofstream *Out;
        InFile *In;
        int fd;
        Client& operator=(const Client &obj);
        Client(int, Server*);
        ~Client();
        Client(const Client &obj);
        void    handleRequest(fd_set *, fd_set *);
        void    ParseRequest();
        void    ParseHeader(std::string &);
        void    ParseFirstLine(std::string );
        void    openFileSendHeader(); 
        void    PostMethod(Client obj);
        void    ChunckedMethod(Client obj);
        void    ReadMore();
        void    PostMethodfunc();
        void    GetMethode();
        void    DeleteMethode();
        void    ServeError(const std::string &, const std::string &);
};

