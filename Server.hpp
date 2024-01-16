/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kelmouto <kelmouto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 11:48:52 by kelmouto          #+#    #+#             */
/*   Updated: 2024/01/16 11:53:28 by kelmouto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include<iostream>
#include<map>
#include<sstream>
#include<string>
#include<fstream>
#include<vector>
#include"Config.hpp"
#include "Client.hpp"
class WebServer;

class Location
{
  public:
  Location()
  {
    root = "";
  }
     std::string root;
    std::vector<std::string> index;
    std::map<int,std::string> Return;
    std::vector<std::string> uploads;
};
 
class Server
{
  public:
    void AddNewClient(fd_set *FdRd, fd_set *FdWr);
    void run(WebServer & web);
    Server();
    void CreationBindListen();
    std::map<std::string, std::string> err;
    std::vector<Client> client;
    int fd;
    int port;
    static std::map<std::string,std::vector<std::string> >mimeTypes;
    std::vector<std::string> serverBlock;
    std::string ipAdress;
    std::vector<std::string> serverName;
    std::string root;
    std::vector<std::string> index;
    bool autoindex;
    size_t client_body_timeout;
    size_t client_max_body_size;
    std::map<std::string, std::string> errorPages;
    std::map<std::string,Location> locations;//class location
    std::string parslocation(std::string v);
    std::map<std::string, bool>allow_methods;
    Location  buildClass(std::string v);
    void setupglobalroot(std::map<std::string,Location> v);
    void  funcMimeTypes(std::string filename);
 };

int findEndofBlock(const std::string& conf, int start);
std::string affect(std::vector<std::string>::iterator it,std::vector<std::string>::iterator e);