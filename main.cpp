#include "WebServer.hpp"
#include "Client.hpp"
#include "Config.hpp"
#include "Server.hpp"
#include "WebServer.hpp"



int main(int ac, char **av) 
{
    WebServer web;
    try
    {
        WebServer b(ac, av);
        web = b;
      
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return -1;
    }
    while(true)
    {
        for (size_t i = 0; i < web.servers.size(); i++)
        {
            web.servers[i].run(web);
        }
    }
    return 0;
}
