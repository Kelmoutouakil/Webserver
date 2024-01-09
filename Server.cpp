/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kelmouto <kelmouto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 13:53:46 by kelmouto          #+#    #+#             */
/*   Updated: 2024/01/09 10:39:05 by kelmouto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"Server.hpp"
#include<algorithm>
#include<iterator>
void Server::parslocation(std::string v)
{
    Location o;
    std::string name ;
    std::vector<std::string> tmp;
    std::map<std::string,std::vector<std::string> > helper;
    size_t found =v.find("location");
    while (found != std::string::npos) 
    {
        int i = found;
        while(v[i] && (v[i] == ' ' || v[i] == '\t'))
            i++;
        while(v[i] != '{')
            name.push_back(v[i]);
        int end = findEndofBlock(v, i + 1);
        if(end == -1)
            throw std::runtime_error(" bloc location error in {}");
        locations.insert(std::make_pair(v.substr(i + 1,end - i - 1));
        v.erase(found,end - found + 1);
        // Move to the next position after the last found occurrence
        found = v.find("location", found + 1);
    }
}

// }
// std::string Server:: getServerName()
// {
//     std::vector<std::string>::iterator it = serverBlock.find("server_name");
//     if(it != serverBlock.end())
//     {
//         it++;
//         while(*it != ";")
//             this->serverName += (*it)++;
//         return  serverName;
//     }
//     else
//         return("default name");
// }

// std::string Server:: getRoot()
// {
//     std::vector<std::string>::iterator it = serverBlock.find("root");
//     if(it != serverBlock.end())
//     {
//         it++;
//         while(*it != ";")
//             this->root += (*it)++;
//         return  root;
//     }
//     else
//         return("default root");
// }

// std::string Server:: getListen()
// {
//     std::vector<std::string>::iterator it = serverBlock.find("listen");
//     if(it != serverBlock.end())
//     {
//         it++;
//         while(*it != ";")
//             this->listen += (*it)++;
//         return  listen;
//     }
//     else
//         throw std::runtime_error("listen not found");
// }



//    std::string getIndex();
//    std::string getAutoindex();
//    std::map<std::string,std::string> getErrorPages();
//    std::map<std::string, bool>getAllow_methods();