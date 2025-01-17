/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kelmouto <kelmouto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 09:31:04 by kelmouto          #+#    #+#             */
/*   Updated: 2024/01/29 10:56:45 by kelmouto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
#define CONFIG_HPP
#include<iostream>
#include<map>
#include<sstream>
#include<string>
#include<fstream>
#include<vector>
#include<exception>

class Server;
class Config
{
    private:
       std::string myConfig;
       std::vector<std::string > ConfigServer;
        std::vector<Server> vectofServers;
    public:
        std::string  ReadFile(std::string s);
        std::string TrimSpace(std::string d);
        std::string  removeComent(std::string c);
        Server fillServervect(int start, int end, std::string conf);
        std::vector<Server> splitServers(std::string conf,int nb);
        void checkServername(std::string& t);
       
};
#endif
