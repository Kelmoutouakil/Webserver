#include "WebServer.hpp"

WebServer::WebServer(int ac, char **av)
{
    Config o;

    if (ac >= 2)
        conf = av[1];
    else
        conf = "config.conf";
    std::string f = o.ReadFile(conf.c_str());
    servers = o.splitServers(f, 0);
    servers.begin()->funcMimeTypes("mime.types");
}