#pragma once
#ifndef BUFFER_SIZE
#define BUFFER_SIZE 501
#endif
 
//#include "WebServer.hpp"
#include "InFile.hpp"
#include <sys/stat.h>
#include <map>
class Location;
class Server;

class Client
{
        bool readMore;
        std::string request;
        std::map <std::string, std::string> header;
        std::string body;
        std::string root;
        std::string M_U_V[3];
        char buffer[BUFFER_SIZE];
        Server *Serv;
        Location *location;
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
        void    ParseHeader(std::string &);
        void    ParseFirstLine(std::string );
        void    SendHeader(std::string); 
        void    PostMethod(Client obj);
        void    ChunckedMethod(Client obj);
        void    ReadMore();
        void    PostMethodfunc();
        void    GetMethod();
        void    DeleteMethod();
        void    ServeError(const std::string &, const std::string &);
        void    OpeningFile();
};

bool fileExists(const std::string& filePath) ;