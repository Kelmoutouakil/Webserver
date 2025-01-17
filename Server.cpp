/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kelmouto <kelmouto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 13:53:46 by kelmouto          #+#    #+#             */
/*   Updated: 2024/01/12 17:35:52 by kelmouto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "WebServer.hpp"
#include<algorithm>
#include<iterator>
#include "Config.hpp"

bool Server ::Pathvalide(std::string s, int mode)
{
    struct stat st;

    if (stat(s.c_str(), &st) == 0)
    {
        if (S_ISDIR(st.st_mode)  || access(s.c_str(), mode) != 0)
            return(false);
    }
    else
        return false;
    return(true);
}
std::map<std::string,std::vector<std::string> > Server::mimeTypes;

Server::Server()
{
    root = "";
    port = 8080;
    fd = -1;
    lflag  = true;
    ipAdress = "127.0.0.1";
    flaag = false;
    client_body_timeout = 30; 
    client_max_body_size = 10485760;
    rodirective = true;
    clbody = true;
}

bool trimspace(std::string s)
{
   int i = 0;
   if(s.empty())
        return(false);
    while(s[i])
    {
        if(!isspace(s[i]))
            return true;
        i++;
    }
    return(false);
}

std::vector<std::string> fillhelper(std::string v)
{
    int j = v.length() - 1;
    while(j > 0  && ( isspace(v[j])|| v[j] == '\n' ))
        j--;
    if(v[j] != ';' )
        throw std::runtime_error("error ");
    v = v.substr(0,j + 1);
    std::istringstream ser(v);
    std::string word;
    size_t i = 0;
    std::vector<std::string>helper;
    std::string line;
    std::string result;
    while(std::getline(ser,line,';'))
    {
        if(trimspace(line))
        {
            result.append(line);
            result.append(" ; ");
        }
        else
            throw std::runtime_error("Error in ;");
    }
    std::stringstream ss(result);
    while(ss >> word)
    {
        i = word.length();
        if( word != ";" && word[i - 1] == ';')
        {
            helper.push_back(word.substr(0,i - 1));
            helper.push_back(";");
        }
        helper.push_back(word);
    }
    return(helper);
}

void Server:: fillErrIndexAllofunc(std::vector<std::string>::iterator& it,std::vector<std::string>::iterator e)
{
    char *end;
    if(*it == "error_page")
    {
        if((it + 1 )!= e)
            it++;
        if(*it != ";" && (it + 1) != e && *(it + 1) != ";"  && (it + 2) != e && *(it + 2) == ";")
        {

            if((*it).find_first_not_of("0123456789") != std::string::npos)
                throw std::runtime_error(" directive should be a digit");
            else if(strtoul((*it).c_str(), &end, 10) < 400 || strtoul((*it).c_str(), &end, 10) > 599)
                throw std::runtime_error(" directive should be a valid status code");
            else if(Pathvalide(*(it + 1),R_OK))
            {
                std::string tmp = *it;
                it++;
                errorPages[tmp]=*it;
                it++;
            }
            else
                it+= 2;
        }
        else 
            throw std::runtime_error(" Error in error pages ");   
    }
}

void Server:: fillTimeSizeClient(std::vector<std::string>::iterator& it,std::vector<std::string>::iterator e)
{
  
    if(*it == "client_max_body_size")
    {
        if(clbody)
        {
            clbody = false;
            if((it + 1 )!= e)
                it++;
            if((*it).find_first_not_of("0123456789") != std::string::npos)
                throw std::runtime_error(" directive should be a positive digit");
            if(*it != ";" &&  (it + 1) != e && *(it + 1) == ";")
            {
                std::stringstream n(*it);
                n >> client_max_body_size;
                  if (n.fail() || n.bad())
                    throw std::runtime_error(" too large input");
                it++;
            }
            else
                throw std::runtime_error("Error client_max_body_size directive ");
        }
        else
            throw std::runtime_error("client_max_body_size directive is  duplicated ");
    } 
}

void Location:: fillLocation(std::vector<std::string>::iterator& it,std::vector<std::string>::iterator e)
{
    if(*it == "index")
    {
        if(!index.size())
        {
            if((it + 1 )!= e)
                it++;
            while(*it != ";" && it != e)
            {
                index.push_back(*it);
                it++;
            }
        }
        else
            throw std::runtime_error("index duplicated");
    }
    if(*it == "uploads")
    {
         if((it + 1 )!= e)
            it++;
        while(*it != ";" && it != e)
        {
            uploads.push_back(*it);
            it++;
        }
        if(uploads.size() > 2 )
            throw std::runtime_error("Error in uploads directive");
    } 
    if(*it == "allow_methods")
    {
        if(allow)
        {
            allow =false;
            int i = 0;
            if((it + 1 )!= e)
                it++;
            while(it != e && *it != ";" )
            {
                if(*it == "GET")
                    allow_methods[*it] = true;
                else if(*it == "POST")
                    allow_methods[*it] = true;
                else if(*it == "DELETE")
                    allow_methods[*it] = true;
                else
                    throw std::runtime_error("Only Post,delete,Get can be allowed");
                it++;
                i++;
                if( i > 3)
                   throw std::runtime_error("Only Post,delete,Get can be allowed"); 
            }
        }
        else
            throw std::runtime_error("allow_methods directive duplicated");
    }
}

std::string affect(std::vector<std::string>::iterator& it,std::vector<std::string>::iterator e)
{
    std::string tmp;
    if((it + 1 )!= e)
        it++;
    if(*it != ";" &&  (it + 1) != e && *(it + 1) == ";")
        {
            tmp = *it;
            it++;
            return(tmp);
        }
    else
        throw std::runtime_error("Error directive ");
}

Location  Server::buildClass(std::string v)
{
    Location o;
    std::vector<std::string>helper = fillhelper(v);
    std::vector<std::string>::iterator it = helper.begin();
    for(;it != helper.end();it++)
    {
        o.fillLocation(it,helper.end());
        if(*it == "root")
            {
                if(o.flagg)
                    {
                        o.root = affect(it,helper.end());
                        o.flagg = false;
                    }
                else
                    throw std::runtime_error(" root duplicated");

            }
        if(*it == "return")
        {
            if(o.Return.empty())
            {
                if((it + 1 )!= helper.end())
                    it++;
                if(*it != ";" && (it + 1) != helper.end() && *(it + 1) != ";"  && (it + 2) != helper.end() && *(it + 2) == ";")
                {
                    char *end;
                    if((*it).find_first_not_of("0123456789") != std::string::npos)
                        throw std::runtime_error(" directive should be a digit");
                    else if(strtoul((*it).c_str(), &end, 10) != 301 && strtoul((*it).c_str(), &end, 10) != 302)
                        throw std::runtime_error("our web serv support just 301 && 302 status code");
                    std::string tmp = *it;
                    it++;
                    o.Return[strtoul(tmp.c_str(), &end, 10)]=*it;
                    it++;
                }
                else 
                    throw std::runtime_error(" Error in return directive ");
            }
            else
                throw std::runtime_error(" return directive duplicated");
        }
        if(*it == "cgi")
        {
            if((it + 1 )!= helper.end())
                it++;
            if(*it != ";" && (it + 1) != helper.end() && *(it + 1) != ";"  && (it + 2) != helper.end() && *(it + 2) == ";")
                {
                    o.cgi.insert(std::make_pair((*it),*(++it)));
                    it++;
                }
        else 
            throw std::runtime_error(" Error in cgi ");
        }
        if(*it == "autoindex") 
        {
            if(o.auflag)
            {
                o.auflag = false;
                if((it + 1 )!= helper.end())
                    it++;
                if(*it == "on" && (it + 1) != helper.end() &&  *(it + 1) == ";")
                    {
                        o.autoindex = true;
                        it++;
                    }
                else if(*it == "off" && (it + 1) != helper.end() &&  *(it + 1) == ";")
                        it++;
                else 
                    throw std::runtime_error(" Error in autoindex ** directive");
            }
            else 
                throw std::runtime_error(" autoindex duplicated");
        }
        if(*it != ";")
                throw std::runtime_error(" fausse location directive");
    }
    return o;
}
void  Server::check_duplicate(std::string s)
{
 std::map<std::string,Location>::iterator it = locations.find(s);
 if(it != locations.end())
    throw std::runtime_error("location duplicate");
}
std::string Server::parslocation(std::string v)
{
    Location o;
    std::string name;
    size_t found = v.find("location");
    size_t i = 0;
    while (found != std::string::npos && i < v.size()) 
    {
        i = found + 8;
        while (v[i] && (v[i] == ' ' || v[i] == '\t'))
            i++;
        name.clear();  
        while(v[i] && v[i] != '\n' && v[i] != ' ' && v[i] != '\t' &&  v[i] != '{')
        {
            name.push_back(v[i]);
            i++;
        }
        while(v[i] && v[i] != '{')
        {
            if(!isspace(v[i]) &&  v[i] != '\n' )
                throw std::runtime_error(" eroor in name location");
            i++;
        }
        int end = findEndofBlock(v, i + 1);
        if (end == -1)
            throw std::runtime_error(" block location error in {}");
        if(name[0] != '/')
            name.insert(name.begin(),'/');
        check_duplicate(name);
        locations.insert(std::make_pair(name, buildClass(v.substr(i + 1, end - i - 1))));
        v.erase(found, end - found + 1);
        found = v.find("location", found + 1);
    }
    return v;
}

void Server::setupglobalroot(std::map<std::string,Location> &v)
{
    std::map<std::string,Location>::iterator it = v.begin();
    for(;it!= v.end();it++)
    {
        if(it->second.root == "")
            it->second.root = this->root;
    }
}

void Server::funcMimeTypes(std::string filename)
{
    std::ifstream buffer(filename.c_str());
    if(!buffer.is_open())
        throw std::runtime_error("could not open filename");
    std::string line;
    std::string save;
    while(std::getline(buffer,line))
        save.append(line);
    std::vector<std::string> extension;
    std::istringstream ss(save);
    std::string key;
    while(ss >> key && key.find('{') == std::string::npos)
        continue;
    while(1)
    {
        if( ss >> key && key.find('}') == std::string::npos)
        {
            std::string value;
            while(ss >> value)
            {
                if(value[value.length() - 1] == ';')
                {
                    extension.push_back(value.substr(0,value.length() - 1));
                    break;
                }
                if(value == ";")
                    break;
                extension.push_back(value);
            }
            Server::mimeTypes[key] = extension;
            extension.clear();
        }
        else 
            break;   
    }
}
///// ajari

void    Server::CreationBindListen()
{
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1)
        throw std::runtime_error("Error creating socket:=>" + host);
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(ipAdress.c_str());
    server_address.sin_port = htons(port);
    int reuse = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1)
        throw std::runtime_error("Error accured in setsockopt:=>" + host); 
    if (bind(fd, (struct sockaddr*)&server_address, sizeof(server_address)) == -1)
        throw std::runtime_error("Error accured in bind:=>" + host);
    if (listen(fd, 5) == -1)
        throw std::runtime_error("Error listening for connections:=>" + host);
}

int Server::AddNewClient()
{
    struct sockaddr_in client_address;
    memset(&client_address, 0, sizeof(client_address));
    socklen_t client_address_len = sizeof(client_address);
    int fd_socket = accept(fd, (struct sockaddr*)&client_address, &client_address_len);
    if (fd_socket == -1) {
        std::cerr << "Error accepting connection\n" << std::endl;
        return 0;
    }
    client.push_back(Client(fd_socket, this));
    client.back().T = clock();
    return fd_socket;
}

void Server::run(WebServer & web)
{
    size_t i;
    try
    {
        for (i = 0; i < client.size(); i++)
            client[i].handleRequest(&web.FdRd, &web.FdWr);
        if (FD_ISSET(fd, &web.FdRd))
        {
            int i = AddNewClient();
            web.fds.push_back(i);
        }
    }
    catch(std::exception& e)
    {
        web.fds.erase(std::find(web.fds.begin(), web.fds.end(), client[i].fd));
        if (client[i].iN)
            closedir(client[i].iN);
        client[i].In->close();
        client[i].Out->close();
        close(client[i].fd);
        delete client[i].In;
        delete client[i].Out;
        client.erase(client.begin() + i);
    }
}