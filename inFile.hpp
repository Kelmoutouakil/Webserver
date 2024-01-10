#pragma once
#include <iostream>
#include <fstream>

class inFile:public std::ifstream
{
    public:
    size_t size();
};
