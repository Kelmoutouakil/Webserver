#include "Client.hpp"
#include "WebServer.hpp"
#include "InFile.hpp"
#include "Server.hpp"

Client::Client(int Fd,Server *serv) : Serv(serv) ,fd(Fd)
{
    readMore = 1;
    location = NULL;
    check = -1;
    In = new InFile();
    Out = new std::ofstream();
}

Client::Client(const Client &obj)
{
    std::cout << "copy constructor called\n";
    *this = obj;
}

Client& Client::operator=(const Client &obj)
{
    for (int i = 0; i < 3; i++)
        M_U_V[i] =  obj.M_U_V[i];
    readMore = obj.readMore;
    header =  obj.header;
    body =  obj.body;
    root =  obj.root;
    Out =  obj.Out;
    In =  obj.In;
    fd = obj.fd;
    check = obj.check;
    Serv = obj.Serv;

    return *this;
}

Client::~Client()
{
    std::cout << "client destructor called \n";
}

void Header(std::map<std::string, std::string> header)
{
    std::cout << "\n\33[1;32m";
    for (std::map<std::string , std::string>::iterator i = header.begin(); i != header.end(); i++)
        std::cout << "key:" << i->first << "|value:" << i->second << "|\n";
    std::cout << "\33[0m\n";
}

void    Client::ServeError(const std::string &Error, const std::string &reason)
{
    std::string response (M_U_V[2]);
    if (!response.length())
        response = "HTTP/1.0";
    response = response + " " + Error + reason + "content-type: txt/html\r\n"; 
    In->open(Serv->errorPages[Error].c_str());
    if (!In->is_open())
        throw std::runtime_error("Error in opning error file\n");
    std::string body((std::istreambuf_iterator<char>(*In)), std::istreambuf_iterator<char>());
    int size = In->size();
    std::cout << "size : " << size << std::endl;
    In->read(buffer, size);
    response = response +  "content-length: " + std::to_string(size) + "\r\n\r\n" + body;
    write(fd, response.c_str(), response.length());
    throw std::runtime_error(Error);
}





void    Client::DeleteMethod()
{
}


