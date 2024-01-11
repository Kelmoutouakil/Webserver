#pragma once
#include <iostream>
#include <fstream>

class InFile:public std::ifstream
{
    public:
        size_t size();
};
