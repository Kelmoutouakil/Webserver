#include "Client.hpp"
#include "WebServer.hpp"
#include "InFile.hpp"
#include "Server.hpp"

void Header(std::map<std::string, std::string> header, std::string FirstLine[3])
{
    std::cout << G + (std::string)"request:\n" << FirstLine[0] + "|" << FirstLine[1] + "|" << FirstLine[2] + "\n";
    for (std::map<std::string , std::string>::iterator i = header.begin(); i != header.end(); i++)
    {
        std::cout << "|";
        p(i->first);
        std::cout << ":";
        p(i->second);
        std::cout  << "|\n";
    }
    std::cout << "\33[0m\n";
}

void    lower(char & c)
{
    if (c <= 'Z' && c >= 'A')
        c +=32;
}

size_t    Client::OurWR(const char *buf, int size)                                                                                             //OurWR(std::string)
{
    int len;
    len = write(fd, buf, size);
    std::cout << std::flush;
    if (len == 0)
        throw std::runtime_error("The client connection has been disconnected while writing");
    if (len == -1)
        throw std::runtime_error("Error accured while writing");
    return len;
}

size_t  Client::OurRD(char *buf, int size)
{
    int len;
    len = read(fd, buf, size);
    std::cout << std::flush;
    if (len == 0)
        throw std::runtime_error("The client connection has been disconnected while reading");
    if (len == -1)
        throw std::runtime_error("Error accured while reading");
    return len;
}

void    Client::ServeError(const std::string &Error, const std::string &reason)                                             //ServeError()
{
    std::string res (M_U_V[2]);
    if (!res.length())
        res = "HTTP/1.1";
    res +=" " + Error + reason + "Content-Type: ";
    In->open(Serv->errorPages[Error].c_str());
    if (!In->is_open())
    {
        int number = 46 + Error.size();
        std::stringstream ss;
        ss << number;
        std::string numberStr = ss.str();
        res +=  "text/html\r\nContent-Length: " + ss.str() + "\r\n\r\n<!DOCTYPE html>Error in opning error file nB:" + Error + "\n";
        OurWR(res.c_str(), res.size());
        throw std::runtime_error(Error + "hi");
    }
    int size = In->size();
    std::stringstream f;
    f << size;
    std::string body((std::istreambuf_iterator<char>(*In)), std::istreambuf_iterator<char>());
    res += "text/html\r\nContent-Length: " + f.str() + "\r\n\r\n" + body;
    OurWR(res.c_str(), res.size());
    throw std::runtime_error(Error);
}
