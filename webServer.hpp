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
// function help for debug 
int countWords(const std::string& input);

void reque(std::vector<std::string > &request);