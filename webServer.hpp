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
#define PORT 8080
#define BUFFER_SIZE 500

int countWords(const std::string& input);