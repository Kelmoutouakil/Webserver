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
#define PORT 8080
#define BUFFER_SIZE 501
#include "Client.hpp"
#include "Server.hpp"

class WebServer 
{
        
    public:
        char *conf;
        std::map<int ,Client> client;
        std::vector<int> idx;
        fd_set FdRd, FdWr;
        std::vector<Server> servers;
        WebServer(int, char *av[]);
};

void reque(std::vector<std::string > &request);