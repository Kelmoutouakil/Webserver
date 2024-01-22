/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostMethod.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kelmouto <kelmouto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 18:58:37 by kelmouto          #+#    #+#             */
/*   Updated: 2024/01/22 11:14:02 by kelmouto         ###   ########.fr       */
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
    
}
void Client::processChunk(size_t startIndex)
{
    std::string line = body.substr(0, startIndex);
    size_t i = startIndex + 2;
    chunkSize = std::stoi(line, NULL, 16);
    if (chunkSize == 0)
        WriteResponse();
    if (body.size() >= i + 2 + chunkSize)
    {
        Out->write(body.substr(i, chunkSize).c_str(), chunkSize);
        body.erase(0, i + 2 + chunkSize);
        chunked = -1;
    }
    else
        chunked = 0;
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

    Out->open(filename, std::ios::out | std::ios::trunc | std::ios::binary);
    if(!Out->is_open())
        throw std::runtime_error("Couldn't open file ");
}
void Client:: WriteResponse()
{
    std::string tmp;
    tmp = M_U_V[2] + " 200 OK\r\nContent-Type: text/html"+"\r\n\r\nstatus: success\n message: File successfully uploaded\r\n";
    write(fd,tmp.c_str(),tmp.length());
    throw std::runtime_error("");
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
                WriteResponse();
            }
            total = read(fd,Store,BUFFER_SIZE - 1);
            if (total > 0 )
            {
                count += total;
                Store[total] = '\0';
                body.insert(body.end(),Store, Store + total);
                PostMethod();
                if(count >= content_length)
                    WriteResponse();
            }
        }
        else if(header.find("Transfer-Encoding") != header.end())
        {
            if (flag == false)
            {
                OpeningFile();
                flag = true;
            }
            size_t      i;
            std::string line;
            if(header["Transfer-Encoding"] == "chunked")
            {
                i = body.find("\r\n");
                if (i == std::string::npos && chunked == -1)
                {
                    total = read(fd, Store, BUFFER_SIZE - 1);
                    if (total > 0)
                    {
                        Store[total] = '\0';
                        body.insert(body.end(), Store, Store + total);
                        i = body.find("\r\n");
                        processChunk(i);
                    }
                }
                else if (chunked == 0 && i != std::string::npos)
                {
                    total = read(fd, Store, BUFFER_SIZE - 1);
                    Store[total] = '\0';
                    body.insert(body.end(), Store, Store + total);
                    if (body.size() >= i  + 4 + chunkSize)
                    {
                        Out->write(body.substr(i + 2, chunkSize).c_str(), chunkSize);
                        body.erase(0, i + 4 + chunkSize);
                        chunked = -1;
                    } 
                    else
                        chunked = 0;
                }
                else if (chunked == -1 && i != std::string::npos)
                    processChunk(i);
            }  
        }
}
    else
        ServeError("403", " Forbidden\r\n");
}