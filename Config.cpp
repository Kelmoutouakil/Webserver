/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kelmouto <kelmouto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 09:36:08 by kelmouto          #+#    #+#             */
/*   Updated: 2024/01/11 14:14:27 by kelmouto         ###   ########.fr       */
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

std::string affect(std::vector<std::string>::iterator it,std::vector<std::string>::iterator e)
{
            std::string ptr;
            it++;
            while(it != e && *it != ";")
            {
                ptr += *it;
                it++;
            }
            return(ptr);
}

bool validIpAdress(std::string IpAdress)
{
    std::istringstream ss(IpAdress);
    std::string part;
    int count = 0;
    while(std::getline(ss,part,'.'))
    {
        int nb = std::stoi(part);
        if(nb < 0 || nb > 255)
            return false;
        count++;
    }
    return(count == 4);
}

bool validPort(std::string PortString)
{
    int nb = std::stoi(PortString);
    if(nb < 0 || nb >= 65535)
        return(false);
    else
        return(true); 
}

void parseListen(std::string content,Server& o)
{
    std::istringstream ss(content);
    std::string Ipadress;
    std::string PortString;
    std::getline(ss,Ipadress,':');
    std::getline(ss,PortString);
    if(!ss.fail() && validIpAdress(Ipadress) && validPort(PortString))
    { 
        o.port = PortString;
        o.ipAdress = Ipadress;
    }
    else if(ss.fail() || !validIpAdress(Ipadress) || !validPort(PortString))
        throw std::runtime_error("Error in listen directive");
}

Server  Config::fillServervect(int start, int end, std::string conf)
{
    Server o;
    conf = o.parslocation(conf);
 
    std::stringstream serverBlock (conf.substr(start + 1, end - start - 1));
    std::vector<std::string> serverBlockLines;
    std::string word;
    while(serverBlock >> word)
    {
        if(word[word.length() - 1]== ';')
        {
            serverBlockLines.push_back(word.substr(0,word.length() - 1));
            serverBlockLines.push_back(";");
        }
        else
            serverBlockLines.push_back(word);
    }
    std::vector<std::string> ::iterator it = serverBlockLines.begin();
    for(;it != serverBlockLines.end();it++)
    {
        if(*it == "listen")
            parseListen(*(++it),o);
        if(*it =="root") 
        {
            o.root = affect(it,serverBlockLines.end());
            o.setupglobalroot(o.locations);
        }
        if(*it == "server_name")
        {
            it++;
            o.serverName.clear();
            while(*it != ";" && it != serverBlockLines.end())
            {
                o.serverName.push_back(*it);
                it++;
            }
        }
        if(*it == "client_body_timeout")
            o.client_body_timeout = atoi((*(++it)).c_str());
        if(*it == "client_max_body_size")
            o.client_max_body_size =atoi((*(++it)).c_str());
        if(*it == "index")
        {
            it++;
            o.index.clear();
            while(*it != ";" && it != serverBlockLines.end())
            {
                o.index.push_back(*it);
                it++;
            }
        }
        if(*it == "autoindex")
        {
            it++;
            if(*it == "off")
                o.autoindex = false;
            else if(*it == "on")
                o.autoindex = true;
        }
        // if(*it == "uploads")
        // {
        //     it++;
        //     if(*it == "off")
        //         o.uploads= false;
        //     else if(*it == "on")
        //         o.uploads = true;
        // }
        if(*it == "allow_methods")
        {
            it++;
            while(it != serverBlockLines.end() && *it != ";" )
            {
                if(*it == "GET")
                    o.allow_methods[*it] = true;
                if(*it == "POST")
                    o.allow_methods[*it] = true;
                if(*it == "DELETE")
                    o.allow_methods[*it] = true;
                it++;
            }
        }
    }
    if(o.root == "" || o.port == "" || o.ipAdress == "")
        throw std::runtime_error("at least root and listen  should be setup");
    return o;
}

std::vector<Server>  Config:: splitServers(std::string conf,int nb)
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
    vectofServers.push_back(fillServervect(start,end,conf));
    if(conf.substr(end) != "\0")
        splitServers(conf.substr(end + 1),nb);    
    return vectofServers;
}
