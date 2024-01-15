/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kelmouto <kelmouto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 13:53:46 by kelmouto          #+#    #+#             */
/*   Updated: 2024/01/12 17:35:52 by kelmouto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "WebServer.hpp"
#include<algorithm>
#include<iterator>

Server::Server()
{
    root = "";
    port = -1;
    fd = -1;
    ipAdress= "";
    serverName.push_back("Default");
    index.push_back("index.html");
    autoindex = 0;
    //uploads = 0;
    allow_methods["GET"] = 0;
    allow_methods["POST"] = 0;
    allow_methods["DELETE"] = 0;
    client_body_timeout = 10;
}

void    Server::CreationBindListen()
{
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        std::cerr << "Error creating socket" << std::endl;
        exit(1) ;
    }
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(port);
    int reuse = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1) {
        perror("setsockopt"); 
        close(fd);
        exit(EXIT_FAILURE);
    }
    if (bind(fd, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        std::cerr << errno << std::endl;
        close(fd);
        exit(-1);
    }
    if (listen(fd, 5) == -1) {
        std::cerr << "Error listening for connections" << std::endl;
        close(fd);
        exit(-1);
    }
}

Location  Server::buildClass(std::string v)
{
    Location o;
    std::stringstream ss(v);
    std::string word;
    size_t i = 0;

    std::vector<std::string>helper;
    while(ss >> word)
    {
      i = word.length();
        if(word[i - 1] == ';')
        {
            helper.push_back(word.substr(0,i - 1));
            helper.push_back(";");
        }
        helper.push_back(word);
    }
    std::vector<std::string>::iterator it = helper.begin();
    for(;it != helper.end();it++)
    {
        if(*it == "root")
        {
            it++;
            while(*it != ";" && it != helper.end())
            {
                o.root += *it;
                it++;
            }
        }
        if(*it == "index")
        {
            it++;
            while(*it != ";" && it != helper.end())
            {
                o.index.push_back(*it);
                it++;
            }
        }
        if(*it == "uploads")
        {
            it++;
            while(*it != ";" && it != helper.end())
            {
                o.uploads.push_back(*it);
                it++;
            }
        }
        if(*it == "return")
        {
            it++;
            while(*(it)!= ";" &&  (it + 2) != helper.end())
            {
                o.Return.insert(std::make_pair(atoi((*it).c_str()),*(it + 1)));
                it+= 2;
            }
        }
    }
    return o;
}

std::string Server::parslocation(std::string v)
{
    Location o;
    std::string name;
    size_t found = v.find("location");
    size_t i = 0;
    while (found != std::string::npos && i < v.size()) 
    {
        i = found + 8;
        while (v[i] && (v[i] == ' ' || v[i] == '\t'))
            i++;
        name.clear();  
        while (v[i] != '{')
            name.push_back(v[i++]);
        int end = findEndofBlock(v, i + 1);
        if (end == -1)
            throw std::runtime_error(" block location error in {}");
        locations.insert(std::make_pair(name, buildClass(v.substr(i + 1, end - i - 1))));
        v.erase(found, end - found + 1);
        found = v.find("location", found + 1);
    }
    return v;
}

void Server::setupglobalroot(std::map<std::string,Location> v)
{
    std::map<std::string,Location>::iterator it = v.begin();
    for(;it!= v.end();it++)
    {
        if(it->second.root == "")
            it->second.root = this->root;
    }
    
}
///// ajari

void Server::AddNewClient(fd_set *FdRd, fd_set *FdWr)
{
    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);
    int fd_socket = accept(fd, (struct sockaddr*)&client_address, &client_address_len);
    if (fd_socket == -1) {
        std::cerr << "Error accepting connection" << std::endl;
        return;
    }
    client.push_back(Client(fd_socket, this));
    FD_SET(fd_socket, FdRd);
    FD_SET(fd_socket, FdWr);
}

void Server::run(WebServer & web)
{
    size_t i;
    if (fd == -1)
    {
        CreationBindListen();
        std::cout << "hello world \n";
    }
    try
    {
        std::cout << "---->\n";
        int maxFd = fd;
        FD_ZERO(&web.FdRd);
        FD_ZERO(&web.FdWr);
        FD_SET(fd, &web.FdRd);
        for (i = 0; i < client.size(); i++)
        {
            (client[i].fd > maxFd) && (maxFd = client[i].fd); 
            FD_SET(client[i].fd, &web.FdRd);
            FD_SET(client[i].fd, &web.FdWr);
        }
        std::cout << "---->\n";
        if (select(maxFd + 1, &web.FdRd, &web.FdWr, NULL, NULL) < 0) 
            exit(EXIT_FAILURE);
        std::cout << "---->\n";
        if (FD_ISSET(fd, &web.FdRd))
            AddNewClient(&web.FdRd, &web.FdWr);
        for (i = 0; i < client.size(); i++)
            client[i].handleRequest(&web.FdRd, &web.FdWr);
    }
    catch(std::exception& e)
    {
        std::cerr << "\33[1;31m" << e.what() << "\33[0m\n";
    }
    std::cout << "out\n";
    // usleep(1000000);

}