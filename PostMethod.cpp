/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostMethod.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajari <ajari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 18:58:37 by kelmouto          #+#    #+#             */
/*   Updated: 2024/02/16 22:55:57 by ajari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "WebServer.hpp"
#include "InFile.hpp"
#include "Server.hpp"
#include <cstdlib>
#include<fcntl.h>

void  Client::fileExists(const std::string filePath) 
{
    struct stat st;
    if (stat(filePath.c_str(), &st) == 0 )
    {
        if (!S_ISDIR(st.st_mode) || access(filePath.c_str(), R_OK | W_OK) == -1)
            ServeError("500", " Internal Server Error\r\n");
    }
    else
        ServeError("500", " Internal Server Error\r\n");
}
void  Client::PostMethod()
{
    Out->write(body.c_str(),body.size());
    body.clear();
}
void Client::ChunckedMethod(ssize_t total)
{   
    size_t      i;
    std::string line;
    char Store[BUFFER_SIZE];
    i = body.find("\r\n");
    if (i == std::string::npos && chunked == -1)
    {
        if(Read)
        {
            total = OurRD(Store,BUFFER_SIZE - 1);
            timing = clock();
            if (total > 0)
            {
                Store[total] = '\0';
                body.insert(body.end(), Store, Store + total);
                i = body.find("\r\n");
                processChunk(i);
            }
        }
        else
            checktimeout();
    }
    else if (chunked == 0 && i != std::string::npos)
    {
        if(Read)
        {
            total = OurRD(Store,BUFFER_SIZE - 1);
            timing = clock();
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
        else
            checktimeout();
       
    }
    else if (chunked == -1 && i != std::string::npos)
        processChunk(i);
}

void Client::processChunk(size_t startIndex)
{
    std::string line = body.substr(0, startIndex);
    size_t i = startIndex + 2;
    char *endPtr;
    chunkSize = strtol(line.c_str(), &endPtr, 16);
    if (*endPtr !='\0') 
    {
        std::remove(filename.c_str());
        ServeError("400", " Bad Request\r\n");
    }
    sum+= chunkSize;
    if(sum > Serv->client_max_body_size)
    {
        std::remove(filename.c_str());
        ServeError("413"," Request Entity Too Large\r\n");
    }
    if (chunkSize == 0)
        WriteResponse();
    if (body.size() >= i + 2 + chunkSize)
    {
        Out->write(body.substr(i, chunkSize)
        .c_str(), chunkSize);
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

bool check_file_exists(const std::string& file_path) 
{
    std::ifstream file(file_path.c_str());
    if(file.is_open())
    {
        file.close();
        return(true);
    }
    else 
        return false;
}

void Client::OpeningFile()
{
    std::string t;
    t = findExtension(header["content-type"]);
    if(t == "")
        t = "txt";
    std::srand(time(NULL));
    std::stringstream ss;
    ss << std::rand() % 1000 + 1;
    std::string name  = ss.str() + "." + t; 
    filename = *(location->uploads.begin() + 1);
    fileExists(filename);
    if(filename[filename.length() - 1] != '/')
        filename +=("/" + name );
    else
        filename += name;
    if(check_file_exists(filename))
        filename = filename.substr(0,filename.length()- t.length()  - 1) + "1." + t;
    Out->open(filename.c_str(), std::ios::in |std::ios::out | std::ios::trunc | std::ios::binary);
    if(!Out->is_open())
        throw std::runtime_error("Couldn't open file ");
}

void Client:: WriteResponse()
{
    handlCgi(filename,1);
    if(timeout)
        return;
    std::string tmp;
    tmp = M_U_V[2] + " 201 Created\r\nContent-Type: text/html"+"\r\n\r\nstatus: success\n message: File successfully uploaded\r\npath:" + filename;
    OurWR(tmp.c_str(),tmp.size());
    throw std::runtime_error("posted");
}

void Client::checktimeout()
{
    if((clock() - timing) / CLOCKS_PER_SEC > 10)
    {   
        std::remove(filename.c_str());
        ServeError("408", " Request Timeout\r\n");
    }
}

void Client::PostMethodfunc()
{

    char Store[BUFFER_SIZE];
    ssize_t total = 0;
    size_t content_length;
    if(location->uploads.size() >= 2 && *(location->uploads.begin()) == "on")
    {
        if(header["content-type"].find("boundary") !=std::string::npos  && PathCGI.empty())
            ServeError("501"," Not Implemented\r\n");
        std::map<std::string,std::string>::iterator it = header.find("content-length");
        if(it != header.end() && header["content-length"] != "" )
        {

            if(check == -1)
            {
                OpeningFile();
                count = 0;
                check = 0;
            }
            std::istringstream a(header["content-length"]);
            a >> content_length;
            if (a.fail() || a .bad())
                ServeError("400"," Bad Request\r\n");
            if(content_length > Serv->client_max_body_size)
            {
                std::remove(filename.c_str());
                ServeError("413"," Request Entity Too Large\r\n");
            }
            if(body.size() >= (size_t)content_length)
            {
                Out->write(body.c_str(),content_length);
                body.clear();
                WriteResponse();
                if(timeout)
                    return;
            }
            if(Read)
            {
                total = OurRD(Store,BUFFER_SIZE - 1);
                timing = clock();
                if (total > 0 )
                {
                    Store[total] = '\0';
                    body.insert(body.end(),Store, Store + total);
                    size_t nn = content_length - body.size();
                    if (nn >= 0)
                    {
                        Out->write(body.c_str(),body.size());
                        count += body.size();
                    }
                    else 
                    {
                        Out->write(body.c_str(),nn);
                        count += nn;
                    }
                    body.clear();
                    if(count >= content_length)
                        WriteResponse();


                }
            }
            else
                checktimeout();
        }
        else if(header.find("transfer-encoding") != header.end())
        {
            if (flag == false)
            {
                OpeningFile();
                flag = true;
            }
            if(header["transfer-encoding"] == "chunked")
                ChunckedMethod(total);
            else
            {
                std::remove(filename.c_str());
                ServeError("400"," Bad Request\r\n");
            }
        }
        else 
            ServeError("411"," Length Required\r\n");
    }
    else
        ServeError("403", " Forbidden\r\n");
}