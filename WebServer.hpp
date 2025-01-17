#pragma once
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <vector>
#include <sstream>
#include <fstream>
#include <unistd.h>
#include <map>
#include <algorithm> 
#include <string>
#include <functional>
#include "Server.hpp"
#include <dirent.h>
#include <unistd.h>

#define BUFFER_SIZE 1024
typedef  std::map<std::string, std::vector<Server> > mp;
class WebServer 
{
        
    public:
        WebServer();
        std::string conf;
        fd_set FdRd, FdWr;
        std::vector<Server> servers;
        std::map<std::string, std::vector<Server> > hosts;
        std::vector<int > fds;
        std::vector<int > fdwrite;
        WebServer(int, char **);
        void RUN();
        void print();
};

void Header(std::map<std::string, std::string> header, std::string FirstLine[3]);
