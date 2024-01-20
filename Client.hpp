#pragma once
#ifndef BUFFER_SIZE
#define BUFFER_SIZE 501
#endif
 
//#include "WebServer.hpp"
#include "InFile.hpp"
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <map>
class Location;
class Server;

class Client
{
        DIR *iN;
        bool readMore;
        std::string request;
        std::string response;
        std::map <std::string, std::string> header;
        std::string body;
        std::string root;
        std::string M_U_V[3];
        char buffer[BUFFER_SIZE];
        Server *Serv;
        Location *location;
    public:
        int count;
        int check;
        std::ofstream *Out;
        InFile *In;
        int fd;
        Client& operator=(const Client &obj);
        Client(int, Server*);
        ~Client();
        Client(const Client &obj);
        void    handleRequest(fd_set *, fd_set *);
        void    ParseKeyValue(std::string&, std::string);
        void    ParseHeader(std::string &);
        void    ParseFirstLine(std::string );
        void    SendHeader(std::string); 
        void    PostMethod();
        void    ChunckedMethod();
        void    ReadMore();
        void    PostMethodfunc();
        void    GetMethod();
        void    DeleteMethod();
        void    ServeError(const std::string &, const std::string &);
        void    OpeningFile();
        void    ServeDirectory();
};

bool fileExists(const std::string filePath) ;