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

#define BUFFER_SIZE 501

class WebServer 
{
        
    public:
        WebServer();
        std::string conf;
        fd_set FdRd, FdWr;
        std::vector<Server> servers;
        WebServer(int, char **);
};

void reque(std::vector<std::string > &request);