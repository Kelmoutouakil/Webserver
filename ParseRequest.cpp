#include "Client.hpp"
#include "WebServer.hpp"
#include "InFile.hpp"
#include "Server.hpp"

void    Client::ParseKeyValue(std::string line)                                                                               //ParseKeyValue()
{
    std::string first, second;
    size_t firstPos = line.find_first_not_of(" \t\n\r\f\v");
    size_t lastPos = line.find_first_of(": \t\n\r\f\v", firstPos);

    if (std::count(line.begin(), line.end(), ':') == 0)
        ServeError("400", " Bad Request\r\n");
    first = line.substr(firstPos, lastPos - firstPos);
    firstPos = line.find_first_not_of(" \t\n\r\f\v", line.find(":") + 1);
    if (firstPos != std::string::npos)
        second = line.substr(firstPos, line.find_last_not_of(" \t\n\r\f\v") - firstPos + 1);
    std::for_each(first.begin(), first.end(), lower);
    if (first == "content-length" && second.find_first_not_of("0123456789") != std::string::npos)
        ServeError("400", " Bad Request\r\n");
    if ((first == "content-length" && header.find("transfer-encoding") != header.end()) || (first == "transfer-encoding" && header.find("content-length") != header.end()))
        ServeError("400", " Bad Request\r\n");
    if (header.find(first) != header.end())
        ServeError("400", " Bad Request\r\n");
    if (first == "content-length" && second.empty())
        ServeError("400", " Bad Request\r\n");
    header[first] = second;
}

void   Client::Redirection(int status, std::string where)                                                                                        //Redirection()
{
    std::stringstream stat;
    std::string st;
    stat << status;
    stat >> st;
    if (status == 301)
        response = M_U_V[2] + " " + st + " Moved Permanently\r\n";
    else
        response = M_U_V[2] + " " + st + " Found\r\n";
    response += "Location: " + where + "\r\n\r\n";
    OurWR(response.c_str(), response.size());
    throw std::runtime_error("Redirection :" + response);
}

void    Client::fileValide(std::string path, int mode)
{
    struct stat st;
    if (stat(path.c_str(), &st) == 0)
    {
        if (S_ISDIR(st.st_mode)  || access(path.c_str(), mode) != 0)
            ServeError("500", " Internal Server Error\r\n");
    }
    else
        ServeError("500", " Internal Server Error\r\n");

}

void Client::IsCgi(std::string file)
{
    std::string exten = file.substr(file.rfind("."));
    file = file.substr(0, file.rfind("."));
    if (exten.find("/") != std::string::npos)
    {
        PATH_INFO = exten.substr(exten.find("/"));
        exten.erase(exten.begin() + exten.find("/"), exten.end());
    }
    if (location->cgi.find(exten) != location->cgi.end())
    {
        M_U_V[1].append(file + exten);
        if (file[0] == '/')
            file.erase(file.begin());
        PathCGI = location->cgi[exten];
        dir = location->root + file + exten;
        std::string name = dir.substr(dir.rfind("/") + 1);
        dir = dir.substr(0 , dir.rfind("/"));
        envv.push_back("REQUEST_METHOD=" + M_U_V[0]);
        envv.push_back("SERVER_PROTOCOL=" + M_U_V[2]);
        envv.push_back("SERVER_NAME=" + header["host"]);
        envv.push_back("SERVER_PORT=" + Serv->host.substr(Serv->host.find(":") + 1));
        envv.push_back("SCRIPT_NAME=" + M_U_V[1]);
        envv.push_back("GATEWAY_INTERFACE=CGI/1.1");
        envv.push_back("REDIRECT_STATUS=200");
        envv.push_back("SCRIPT_FILENAME=" + name);
        envv.push_back("PATH_TRANSLATED=" + name);
        if (!query.empty())
            envv.push_back("QUERY_STRING=" + query);
        if (PATH_INFO != "")
            envv.push_back("PATH_INFO=" + PATH_INFO);
        if (header.find("content-type") != header.end())
            envv.push_back("CONTENT_TYPE=" + header["content-type"]);
        if (header.find("content-length") != header.end())
            envv.push_back("CONTENT_LENGTH=" + header["content-length"]);
        if (header.find("cookie") != header.end())
            envv.push_back("HTTP_COOKIE=" + header["cookie"]);
        argvv[0] = PathCGI;
        argvv[1] = name;
        fileValide(argvv[0], X_OK);
        fileValide(location->root +file +exten, X_OK);
        M_U_V[1] = location->root + file + exten;
    }
} 

