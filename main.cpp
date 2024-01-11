/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kelmouto <kelmouto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 13:34:47 by kelmouto          #+#    #+#             */
/*   Updated: 2024/01/11 14:01:08 by kelmouto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include "Server.hpp" 
 
int main(int ac,char **av)
{
    if(ac == 2)
    {
        std::map<std::string ,std::vector<std::string> > map;
        try
        {       
            Config o;
            std::string f= o.ReadFile(av[1]);
            std::vector<Server>v = o.splitServers(f,0);
            std::vector<Server> :: iterator it = v.begin();
            for(;it != v.end(); it++)
            {
                std::cout<<"hello\n";
                std::cout<< "Ipadress : "<< (*it).ipAdress << " Port : "<< (*it).port << "\n";
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }    
    }
    else
        std::cout<< "invalid argument\n";        
    return (0);
}