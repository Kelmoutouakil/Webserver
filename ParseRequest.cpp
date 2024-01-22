#include "Client.hpp"
#include "WebServer.hpp"
#include "InFile.hpp"
#include "Server.hpp"

void    Client::ParseKeyValue(std::string &ln, std::string line)
{
    std::string second;

    if (std::count(line.begin(), line.end(), ':') == 0)
        ServeError("400", " Bad Request\r\n");  
    second = line.substr(line.find(":") + 1);
    line = line.substr(0, line.find(":"));
    std::stringstream a(line);
    std::stringstream b(second);
    a >> line;
    b >> second;
    header[line] = second;
    ln.erase(ln.begin(), ln.begin() + ln.find("\r\n") + 2);
}

void   Client::ParseFirstLine(std::string line)
{
    std::stringstream first(line);
    for (int i = 0;i < 3 && first >> M_U_V[i];i++)
        ;
    if (!M_U_V[0][0] || !M_U_V[1][0]|| !M_U_V[1][0])
        ServeError("400", " Bad Request\r\n");
    std::string URI(M_U_V[1]);
    while (URI.length() && Serv->locations.find( URI) == Serv->locations.end())
        URI.pop_back();
    if (Serv->locations.find(URI) != Serv->locations.end())
    {
        location = &Serv->locations[URI];
        if (location->root.back() == '/')
            location->root.pop_back();
        //std::cout << R << location->root << std::endl << D;
    }
    else
        ServeError("4042", " Not Found\r\n");
    //std::cout << Y << "LOCATION:" << location->root << "<" << std::endl << D;
    request.erase(request.begin(), request.begin() + request.find("\r\n") + 2);
}

void Client::ReadMore()
{
    //std::cout  << R << "readMore ...\n" << D;
    int r = read(fd, buffer, BUFFER_SIZE - 1);
    if (!r) 
        ServeError("400", " Bad Request\r\n");
    buffer[r] = 0;
    request.insert(request.end(), buffer , buffer + r);
    if (request.find("\r\n\r\n") != std::string::npos)
    {
        readMore = 0;
        header.clear();
        body.clear();
        std::string head(request.substr(0, request.find("\r\n\r\n") + 2));
        std::cout << Y << head << std::endl << D;
        body.insert(body.end(),  request.begin() + request.find("\r\n\r\n") + 4, request.end());
        ParseFirstLine(head.substr(0, head.find("\r\n")));
        head.erase(head.begin() , head.begin() + head.find("\r\n") + 2);
        while(head.find("\r\n") != std::string::npos)
            ParseKeyValue(head, head.substr(0, head.find("\r\n")));
        if (header.find("Host") == header.end())
            ServeError("400", " Bad Request\r\n");
        request.clear();
        //Header(header, M_U_V);
        //sleep(2);
    }
}