/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kelmouto <kelmouto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 13:34:47 by kelmouto          #+#    #+#             */
/*   Updated: 2024/01/10 21:20:15 by kelmouto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include "Server.hpp" 
 
int main(int ac,char **av)
{
    if(ac == 2)
    {
        try
        {       
            Config o;
            std::string f= o.ReadFile(av[1]);
            std::vector<Server>v = o.splitServers(f,0);
            std::vector<Server> :: iterator it = v.begin();
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