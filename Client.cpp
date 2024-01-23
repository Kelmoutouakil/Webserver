#include "Client.hpp"
#include "WebServer.hpp"
#include "InFile.hpp"
#include "Server.hpp"

bool ok;
Client::Client(int Fd,Server *serv) : Serv(serv) ,fd(Fd)
{
    readMore = 1;
    location = NULL;
    check = -1;
    In = new InFile();
    Out = new std::ofstream();
    reserve = "";
    chunked = -1;
    flag = false;
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
    chunked = obj.chunked;
    Serv = obj.Serv;
    iN = NULL;
    reserve = obj.reserve;

    return *this;
}

Client::~Client()
{
    std::cout << "client destructor called \n";
}

void Header(std::map<std::string, std::string> header, std::string FirstLine[3])
{
    std::cout << R + (std::string)"request:\n" + G << FirstLine[0] + "|" << FirstLine[1] + "|" << FirstLine[2] + "\n";
    for (std::map<std::string , std::string>::iterator i = header.begin(); i != header.end(); i++)
        std::cout << "|" << i->first << ":" << i->second << "|\n";
    std::cout << "\33[0m\n";
}

void    Client::ServeError(const std::string &Error, const std::string &reason)
{
    std::string res (M_U_V[2]);
    if (!res.length())
        res = "HTTP/1.0";
    res +=" " + Error + reason + "Content-Type: txt/html\r\nContent-Length: "; 
    In->open(Serv->errorPages[Error].c_str());
    if (!In->is_open())
    {
        res +=  std::to_string(31 + Error.size()) + "\r\n\r\nError in opning error file nB:" + Error + "\n";
        write(fd, res.c_str(), res.size());
        throw std::runtime_error(Error);
    }
    std::string body((std::istreambuf_iterator<char>(*In)), std::istreambuf_iterator<char>());
    int size = In->size();
    In->read(buffer, size);
    res += std::to_string(size) + "\r\n\r\n" + body;
    write(fd, res.c_str(), res.length());
    throw std::runtime_error(Error);
}

void    Client::DeleteMethod()
{
}

void Signal(int j)
{
    (void)j;
    ok = 0;
}

void   Client::handleRequest(fd_set *Rd, fd_set *Wr)
{
    std::string nBytes;
    std::string response;
   
    if (FD_ISSET(fd, Rd) || FD_ISSET(fd, Wr))
    {
        
        if (readMore)
            ReadMore();
        else if (M_U_V[0] == "GET")
            GetMethod();
        else if(M_U_V[0] == "POST")
            PostMethodfunc();
        else if (M_U_V[0] == "DELETE")
            DeleteMethod();
        signal(SIGPIPE, Signal);
        if (!ok)
        {
            ok = 1;
            throw std::runtime_error("anvalide socket");
        }
    }
}