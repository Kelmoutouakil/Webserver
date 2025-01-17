/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlCgi.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajari <ajari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 13:33:21 by kelmouto          #+#    #+#             */
/*   Updated: 2024/02/16 21:25:25 by ajari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "WebServer.hpp"
#include "InFile.hpp"
#include "Server.hpp"
#include <cstdlib>

void p(std::string s)
{
    for (size_t i  = 0; i < s.length(); i++)
    {
        if (s[i] == '\n')
            std::cout << R << "\\n" << D;
        else if (s[i] == '\r')
            std::cout << G << "\\r" << D;
        if (s[i] != '\r')
            std::cout << s[i];
    }
}

std::string Client::parsheader(size_t i,std::string s, int n)
{
    std::stringstream stream(s.substr(0, i));
    std::map<std::string, std::string> head;
    std::string first, second;
    std::string line;
    char ptr[] = "\r\n\r\n";
    s.erase(s.begin(), s.begin() + i + n);
    s.insert(s.begin(), ptr , ptr + 4);
    while(std::getline(stream, line))
    {
        if (line.find(":") == std::string::npos)
            ServeError("502"," Bad Gateway\r\n");
        std::stringstream a(line);
        std::getline(a,first, ':');
        std::getline(a,second, ':');
        if (first.find_first_not_of(" \t\n\r\f\v") == std::string::npos || second.find_first_not_of(" \t\n\r\f\v") == std::string::npos)
            ServeError("502"," Bad Gateway\r\n");
        std::for_each(first.begin(), first.end(), lower);
        size_t pos = first.find_first_not_of(" \t\n\r\f\v");
        first = first.substr(pos, first.find_last_not_of(" \t\n\r\f\v") - pos + 1);
        pos = second.find_first_not_of(" \t\n\r\f\v");
        second = second.substr(pos, second.find_last_not_of(" \t\n\r\f\v") - pos + 1);
        if (head.find(first) != head.end())
            ServeError("502"," Bad Gateway\r\n");
        line = first + ": " + second;
        char f[] = "\r\n";
        s.insert(s.begin(),line.begin(), line.end());   
        s.insert(s.begin(), f , f + 2);
        head[first] = second;
    }
    if (head.find("content-type") == head.end())
        ServeError("502", " Bad Gateway\r\n");
    if (head.find("status") == head.end())
        head["status"] = "200 OK";

    return(M_U_V[2] + " " + head["status"] + s);
}

void Client::sendResponse(std::string s)
{
    size_t i,j;
    std::string str;
    i = s.find("\r\n\r\n");
    j = s.find("\n\n");
    if( i != std::string::npos)
        str = parsheader(i, s, 4);
    else if(j != std::string::npos)
        str = parsheader(j, s, 2);
    else
        ServeError("502"," Bad Gateway\r\n");
    OurWR(str.c_str(), str.size());
    throw std::runtime_error("cgi");
}

void ev(char **env, char **argv)
{
    std::cout << "env:\n";
    for (size_t i = 0; env[i];i++)
        std::cout << env[i] << "|" << std::endl;
    for (size_t i = 0; argv[i];i++)
        std::cout << argv[i] << "|" << std::endl;
    std::cout << "<\n";
}

void Client::handlCgi(std::string filepath,int i)
{
    std::stringstream f;
    f << fd;
    std::string  res;
    size_t j;
    if (!PathCGI.empty())
    {
        std::string newfile = "file.txt" + f.str();
        Out->close();
        if(!timeout)
        {
            timeout = true;
            for (j = 0; j < envv.size();j++)
                env[j] = (char *)envv[j].c_str();
            env[j] = NULL;
            for (j = 0; j < 2;j++)
                argv[j] = (char *)argvv[j].c_str();
            argv[j] = NULL;
            startTime = clock();
            if ((id  = fork()) == -1)
                std::runtime_error("Error in fork");
            if (id == 0)
            {
                if(freopen(filepath.c_str(), "r", stdin) == NULL)
                    std::exit(EXIT_FAILURE);
                chdir(dir.c_str());
                if(freopen(newfile.c_str(), "w", stdout) == NULL)
                    std::exit(EXIT_FAILURE);
                execve(PathCGI.c_str(), argv, env);
                std::exit(EXIT_FAILURE);
            }
        }
        int status;
        pid_t t = waitpid(id, &status, WNOHANG);
        if(t  > 0)
        {
            if(i)
                std::remove(filepath.c_str());
            if (WEXITSTATUS(status) != 0 || freopen((dir + "/" + newfile).c_str(), "r", stdin) == NULL)
            {
                std::remove((dir +"/" + newfile).c_str());
                ServeError("500"," Internal Server Error\r\n");
            }
            int red;
            while ((red = read(0, buffer, 1023)) > 0) 
                res.insert(res.end(),buffer,buffer + red);
            std::remove((dir + "/" + newfile).c_str());
            sendResponse(res);
        }
        else
        {
            if((clock() - startTime) / CLOCKS_PER_SEC >  5)
            {
                std::remove((dir + "/" + newfile).c_str());
                kill(id, SIGKILL);
                waitpid(id, &status, 0);
                ServeError("504"," Gateway Timeout\r\n");
            }
        }
    }
}
