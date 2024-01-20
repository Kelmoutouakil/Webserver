/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostMethod.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kelmouto <kelmouto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 18:58:37 by kelmouto          #+#    #+#             */
/*   Updated: 2024/01/20 10:17:20 by kelmouto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "WebServer.hpp"
#include "InFile.hpp"
#include "Server.hpp"


bool fileExists(const std::string filePath) 
{
    std::ifstream file(filePath);
    if (file.is_open())
    {
        file.close();
        return true;
    }
    return false;
}

void  Client::PostMethod()
{
    
    Out->write(body.c_str(),body.size());
    body.clear();
}

void Client::ChunckedMethod()
{
    
    std::string line;
    size_t i;
    size_t chunkSize;
    size_t len = body.size();
    size_t totalSize = BUFFER_SIZE;
    while(len > 0 &&  totalSize > 0)
    { 
        i = body.find("\r\n");
        line = body.substr(0,i);
        i+= 2;
        chunkSize = std::stoi(line, nullptr, 16);
        if (chunkSize == 0)
            break;
        *Out<< body.substr(i ,chunkSize);;
        body.erase(0, i  + 2+ chunkSize);
        len -= i + 2 + chunkSize;
        totalSize -= i + 2 + chunkSize;
    }
}

std::string findExtension(std::string t)
{
    std::map<std::string,std::vector<std::string> >::iterator it = Server::mimeTypes.find(t);
    if(it != Server::mimeTypes.end())
        return (*(it->second.begin()));
    return "";
}

void Client::OpeningFile()
{
    std::string t;
    std::string filename;
    t = findExtension(header["Content-Type"]);
    std::srand(time(NULL));
    std::string name  = std::to_string(std::rand() % 100 + 1) + "." + t;
    filename = *(location->uploads.begin() + 1);
    if(fileExists(filename))
    {
        if(filename[filename.length() - 1] != '/')
            filename +=("/" + name );
        else
            filename+= name;
    }
    else
       ServeError("403"," Forbidden\r\n");
    Out->open(filename, std::ios::out | std::ios::app);
    if(!Out->is_open())
        throw std::runtime_error("Couldn't open file ");
}

void Client::PostMethodfunc()
{
    char Store[BUFFER_SIZE];
    int total  = 0;
    int content_length;
    if(location->uploads.size() >= 2 && *(location->uploads.begin()) == "on")
    { 
        std::map<std::string,std::string>::iterator it = header.find("Content-Length");
        if(it != header.end())
        {
            if(check == -1)
            {
                OpeningFile(); 
                count  = body.size();
                check = 0;
            }
            content_length = std::stoi(header["Content-Length"]);
            if( body.size() >= (size_t)content_length)
                {
                    PostMethod();
                    write(fd,M_U_V[2].c_str(),M_U_V[2].length());
                    write(fd," 200 OK\r\n",9);
                    write(fd,"Content-Type: ",14);
                    write(fd,header["Content-Type"].c_str(),header["Content-Type"].length());
                    write(fd,"\r\n\r\nstatus: success\n message: File successfully uploaded\r\n",58);
                    throw std::runtime_error("");
                }
            total = read(fd,Store,BUFFER_SIZE - 1);
            if (total > 0 )
            {
                count += total;
                Store[total] = '\0';
                body.insert(body.end(),Store, Store + total);
                PostMethod();
                if(count >= content_length)
                {
                    write(fd,M_U_V[2].c_str(),M_U_V[2].length());
                    write(fd," 200 OK\r\n",9);
                    write(fd,"Content-Type: ",14);
                    write(fd,header["Content-Type"].c_str(),header["Content-Type"].length());
                    write(fd,"\r\n\r\nstatus: success\n message: File successfully uploaded\r\n",58);
                    throw std::runtime_error("");
                }
            }
        }
        else if(header.find("Transfer-Encoding") != header.end())
        {
            OpeningFile(); 
            if(header["Transfer-Encoding"] == "chunked")
            {
                if(body.substr(body.size() -  4,4) == "\r\n\r\n")
                    {
                        ChunckedMethod();
                        throw std::runtime_error("");
                    }
                total = read(fd,Store,BUFFER_SIZE - 1);
                if(total > 0)
                {
                    Store[total] = '\0';
                    body.insert(body.end(),Store, Store + total);
                    ChunckedMethod();
                    throw std::runtime_error("");
                }
            }
        }
    }
    else
        ServeError("403", " Forbidden\r\n");
}