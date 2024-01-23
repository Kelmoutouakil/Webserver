#include "Client.hpp"
#include "WebServer.hpp"
#include "InFile.hpp"
#include "Server.hpp"
#include <fcntl.h>
#include <sys/stat.h>

void    Client::SendHeader(std::string extension)
{
    std::string header;
    std::string conType("txt/txt\r\n");

    for (std::map<std::string, std::vector<std::string> >::iterator i = Serv->mimeTypes.begin(); i != Serv->mimeTypes.end();i++)
    {
        for (std::vector<std::string>::iterator j = i->second.begin(); j != i->second.end(); j++)
        {
            if (extension == *j)
            {
                conType = i->first;
                break;
            }
        }
    }
    header = M_U_V[2] + " 200 OK\r\nContent-Type: " + conType + "\r\nTransfer-Encoding: Chunked\r\n\r\n";
    write(fd, header.c_str(), header.length());
    std::cout << std::flush;
}

void    Client::ServeDirectory()
{
    std::string fil;
    std::string prefix("<li><a href=\"http://" + Serv->ipAdress + M_U_V[1] + ((M_U_V[1].back() == '/')? "":"/"));
    dirent *t;

    std::cout << "autoindex :" << location->autoindex << std::endl;
    if (!location->autoindex || (iN = opendir((location->root + M_U_V[1]).c_str())) == NULL)
        ServeError("404", " Not Found\r\n");
    response = M_U_V[2] + " 200 OK\r\nConten-Type: txt/html\r\n\r\n<html lang=\"en\"> <body>\r\n";
    while((t = readdir(iN)) != NULL)
    {
        if (t->d_type == DT_DIR)
            response += prefix + (std::string)t->d_name + "\">" + t->d_name + "/</a></li>\r\n";
        else
            fil += prefix + (std::string)t->d_name + "\">" + t->d_name + "</a></li>\r\n";
    }
    response +=  fil + "</body>\n</html>\r\n";
    write(fd, response.c_str(), response.size());
    throw std::runtime_error("serv directory");
}

void    Client::GetFile()
{
    std::cout << G << "lcation root:\n" << location->root << "\nURI:" << M_U_V[1] << std::endl << D;
    std::string path(location->root + M_U_V[1]);
    struct stat st;

    if (stat(path.c_str(), &st) == 0)
    {
        //std::cout << B << "its enter \n" << D;
        if (S_ISREG(st.st_mode))
        {
            In->open(path.c_str());
            if (!In->is_open())
                throw std::runtime_error("input file not open ?");
            SendHeader(path.substr((path.rfind(".") != std::string::npos )? path.rfind(".") + 1 : 0));
            return ;
        }
        path = (path.back() == '/')? path: path + '/';
        for(size_t i = 0;i < location->index.size(); i++)
        {
            std::string pathfile(path + location->index[i]);
            if (access(pathfile.c_str(), R_OK) != -1)
            {
                In->open(pathfile.c_str());
                if (!In->is_open())
                    throw std::runtime_error("input file not open ?");
                SendHeader(pathfile.substr((pathfile.rfind(".") != std::string::npos)? pathfile.rfind(".") + 1: 0));
                return ;
            }
        }
        ServeDirectory();
    }
    else
        ServeError("4045", " Not Found\r\n");
}

void    Client::GetMethod()
{
    std::stringstream N;
    if (!In->is_open())
    {
        std::cout << Y << "size:\n" << fd << "|"<< location << std::endl << D;
        GetFile();
    }
    else if (In->eof())
    {
        write(fd, "0\r\n\r\n", 5);
        throw std::runtime_error("finish");
    }
    else if (!In->fail())
    {
        In->read(buffer, BUFFER_SIZE - 1);
        N << std::uppercase  << std::hex << In->gcount() ;
        N >> response;
        response.insert(response.end(), "\r\n" , "\r\n" + 2);
        response.insert(response.end(), buffer ,buffer + In->gcount());
        response.insert(response.end(), "\r\n" , "\r\n" + 2);
        write(fd, response.c_str(), response.size());
        std::cout << std::flush;
    }
    else
        throw std::runtime_error("error in the input file G2ET\n"); 
}
