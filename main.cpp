#include "WebServer.hpp"
#include "Client.hpp"
#include "Config.hpp"
#include "Server.hpp"
#include <cmath>

void func(int k ) {(void)k;}

int main(int ac, char **av) 
{
    if (ac > 2)
    {
        std::cout << "Error: invalid argument\n";
        std::exit(EXIT_FAILURE);
    }
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
    signal(SIGPIPE, func);
    web.RUN();
}
