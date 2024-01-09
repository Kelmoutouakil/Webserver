/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kelmouto <kelmouto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 11:48:52 by kelmouto          #+#    #+#             */
/*   Updated: 2024/01/09 10:25:37 by kelmouto         ###   ########.fr       */
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
    std::string index;
    std::string autoindex;
    std::string Return;
    std::string  uploads;
};
 class Server
 {
   public:
    std::vector<std::string> serverBlock;
    std::string listen;
    std::string serverName;
    std::string root;
    std::string index;
    std::string autoindex;
    std::map<std::string, std::string> errorPages;
   std::map<std::string,Location> locations;//class location
   void parslocation(std::string v);
   std::map<std::string, bool>allow_methods;
   std::string getListen();
   std::string getServerName();
   std::string getRoot();
   std::string getIndex();
   std::string getAutoindex();
   std::map<std::string,std::string> getErrorPages();
   std::map<std::string, bool>getAllow_methods();
 
 };
int findEndofBlock(const std::string& conf, int start);
#endif