/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kelmouto <kelmouto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 09:31:04 by kelmouto          #+#    #+#             */
/*   Updated: 2024/01/04 13:19:31 by kelmouto         ###   ########.fr       */
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
class Config
{
    private:
       std::string myConfig;
       std::vector<std::string > ConfigServer;
        std::vector<std::vector<std::string> > vectofServers;
    public:
        std::string  ReadFile(std::string s);
        std::string TrimSpace(std::string d);
        std::string  removeComent(std::string c);
        std::vector<std::string> fillServervect(int start, int end, std::string conf);
        std::vector<std::vector<std::string> >splitServers(std::string conf,int nb);
};






#endif