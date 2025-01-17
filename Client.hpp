#pragma once
#ifndef BUFFER_SIZE
#define BUFFER_SIZE 1024
#endif
 
//#include "WebServer.hpp"
#include "InFile.hpp"
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <dirent.h>
#include <vector>
#include <map>
#define R "\33[1;31m"
#define G "\33[1;32m"
#define Y "\33[1;33m"
#define B "\33[1;34m"
#define D "\33[0m"
class Location;
class Server;
static int variable  = 0;
class Client
{
        std::string dir;
        std::string currentdirec;
        bool readMore;
        bool Read;
        std::string uri;
        std::string request;
        std::string AfterLoc;
        std::string response;
        std::map <std::string, std::string> header;
        std::string body;
        std::string root;
        std::string M_U_V[3];
        std::string PATH_INFO;
        char buffer[BUFFER_SIZE];
        Location *location;
        std::string reserve;
        bool flag;
        size_t sum;
        char *env[20];
        bool timeout;
        std::vector<std::string > envv;
        std::string argvv[2];
        char *argv[3];
        Server *Serv;
        clock_t startTime;
        clock_t timing;
    public:
        pid_t id;
        clock_t T;
        DIR *iN;
        std::string query;
        size_t count;
        std::string filename;
        int check;
        int chunked;
        size_t chunkSize;
        std::fstream *Out;
        InFile *In;
        int fd;
        std::string PathCGI;
        Client& operator=(const Client &obj);
        Client(int, Server *);
        ~Client();
        Client(const Client &obj);
        void    GetFile();
        void    handleRequest(fd_set *, fd_set *);
        void    ParseKeyValue(std::string);
        void    ParseFirstLine(std::string );
        void    SendHeader(std::string); 
        void    PostMethod();
        void    ChunckedMethod(ssize_t total);
        void    ReadMore();
        void    PostMethodfunc();
        void    GetMethod();
        void    DeleteMethod(std::string ,bool);
        void    ServeError(const std::string &, const std::string &);
        void    OpeningFile();
        void    ServeDirectory();
        void    Redirection(int, std::string);
        void    processChunk(size_t startIndex);
        void    MatchLocation(std::string &URI);
        void    WriteResponse();
        size_t  OurWR(const char *buf, int size);
        size_t  OurRD(char *buf, int size);
        void    IsCgi(std::string);
        void    handlCgi(std::string filepath,int i);
        void    sendResponse(std::string s);
        void    folder();
        void    fileValide(std::string, int);
        void  fileExists(const std::string filePath);
        void checktimeout();
        bool    Indexes(std::vector<std::string>, std::string,std::string);
        void    resolvepath(std::string& URI);
        void  getcurrentDirectory();
        std::string parsheader(size_t i,std::string s,int n);
};
void    lower(char & c);
void p(std::string s);
void pp(std::map<std::string, std::string> & header);
