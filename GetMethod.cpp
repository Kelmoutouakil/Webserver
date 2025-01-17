#include "Client.hpp"
#include "WebServer.hpp"
#include "InFile.hpp"
#include "Server.hpp"
#include <fcntl.h>
#include <sys/stat.h>

void    Client::SendHeader(std::string extension)                                                                             //SendHeader
{
    std::string header;
    std::string conType("txt/txt\r\n");

    for (std::map<std::string, std::vector<std::string> >::iterator i = Serv->mimeTypes.begin(); i != Serv->mimeTypes.end();i++)
    {
        for (std::vector<std::string>::iterator j = i->second.begin(); j != i->second.end(); j++)
        {
            if (extension == *j)
            {
                conType = i->first + "\r\n";
                break;
            }
        }
    }
    header = M_U_V[2] + " 200 OK\r\nContent-Type: " + conType + "Transfer-Encoding: Chunked\r\n\r\n";
    OurWR(header.c_str(), header.length());
}

void Client::folder()
{
    response = M_U_V[2] + " 200 OK\r\nConten-Type: txt/html\r\n\r\n<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>Directory Listing</title><style>body {font-family: Arial, sans-serif;margin: 0;padding: 0;background-color: #f5f5f5;}.container {width: 80%;margin: 20px auto;padding: 20px;background-color: #fff;border-radius: 5px;box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);}h1 {color: #333;text-align: center;}ul {list-style-type: none;padding: 0;margin: 0;}li{padding: 10px;border-bottom: 1px solid #eee;}li:hover{background-color:#f0f0f0;}a{color: #007bff;text-decoration: none;}a:hover{text-decoration: underline;}</style></head><body><div class=\"container\"><h1>Directory Listing</h1>";
}

void Client::ServeDirectory()
{
    std::string URI(uri);
    if (!URI.empty() && URI[URI.size() - 1] != '/') 
        URI.append("/");
    std::string prefix("<a href=\"http://" + Serv->host + URI);
    std::string fil;
    dirent *t;

    if (!location->autoindex || (iN = opendir(M_U_V[1].c_str())) == NULL) ////   WARNING auto index always set to true 
        ServeError("403", " Forbiden\r\n");
    folder();
    while((t = readdir(iN)) != NULL)
    {
        if (t->d_type == DT_DIR)
            response += prefix + (std::string)t->d_name + "\">" + t->d_name + "/</a><br>\n";
        else
            fil += prefix + (std::string)t->d_name + "\">" + t->d_name + "</a><br>\n";
    }
    response +=  fil + "</body>\n</html>\r\n";
    OurWR(response.c_str(), response.size());
    throw std::runtime_error("serv directory");
}

bool    Client::Indexes(std::vector<std::string> idxs, std::string path, std::string root)
{
    for(size_t i = 0;i < idxs.size(); i++)
    {
        std::string pathfile(path + idxs[i]);
        struct stat st;
        if (stat(pathfile.c_str(), &st) == 0 && S_ISDIR(st.st_mode))
        {
            std::string n(pathfile.substr(root.size()));
            if (n.size() && n[0] != '/' && location->name[location->name.length() - 1] != '/')
                n.insert(n.begin(), '/');
            Redirection(301, uri + idxs[i]);
        }
        if (access(pathfile.c_str(), R_OK) != -1)
        {
            if (idxs[i].find(".") != std::string::npos)
                IsCgi(pathfile.substr(root.size()));
            if (PathCGI != "")
            {
                handlCgi(pathfile, 0);
                return 1;
            }
            In->open(pathfile.c_str());
            if (!In->is_open())
                throw std::runtime_error("input file not open ?");
            SendHeader(pathfile.substr(pathfile.rfind(".") != std::string::npos? pathfile.rfind(".") + 1: 0));
            return 1;
        }
    }
    return 0;
}

void    Client::GetFile()                                                                                                     //GetFile()
{
    std::string path(M_U_V[1]);
    struct stat st;
    if (stat(path.c_str(), &st) == 0)
    {
        if (PathCGI != "")
        {
            handlCgi(path, 0);
            return ;
        }
        if (S_ISREG(st.st_mode))
        {
            if (path[path.size() - 1] == '/')
                ServeError("404", " Not Found\r\n");
            In->open(path.c_str());
            if (!In->is_open())
                ServeError("403", " Forbiden\r\n");
            SendHeader(path.substr((path.rfind(".") != std::string::npos )? path.rfind(".") + 1 : 0));
            return ;
        }
        if (!path.empty() && path[path.size() - 1] != '/') 
            Redirection(301, uri + "/");
        if (Indexes(location->index, path, location->root))
            return;
        ServeDirectory();
    }
    ServeError("404", " Not Found\r\n");
}

void    Client::GetMethod()                                                                                                   //GetMethod()
{
    std::stringstream N;

    if (!In->is_open())
        GetFile();
    else if (In->eof())
    {
        OurWR("0\r\n\r\n", 5);
        throw std::runtime_error("finish");
    }
    else if (!In->fail())
    {
        char s[] = "\r\n";
        In->read(buffer, BUFFER_SIZE - 1);
        N << std::uppercase  << std::hex << In->gcount() ;
        N >> response;
        response.insert(response.end(), s , s + 2);
        response.insert(response.end(), buffer ,buffer + In->gcount());
        response.insert(response.end(), s , s + 2);
        OurWR(response.c_str(), response.size());
    }
    else
        throw std::runtime_error("error in the input file GET\n"); 
}