void   Client::ParseFirstLine(std::string line)                                                                               //ParseFirstLi()                                          
{
    size_t pos;
    std::stringstream first(line);
    int  i;
    for (i = 0; std::getline(first, M_U_V[i], ' ') && i < 3;i++)
        ;
    if (i != 3 || std::getline(first, line))
        ServeError("400", " Bad Request\r\n");
    if (M_U_V[0] == "PUT" || M_U_V[0] ==  "CONNECT" || M_U_V[0] == "OPTIONS" || M_U_V[0] == "TRACE" || M_U_V[0] == "PATCH" || M_U_V[0] == "HEAD")
        ServeError("501"," Not Implemented\r\n");
    if (M_U_V[0] != "GET" && M_U_V[0] != "POST" && M_U_V[0] != "DELETE")
        ServeError("400"," Bad Request\r\n");
    if (M_U_V[1].size() > 4096)
        ServeError("414", " Request-URI Too Large\r\n");
    if (M_U_V[2] != "HTTP/1.1")
        ServeError("505", " HTTP Version Not Supported\r\n");
    if (M_U_V[1].empty() || M_U_V[1][0] != '/')
        ServeError("400"," Bad Request\r\n");
    if ((pos = M_U_V[1].find("?")) != std::string::npos)
    {
        query = M_U_V[1].substr(pos + 1);
        M_U_V[1].erase(M_U_V[1].begin() + pos , M_U_V[1].end());
    }
    for (size_t i;(i = M_U_V[1].find("%")) != std::string::npos;)
    {
        std::stringstream nb;
        int a;
        nb << std::hex << M_U_V[1].substr(i + 1, 2);
        nb >> a;
        M_U_V[1].replace(i, 3, std::string(1, a));
    }
}

void    Client::resolvepath(std::string& URI)
{
    std::vector<std::string> parts;
    std::stringstream ss(URI);
    std::string part;
    bool t = false;
    if (URI.size() > 1 && URI[URI.length() - 1] == '/')
        t = true;
    while (std::getline(ss, part, '/')) 
    {
        if (part == "..")
        {
            if (parts.empty())
                ServeError("400"," Bad Request\r\n");
            parts.pop_back();
        }
        else if (part != "." && part != ".." && part != "") 
            parts.push_back(part);
    }
    URI.clear();
    for (std::vector<std::string>::iterator p = parts.begin(); p != parts.end();p++)
        URI += '/' + *p;
    if (URI.empty())
        URI = "/";
    if (URI[URI.length() - 1] != '/' && t)
        URI.append("/");
}

void    Client::MatchLocation(std::string &URI)                                                                                  //MatchLocation(strign URI)
{
    resolvepath(URI);
    uri = URI;
    AfterLoc = "";
    while (URI.length() && Serv->locations.find(URI) == Serv->locations.end())
    {
        AfterLoc.insert(AfterLoc.begin(), URI[URI.length() - 1]); 
        URI.erase(URI.length() - 1);
    }
    if (Serv->locations.find(URI) != Serv->locations.end())
    {
        location = &Serv->locations[URI];
        location->name = URI;
        if (location->Return.size() && (location->Return.begin()->first == 302 || location->Return.begin()->first == 301))
            Redirection(location->Return.begin()->first, location->Return.begin()->second);
        if (location->root[location->root.size() - 1] != '/')
            location->root.append("/");
        if (AfterLoc.rfind(".") != std::string::npos)
            IsCgi(AfterLoc);
        if (PathCGI == "")
        {
            if (!AfterLoc.empty() && AfterLoc[0] == '/')
                AfterLoc.erase(AfterLoc.begin());
            M_U_V[1] = location->root + AfterLoc;
        }
    }
    else
        ServeError("404", " Not Found\r\n");
    if (location->allow_methods.find(M_U_V[0]) != location->allow_methods.end() && !location->allow_methods[M_U_V[0]])
        ServeError("405", " Method Not Allowed\r\n");
}

void    Client::ReadMore()                                                                     //ReadMore()
{
    std::vector <std::string> Names;
    std::string name;
    int r = OurRD(buffer, BUFFER_SIZE);
    T = clock();
    request.insert(request.end(), buffer , buffer + r);
    if (request.find("\r\n\r\n") != std::string::npos ) //parce the request
    {
        readMore = 0;
        body.insert(body.end(),  request.begin() + request.find("\r\n\r\n") + 4, request.end());
        request.erase(request.begin() + request.find("\r\n\r\n") + 2, request.end());
        ParseFirstLine(request.substr(0, request.find("\r\n")));
        request.erase(request.begin() , request.begin() + request.find("\r\n") + 2);
        while(request.find("\r\n") != std::string::npos)
        {
            ParseKeyValue(request.substr(0, request.find("\r\n")));
            request.erase(request.begin(), request.begin() + request.find("\r\n") + 2);
        }
        if (header.find("host") == header.end())
            ServeError("400", " Bad Request\r\n");
        else                                           //match the server 
        {
            name = header["host"];
            if (header["host"].find(":") != std::string::npos)
                name = header["host"].substr(0, header["host"].find(":"));
            for (size_t i = 0; i < Serv->Servs.size(); i++)
            {
                Names = Serv->Servs[i].serverName;
                if (std::find(Names.begin(), Names.end(), name) != Names.end())
                    Serv = &Serv->Servs[i];
            }
        }
        MatchLocation(M_U_V[1]);                       //match the location
    }
}
