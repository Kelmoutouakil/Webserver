/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kelmouto <kelmouto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 13:34:47 by kelmouto          #+#    #+#             */
/*   Updated: 2024/01/10 09:34:10 by kelmouto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include "Server.hpp"  
int main()
{
 
 std::map<std::string ,std::vector<std::string> > map;
        try
        {
            std::string s;
            Config o;
            std::string f= o.ReadFile("config.conf");
;
            std::vector<Server>v = o.splitServers(f,0);
           std::vector<Server> :: iterator it = v.begin();
            for(;it != v.end(); it++)
            {
                std::cout<< (*it).listen << "   it" << (*it).root << "\n"; 
            }
            
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }    
    
    return (0);
}