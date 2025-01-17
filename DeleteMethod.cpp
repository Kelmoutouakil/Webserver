/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DeleteMethod.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kelmouto <kelmouto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 09:36:31 by kelmouto          #+#    #+#             */
/*   Updated: 2024/02/15 11:27:49 by kelmouto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Server.hpp"
#include "WebServer.hpp"
#include<cstdio>

void    Client::DeleteMethod(std::string filepath,bool isRoot = true)
{
    char path[PATH_MAX];
    char *s = realpath(filepath.c_str(),path);
    if (s != NULL)
    {
        if(currentdirec!= "" && s == currentdirec)
            ServeError("403", " Forbiden\r\n");
    }
    else
    {
        if(currentdirec!= "" && filepath == currentdirec)
            ServeError("403", " Forbiden\r\n");
    }
    struct stat st;
    if (stat(filepath.c_str(), &st) == 0 )
    {
        if (access(filepath.c_str(), R_OK | W_OK) == -1)
            ServeError("403", " Forbiden\r\n");
    }
    else
        ServeError("404", " Not Found\r\n");

    DIR *dir = opendir(filepath.c_str());
    if(dir)
    {
        struct dirent* dp;
        while ((dp = readdir(dir)) != NULL) 
        {
            if(dp->d_type == DT_DIR) 
            {   

                if((std::string)dp->d_name != "." && (std::string)dp->d_name != "..")
                    DeleteMethod((filepath + "/" + (std::string)dp->d_name),false);
            }
            else if(dp->d_type == DT_REG)
            {
                if( access((filepath + "/" + (std::string)dp->d_name).c_str(), R_OK | W_OK) == -1 || remove((filepath + "/"+(std::string)dp->d_name).c_str()) == -1)
                    ServeError("403"," Forbidden\r\n");  
            }
        }
        if(remove((filepath).c_str()) == -1)
            ServeError("403"," Forbidden\r\n");  
    }
    else if (remove(filepath.c_str()) != 0) 
        ServeError("403"," Forbidden\r\n");
    if (isRoot)
        ServeError("204"," No Content\r\n");
}
