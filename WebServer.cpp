#include "WebServer.hpp"
#include "Server.hpp"

WebServer::WebServer() {}
void WebServer::print()
{
    for (std::vector<Server>::iterator i = servers.begin(); i != servers.end();i++)
    {
        std::cout << "listen:" << i->ipAdress<< ":" << i->port << "|" << std::endl;     //directive 
        if (i->serverName.size())
            std::cout << "serverNames: " ;
        for (std::vector<std::string>::iterator k = i->serverName.begin(); k != i->serverName.end();k++)   //server names
            std::cout << *k << " ";
        std::cout << "|\n";
        for (std::map<std::string, Location>::iterator j = i->locations.begin(); j != i->locations.end(); j++) // locaitons
        {
            std::cout << "\t" << j->first << "|\n"; //location name
            if (!j->second.root.empty())// location root
                std::cout << "\t\troot:" << j->second.root << "|\n"; 
            if (j->second.index.size())// location index
                std::cout << "\t" "\t" << "index: ";                 
            for (std::vector<std::string>::iterator k = j->second.index.begin(); k != j->second.index.end();k++) 
                std::cout << *k << (((k + 1) == j->second.index.end())? "|\n": " ");
            std::cout << "\t"<< "\t" <<"autoindex: " << j->second.autoindex << std::endl; // autoindex
            if (j->second.Return.size())  // return
                std::cout << "\t" << "\t" << "return: " << j->second.Return.begin()->first << "|" << j->second.Return.begin()->second << "|\n";
            std::cout << "\t"<< "\t" <<"allowed method \n"; // allowed method
            std::cout << "\t" << "\t"<< "\t" <<"GET    :" << j->second.allow_methods["GET"] << "\n";
            std::cout << "\t" << "\t"<< "\t" <<"POST   :" << j->second.allow_methods["POST"] << "\n";
            std::cout << "\t" << "\t"<< "\t" <<"DELETE :" << j->second.allow_methods["DELETE"] << "\n";
            for (std::map<std::string, std::string>::iterator t = j->second.cgi.begin(); t != j->second.cgi.end();t++)
                std::cout << "\t"<< "\t" <<"extention: " << t->first << " scriptPath: " << t->second << "\n";
        }
    }    
}

WebServer::WebServer(int ac, char **av)
{
    Config o;

    if (ac >= 2)
        conf = av[1];
    else
        conf = "config.conf";
    std::string f = o.ReadFile(conf.c_str());
    Server::funcMimeTypes("mime.types");
    servers = o.splitServers(f, 0);
    for (size_t i = 0; i < servers.size(); i++)
    {
        int nb  = servers[i].port;
        std::stringstream ss;
        ss << nb;
        servers[i].host = servers[i].ipAdress + ":" + ss.str();
        hosts[servers[i].host].push_back(servers[i]);
    }
    for (mp::iterator i = hosts.begin(); i != hosts.end();)
    {
        try
        {
            if (i->second.front().fd == -1)
            {
                i->second.front().CreationBindListen();
                std::cout << G << i->first  << "bind: " << i->second.front().fd << "\n" << D;
                fds.push_back(i->second.front().fd);
                i->second.front().Servs = i->second;
            }
            i++;
        }
        catch(const std::exception& e)
        {
            hosts.erase(i);
            i = hosts.begin();
            std::cerr << R << e.what() << '\n' << D;
        }
        
    }
}

void WebServer::RUN()
{

    while(true && !hosts.empty())
    {
        FD_ZERO(&FdRd);
        FD_ZERO(&FdWr);
        for (size_t i = 0; i < fds.size(); i++)
        {
            FD_SET(fds[i], &FdRd);
            FD_SET(fds[i], &FdWr);
        }
        int j;
        if ((j = select(*std::max_element(fds.begin() , fds.end()) + 1, &FdRd, &FdWr, NULL, NULL)) < 0)
            std::exit(EXIT_FAILURE);
        for (mp::iterator i = hosts.begin(); i != hosts.end(); i++)
            i->second.begin()->run(*this);
    }
}
