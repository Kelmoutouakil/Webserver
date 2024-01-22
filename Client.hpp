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
#define R "\33[1;31m"
#define G "\33[1;32m"
#define Y "\33[1;33m"
#define B "\33[1;34m"
#define D "\33[1m"
class Location;
class Server;

class Client
{
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
        std::string reserve;
        bool flag;
    public:
        DIR *iN;
        int count;
        int check;
        int chunked;
        size_t chunkSize;
        std::ofstream *Out;
        InFile *In;
        int fd;
        Client& operator=(const Client &obj);
        Client(int, Server*);
        ~Client();
        Client(const Client &obj);
        void    GetFile();
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
        void    processChunk(size_t startIndex);
};

bool fileExists(const std::string filePath) ;