/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kelmouto <kelmouto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 11:48:52 by kelmouto          #+#    #+#             */
/*   Updated: 2024/02/16 11:38:43 by kelmouto         ###   ########.fr       */
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
    flagg = true;
    auflag = true;
    allow = true;
    autoindex = 0;
    allow_methods["GET"] = 0;
    allow_methods["POST"] = 0;
    allow_methods["DELETE"] = 0;
  }
    std::string root;
    bool flagg;
    bool auflag ;
    bool allow;
    std::string name;
    std::vector<std::string> index;
    std::map<int,std::string> Return;
    std::vector<std::string> uploads;
     std::map<std::string, bool>allow_methods;
    bool autoindex;
    std::map<std::string,std::string> cgi;
    void fillLocation(std::vector<std::string>::iterator& it,std::vector<std::string>::iterator e);
};
 
class Server
{
  public:
    Server();
    bool clbody;
    bool lflag;
    bool rodirective;
    void fillErrIndexAllofunc(std::vector<std::string>::iterator& it,std::vector<std::string>::iterator e);
    void fillTimeSizeClient(std::vector<std::string>::iterator& it,std::vector<std::string>::iterator e);
    void setupglobalroot(std::map<std::string,Location> &v);
    int AddNewClient();
    static void  funcMimeTypes(std::string filename);
    std::string parslocation(std::string v);
    Location  buildClass(std::string v);
    void run(WebServer & web);
    void CreationBindListen();
    static std::map<std::string,std::vector<std::string> >mimeTypes;
    std::map<std::string, std::string> errorPages;
    std::map<std::string, std::string> err;
    std::map<std::string,Location> locations;//class location
    std::map<std::string, bool>allow_methods;
    std::vector<std::string> serverBlock;
    std::vector<std::string> index;
    size_t client_max_body_size;
    std::vector<Client> client;
    size_t client_body_timeout;
    std::string ipAdress;
    std::vector<std::string> serverName;
    std::string root;
    std::vector<Server> Servs;
    std::string host;
    int port;
    int fd;
    bool flaag;
    void parseListen(std::string content);
    bool Pathvalide(std::string s, int mode);
    void defaultErrorPages();
    void  check_duplicate(std::string s);

 };

int findEndofBlock(const std::string& conf, int start);
std::string affect(std::vector<std::string>::iterator &it,std::vector<std::string>::iterator e);
std::vector<std::string> fillhelper(std::string v);
