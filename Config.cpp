/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kelmouto <kelmouto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 09:36:08 by kelmouto          #+#    #+#             */
/*   Updated: 2024/01/10 10:27:10 by kelmouto         ###   ########.fr       */
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
        {
            it++;
            if(*(it + 1) != ";")
                std::runtime_error("Error in listen port");
            else
                o.listen = *it; 
        }
        if(*it =="root") 
            o.root = affect(it,serverBlockLines.end());
        if(*it == "server_name")
            o.serverName = affect(it,serverBlockLines.end());
        if(*it == "client_body_timeout")
            o.client_body_timeout = affect(it,serverBlockLines.end());
        if(*it == "client_max_body_size")
            o.client_max_body_size = affect(it,serverBlockLines.end());
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
        if(*it == "uploads")
        {
            it++;
            if(*it == "off")
                o.uploads= false;
            else if(*it == "on")
                o.uploads = true;
        }
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
        if(*it == "return")
        {
            it++;
            while(*(it)!= ";" &&  (it + 2) != serverBlockLines.end())
            {
                o.Return.insert(std::make_pair(atoi((*it).c_str()),*(it + 1)));
                it+= 2;
            }
        }
    }
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
