/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kelmouto <kelmouto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 09:36:08 by kelmouto          #+#    #+#             */
/*   Updated: 2024/02/16 18:10:13 by kelmouto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include "Server.hpp"

std::string Config:: ReadFile(std::string s)
{
        std::ifstream buffer(s.c_str());
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
        size_t end  = c.find('\n',pos);
        c.erase(pos,end - pos);
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

bool validIpAdress(std::string IpAdress)
{
    int nb;
    std::istringstream ss(IpAdress);
    std::string part;
    int count = 0;
    if(IpAdress == "localhost")
        return(true);
    while(std::getline(ss,part,'.'))
    {
        if (part.find_first_not_of("0123456789") != std::string::npos)
            throw std::runtime_error("invalid ip address");
        std::istringstream a(part);
            a >> nb;
        if(nb < 0 || nb > 255)
            return false;
        count++;
    }
    return(count == 4);
}

bool validPort(std::string PortString)
{
    int nb;
    if (PortString.find_first_not_of("0123456789") != std::string::npos)
        return (false);
    std::istringstream a(PortString);
    a >> nb;
    if(nb < 0 || nb >= 65535)
        return(false);
    else
        return(true); 
}

void Server:: parseListen(std::string content)
{
    std::istringstream ss(content);
    std::string Ipadress;
    std::string ip ="";
    int p = -1;
    std::string PortString;
    if(flaag == true)
    {
        ip = ipAdress;
        p = port;  
    }
    
    std::getline(ss,Ipadress,':');
    std::getline(ss,PortString);
    if(!ss.fail() && validIpAdress(Ipadress) && validPort(PortString))
    {
        if(Ipadress == "localhost")
           ipAdress = "127.0.0.1";
        else
            ipAdress = Ipadress;
        std::istringstream a(PortString);
            a >> port;
        flaag = true;
    }
    else if(ss.fail() || !validIpAdress(Ipadress) || !validPort(PortString))
        throw std::runtime_error("Error in listen directive");
    if(ip != "" || p != -1)
    {
        if(p == port || ipAdress == ip)
            throw std::runtime_error(" duplicate listen directive");
    }
}

void    Server:: defaultErrorPages()
{
    errorPages["400"] = "./error_pages/400.html";
    errorPages["300"] = "./error_pages/300.html";
    errorPages["204"] = "./error_pages/204.html";
    errorPages["403"] = "./error_pages/403.html";
    errorPages["404"] = "./error_pages/404.html";
    errorPages["405"] = "./error_pages/405.html";
    errorPages["408"] = "./error_pages/408.html";
    errorPages["411"] = "./error_pages/411.html";
    errorPages["413"] = "./error_pages/413.html";
    errorPages["414"] = "./error_pages/414.html";
    errorPages["500"] = "./error_pages/500.html";
    errorPages["501"] = "./error_pages/501.html";
    errorPages["502"] = "./error_pages/502.html";
    errorPages["505"] = "./error_pages/505.html";
    errorPages["504"] = "./error_pages/504.html";
}

void Config::checkServername(std::string& t)
{
    for (std::vector<Server>::iterator it = vectofServers.begin(); it != vectofServers.end(); ++it)
    {
        for (std::vector<std::string>::iterator ite = it->serverName.begin(); ite != it->serverName.end(); ++ite)
        {
            if (*ite == t)
                throw std::runtime_error("Servername duplicated");
        }
    }
}

Server  Config::fillServervect(int start, int end, std::string conf)
{
    Server o;
    o.defaultErrorPages(); 
    std::string s = conf.substr(start + 1, end - start - 2 );
    s = o.parslocation(s);
    std::vector<std::string> serverBlockLines =fillhelper(s);
    std::vector<std::string> ::iterator it = serverBlockLines.begin();
    for(;it != serverBlockLines.end();it++)
    {
        o.fillTimeSizeClient(it,serverBlockLines.end());
        o.fillErrIndexAllofunc(it,serverBlockLines.end());
        if(*it == "listen")
        {
            if((it + 1 )!= serverBlockLines.end())
                it++;
            if(o.lflag)
            {
                o.lflag = false;
                o.parseListen(*it);
                it++;
            }
            else
                throw std::runtime_error("listen directive is duplicate");
            
        }
        if(*it =="root") 
        {
            if(o.rodirective)
            {
                o.rodirective = false;
                o.root = affect(it,serverBlockLines.end());
                o.setupglobalroot(o.locations);
            }
            else
                throw std::runtime_error(" root directive duplicated");
            
        }
        if(*it == "server_name")
        {
            if((it + 1 )!= serverBlockLines.end())
                it++;
            while(*it != ";" && it != serverBlockLines.end())
            {
                checkServername(*it);
                o.serverName.push_back(*it);
                it++;
            }
        }
        if(*it != ";")
                throw std::runtime_error("fausse directive");
    }
   
    if(o.root == "")
        throw std::runtime_error("at least root should be setup");
    return o;
}

std::vector<Server>  Config:: splitServers(std::string conf,int nb)
{
    conf = removeComent(conf);
    conf = TrimSpace(conf);
    size_t i = 0;
    while (conf[i] && conf[i] != 's') 
    {
        if (!isspace(conf[i]) && conf[i] != '\n')
            throw std::runtime_error("error in input of configfile");
        i++;
    }
    std::string tmp;
    tmp = conf.substr(i,6);
    if(tmp == "\0" && nb > 0)
        return vectofServers;
    else if(tmp != "server")
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
    Server ser = fillServervect(start,end + 1,conf);
    vectofServers.push_back(ser);
    if(conf.substr(end + 1) != "\0")
        splitServers(conf.substr(end + 1),nb);
    return vectofServers;
}
