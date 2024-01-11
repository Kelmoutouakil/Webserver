#include "WebServer.hpp"

WebServer::WebServer(int ac, char *av[])
{
    Config o;

    if (ac >= 2)
        conf = av[1];
    else
        conf = "webserver.conf";
    std::string f = o.ReadFile(av[1]);
    std::vector<Server> servers = o.splitServers(f,0);
}