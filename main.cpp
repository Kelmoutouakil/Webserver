#include "WebServer.hpp"
#include "Client.hpp"
#include "Config.hpp"
#include "Server.hpp"
#include "WebServer.hpp"

 
int main(int ac, char **av) 
{
    WebServer web(ac, av);

    while(true)
    {
        for (size_t i = 0; i < web.servers.size(); i++)
        {
            web.servers[i].run(web);
        }
    }
    return 0;
}
