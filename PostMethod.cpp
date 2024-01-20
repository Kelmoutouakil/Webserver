/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostMethod.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kelmouto <kelmouto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 18:58:37 by kelmouto          #+#    #+#             */
/*   Updated: 2024/01/20 18:25:36 by kelmouto         ###   ########.fr       */
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
    
    
    // std::string line;
    // size_t i;
    
    // size_t chunkSize;
    // int j = body.size();
    // if (j > 1 && body.substr(j - 2, 2) != "\r\n")
    // {
    //     while (j > 1 && body[j - 1] != '\r' && body[j - 2] != '\n')
    //         j--;
    //     reserve = body.substr(j - 1);
    //     body.erase(j - 1);
    //     // std::cout << "***************************\n";
    //     // std::cout<<"--->" <<reserve<<"<--------\n";
    //     // std::cout << "***************************\n";
    //     // std::cout << "***************************\n";
    //     // std::cout<<"--->" <<body<<"<--------\n";
    //     // std::cout << "***************************\n";
    //     size_t len = body.length();
        
    //     while(len > 0)
    //     { 
    //         i = body.find("\r\n");
    //         line = body.substr(0,i); 
    //         i+= 2;
    //         chunkSize = std::stoi(line, NULL, 16);
    //         std::cout << "chunk : " << chunkSize << "\n";
    //         if(i ==  body.size())
    //             break;
    //         if (chunkSize == 0)
    //             break;
    //         if(i ==  body.size())
    //             break;
    //         *Out<< body.substr(i ,chunkSize);;
    //         body.erase(0, i  + ++chunkSize);
    //          std::cout << "len : "<<  (i + 2 + chunkSize)<< "\n"; 
    //         std::cout << "***************************\n";
    //         len -= (i + 2 + chunkSize);
           
    //     }
    //     // if(chunkSize == 0)
    //         throw std::runtime_error("");
    // }
    // else
    // {
    //     size_t len = body.size();
    //     size_t totalSize = BUFFER_SIZE;
    //     while(len > 0 &&  totalSize > 0)
    //     { 
            
    //         i = body.find("\r\n");
    //         line = body.substr(0,i);
    //         i+= 2;
    //         chunkSize = std::stoi(line, NULL, 16);
    //         if (chunkSize == 0)
    //          break;
    //         *Out<< body.substr(i ,chunkSize);;
    //         body.erase(0, i  + 2+ chunkSize);
    //         len -= i + 2 + chunkSize;
    //         totalSize -= i + 2 + chunkSize;
    //     }
    // }
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
            if (flag == false)
            {
                OpeningFile();
                flag = true;
            }
            size_t i ;
            std::string line ;
            size_t len;
            if(header["Transfer-Encoding"] == "chunked")
            {
                if(chunked == -1)
                {
                len = body.size();
                i = body.find("\r\n");
                line = body.substr(0,i);
                i+= 2;
                chunkSize = std::stoi(line, NULL, 16);
                if(chunkSize == 0)
                    throw std::runtime_error("");
                if(len  <= chunkSize)
                {
                    total = read(fd,Store,BUFFER_SIZE - 1);
                    if(total > 0)
                    {
                        Store[total] = '\0';
                        body.insert(body.end(),Store, Store + total);
                        if(body.size() >= chunkSize)
                        {
                            *Out << body.substr(i,chunkSize);
                            body.erase(0, i  + 2 + chunkSize);
                        }
                        else
                            chunked = 0;
                    }
                }
                else
                {
                    *Out << body.substr(i,chunkSize);
                    body.erase(i,chunkSize);
                }
            }
            else 
            {
                total = read(fd,Store,BUFFER_SIZE - 1);
                Store[total] = '\0';
                body.insert(body.end(),Store, Store + total);
                std::cout << body.size()<< " body size : \n";
                if(body.size() >= chunkSize)
                {
                    i = body.find("\r\n") + 2;
                    Out->write((body.substr(i,chunkSize)).c_str(),(body.substr(i,chunkSize)).size());
                    //*Out << std::flush;
                    body.erase(0,i + chunkSize +  2);
                    chunked = -1;
                }
                else
                    chunked = 0;
            }
                
            }
        }
    }
    else
        ServeError("403", " Forbidden\r\n");
}