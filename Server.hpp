/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kelmouto <kelmouto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 11:48:52 by kelmouto          #+#    #+#             */
/*   Updated: 2024/01/10 10:34:08 by kelmouto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP
#include<iostream>
#include<map>
#include<sstream>
#include<string>
#include<fstream>
#include<vector>
#include"Config.hpp"
class Location
{
  public:
     std::string root;
    std::vector<std::string> index;
    bool autoindex;
    std::map<int,std::string> Return;
    bool uploads;
};
 class Server
 {
   public:
   Server()
   {
    serverName = "Default";
    index.push_back("index.html");
    autoindex = 0;
    uploads = 0;
    allow_methods["GET"] = 0;
    allow_methods["POST"] = 0;
    allow_methods["DELETE"] = 0;
    client_body_timeout = "10s";
   }
    std::vector<std::string> serverBlock;
    std::string listen;
    std::string serverName;
    std::string root;
     std::vector<std::string> index; 
   bool autoindex;
   bool uploads;
    std::string client_body_timeout;
    std::string client_max_body_size;
    std::map<std::string, std::string> errorPages;
   std::map<std::string,Location> locations;//class location
   std::string parslocation(std::string v);
   std::map<std::string, bool>allow_methods;
std::map<int,std::string> Return;
 };
int findEndofBlock(const std::string& conf, int start);
#endif