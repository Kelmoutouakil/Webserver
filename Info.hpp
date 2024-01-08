#pragma once

class Info
{
        int fd;
        std::ifstream InFile;
        std::ofstream OutFile;
        std::vector<std::string> request;
    public:
        Info(int);
        ~Info();
        Info(const Info &obj);
        Info& operator=(const Info &obj);
        void   handle_request();
};