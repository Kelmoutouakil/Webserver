#include "WebServer.hpp"
WebServer::WebServer() {}
WebServer::WebServer(int ac, char **av)
{
    Config o;

    if (ac >= 2)
        conf = av[1];
    else
        conf = "config.conf";
    std::string f = o.ReadFile(conf.c_str());
    servers = o.splitServers(f, 0);
     std::map<std::string,Location>::iterator it = (servers.begin())->locations.begin();
    for(;it != (servers.begin())->locations.end();it++)
    {
        std::cout<< "name : "<< it->first << " : ";
        std::vector<std::string> ::iterator ite = it->second.index.begin();
        for(;ite != it->second.index.end();ite++)
            std::cout<<">"<< *ite << "< , ";
        std::cout<< "\n";
    }
    exit(0);
    servers.begin()->funcMimeTypes("mime.types");
}