#pragma once
#include <iostream>
#include <fstream>

class InFile:public std::ifstream
{
    public:
        InFile(){}
        size_t size();
};
