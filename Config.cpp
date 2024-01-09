/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kelmouto <kelmouto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 09:36:08 by kelmouto          #+#    #+#             */
/*   Updated: 2024/01/09 09:58:38 by kelmouto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include "Server.hpp"

std::string Config:: ReadFile(std::string s)
{
        std::ifstream buffer(s);
        std::stringstream configfile;

        if (!buffer.is_open())
            throw std::runtime_error("Couldn't open file: " + s);

        configfile << buffer.rdbuf();
        buffer.close();

        myConfig = configfile.str();
        return myConfig;
}

std::string Config:: TrimSpace(std::string d)
{
    int i = 0;
    while(d[i] && isspace(d[i]))
        i++;
    d = d.substr(i);
    i = d.length();
    while(d[i] && isspace(d[i]))
        i--;
    d = d.substr(0,i + 1);
    return(d);
}

std::string Config:: removeComent(std::string c)
{
    size_t pos = c.find('#');
    while(pos != std::string::npos)
    {
        size_t end  = c.find('\n',pos + 1);
        c.erase(pos,end);
        pos = c.find('#');
    }
    return(c);
}
int findEndofBlock(const std::string& conf, int start)
 {
    int count = 0;
    while ((size_t)start < conf.length()) 
    {
        if (conf[start] == '{') 
            count++;
        else if (conf[start] == '}' && count == 0) 
            return start;
        else if (conf[start] == '}') 
            count--;
        start++;
    }
    return -1;  
}

std::vector<std::string>  Config::fillServervect(int start, int end, std::string conf)
{
    std::stringstream serverBlock (conf.substr(start + 1, end - start - 1));
   // std::string serverBlock (conf.substr(start + 1, end - start - 1));

    std::vector<std::string> serverBlockLines;
    std::string word;
    while(serverBlock >> word)
    {
        if(word[word.length() - 1]== ';')
        {
            serverBlockLines.push_back(word.substr(0,word.length() - 1));
            serverBlockLines.push_back(";");
        }
        // if(word == "location")
        serverBlockLines.push_back(word);
        
    }
//     int i = 0;
//    std::string temp;
//     while(serverBlock[i])
//     {
//         if(serverBlock[i] == ' ')
//         {
//             serverBlockLines.push_back(temp);
//             temp = "";
//             while(serverBlock[i]== ' ')
//                 i++;
//         }
//         if(serverBlock[i] == ';')
//         {
//             serverBlockLines.push_back(temp);
//             serverBlockLines.push_back(";");
//             temp = "";
//             i++;
//         }
//         if(serverBlock[i] == '\n')
//         {
//             i++;
//             while(serverBlock[i]== ' ' || serverBlock[i] == '\n')
//                 i++;
//             continue;
//         }
//         temp.push_back(serverBlock[i]);
//         i++;
//     }
    return serverBlockLines;
}

std::vector<std::vector<std::string> > Config::splitServers(std::string conf,int nb)
{
    conf = removeComent(conf);
    conf = TrimSpace(conf);
    size_t i = 0;
    while (conf[i] && conf[i] != 's') 
    {
        if (!isspace(conf[i])) 
            throw std::runtime_error("error in input of configfile");
        i++;
    }
    std::string tmp;
    tmp = conf.substr(i,6);
    if(tmp != "server" && nb > 0)
        return vectofServers;
    else if(tmp != "server" && nb == 0)
        throw std::runtime_error("bloc of server not found");
    i += 6;
    nb++;
    while(conf[i] && conf[i] != '{')
    {
        if (!isspace(conf[i])) 
            throw std::runtime_error("error in {} of configfile");
        i++; 
    }
    int start = i;
    int end = findEndofBlock(conf ,i + 1);
    if(end == - 1)
        throw std::runtime_error("error in { }");
    ConfigServer = fillServervect(start,end,conf);
    vectofServers.push_back(ConfigServer);
    if(conf.substr(end) != "\0")
        splitServers(conf.substr(end + 1),nb);    
    return vectofServers;
}
