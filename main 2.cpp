/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kelmouto <kelmouto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 13:34:47 by kelmouto          #+#    #+#             */
/*   Updated: 2024/01/09 09:57:43 by kelmouto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include "Server.hpp"
// void buildServers(std::vector<std::string> vect)
// {
//     Server o;
//     o.serverBlock = vect;
//     o.listen = o.getListen();
//     o.serverName = o.getServerName();
    
    

// }
int main()
{
 
 std::map<std::string ,std::vector<std::string> > map;
        try
        {
            std::string s;
            Config o;
            std::string f= o.ReadFile("config.conf");
            std::vector<std::vector<std::string> >v = o.splitServers(f,0);
            std::vector<std::vector<std::string> > :: iterator it = v.begin();
            for(;it != v.end(); it++)
            {
               
                std::vector<std::string>::iterator ite = (*it).begin();
                for(;ite != (*it).end();ite++)
                    std::cout <<">"<< *ite<< "<"<< "\n";
            }
                //  buildServers(*it);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }    
    
    return (0);
}