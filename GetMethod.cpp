#include "Client.hpp"
#include "WebServer.hpp"
#include "InFile.hpp"
#include "Server.hpp"

void   Client::ParseFirstLine(std::string line)
{
    std::stringstream first(line);

    for (int i = 0;i < 3 && first >> M_U_V[i];i++)
        ;
    if (!M_U_V[0][0] || !M_U_V[1][0]|| !M_U_V[1][0])
        ServeError("400", " Bad Request\r\n");
    while (M_U_V[1].length() && Serv->locations.find(M_U_V[1]) == Serv->locations.end())
        M_U_V[1].pop_back();
        // std::cout << "location name :" << M_U_V[1] << "<";
    // for(std::map<std::string , Location>::iterator i = Serv->locations.begin(); i != Serv->locations.end();i++)
    //     std::cout << "\33[1;32mlocation :" << ">" << i->first << "<" << "\n\33[1;32m";
    if (Serv->locations.find(M_U_V[1]) != Serv->locations.end())
    {
        location = &Serv->locations[M_U_V[1]];
        if (location->root.back() == '/')
            location->root.erase(location->root.end() - 1);
        if (M_U_V[1].back() != '/')
            M_U_V[1].append("/");
        location->root.append(M_U_V[1]);
    }
    else
        ServeError("404", " Not found\r\n");
    request.erase(request.begin(), request.begin() + request.find("\r\n") + 2);
}



void    Client::SendHeader(std::string extension)
{
    std::string header;
    std::string conType("txt/txt\r\n");

    std::cout << "extension :>" << extension << "<" << std::endl;
    for (std::map<std::string, std::vector<std::string> >::iterator i = Serv->mimeTypes.begin(); i != Serv->mimeTypes.end();i++)
    {
        std::cout << "content-type:" << i->first << "<" << std::endl;
        for (std::vector<std::string>::iterator j = i->second.begin(); j != i->second.end(); j++)
        {
            if (extension == *j)
            {
                std::cout << "enter \n";
                conType = i->first;
                break;
            }
        }
    }
    header = M_U_V[2] + " 200 OK\r\nContent-Type: " + conType + "\r\nTransfer-Encoding: Chunked\r\n\r\n";
    write(fd, header.c_str(), header.length());
    std::cout << std::flush;
}

void    Client::GetMethod()
{
    if (!In->is_open())
    {
        for(size_t i = 0;i < location->index.size(); i++)
        {
            std::cout << ">" << location->index[i] << "<" << std::endl;
            if (access((location->root + location->index[i]).c_str(), R_OK) != -1)
            {
                // std::cout << location->root << location->index[i] << std::endl;
                In->open((location->root + location->index[i]).c_str());
                if (!In->is_open())
                    throw std::runtime_error("input file not open ?");
                SendHeader((location->root + location->index[i]).substr((location->root + location->index[i]).rfind(".") + 1));
                return ;
            }
            if (i == location->index.size() - 1)
                ServeError("404", " Not Found\r\n");
        }
    }
    if (!In->fail())
    {
        std::cout << "fail :" << In->fail() << "\n";
        In->read(buffer, BUFFER_SIZE - 1);
        write(fd, buffer, (size_t)In->gcount());
        std::cout << std::flush;
        if (In->eof())
            throw std::runtime_error("hello");
    }
    else
        throw std::runtime_error("error in the input file G2ET\n");
    std::cout << "out GET\n";
        
}

void    Client::ParseKeyValue(std::string line)
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
    request.erase(request.begin(), request.begin() + request.find("\r\n") + 2);
}


void Client::ReadMore()
{
    std::string line;

    int r = read(fd, buffer, BUFFER_SIZE - 1);
    if (!r)
        ServeError("400", " Bad Request\r\n");
    buffer[r] = 0;
    request += buffer;
    if (request.find("\r\n\r\n") != std::string::npos)
    {
        readMore = 0;
        ParseFirstLine(request.substr(0, request.find("\r\n")));
        if (request == "\r\n")
            return ;
        while(request.find("\r\n\r\n") != std::string::npos)
            ParseKeyValue(request.substr(0, request.find("\r\n")));
        if (request.length() > 2)
            body = request.substr(2, request.length());
        if (header.find("Host") == header.end())
            ServeError("400", " Bad Request\r\n");
        Header(header);
    }
}

void   Client::handleRequest(fd_set *Rd, fd_set *Wr)
{
    std::string nBytes;
    std::string response;
    if (FD_ISSET(fd, Rd) || FD_ISSET(fd, Wr))
    {
        std::cout << "hello fd:" <<  fd << std::endl;
        if (readMore)
            ReadMore();
        else if (M_U_V[0] == "GET")
        {
            std::cout << "\33[1;32mhello\n\33[0m";
            GetMethod();
        }
        else if(M_U_V[0] == "POST")
            PostMethodfunc();
        else if (M_U_V[0] == "DELETE")
            DeleteMethod();
    }
}