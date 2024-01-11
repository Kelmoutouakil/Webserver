#include "WebServer.hpp"
#include "Client.hpp"
#include "Config.hpp"
#include "Server.hpp"
#include "WebServer.hpp"

void Error(const std::string &error, bool ex, int value = 0)
{
    std::cerr << error << std::endl;
    if (ex)
        exit(value);
}

void reque(std::vector<std::string> &request)
{
    std::cout << "request: size:" << request.size() << "\33[1;34m\n";
    for (size_t i = 0; i < request.size(); i++)
        std::cout << request[i] << std::endl;
    std::cout << "\33[0m\n";
}

int max_fd(std::vector<int> client, int fd)
{
    if (client.empty())
        return fd + 1;
    return std::max(*std::max_element(client.begin(), client.end()), fd) + 1;
}

void AddNewClient(std::map<int, Client > &client, std::vector<int> &idx, fd_set &FdRd, fd_set &FdWr, int fd)
{
    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);
    int client_socket = accept(fd, (struct sockaddr*)&client_address, &client_address_len);
    //std::cout << "\33[0;31mfd : " << client_socket << std::endl << "\33[0m";
    if (client_socket == -1) {
        std::cerr << "Error accepting connection" << std::endl;
        return;
    }
    idx.push_back(client_socket);
    client.insert(std::make_pair(client_socket, Client()));
    client[client_socket].fd = client_socket;
    FD_SET(client_socket, &FdRd);
    FD_SET(client_socket, &FdWr);
}

void serverOn(WebServer & web, Server &server)
{
    if (server.fd == -1)
        server.CreationBindListen();
    try
    {
        FD_ZERO(&web.FdRd);
        FD_ZERO(&web.FdWr);
        FD_SET(server.fd, &web.FdRd);
        for (size_t i = 0; i < server.idx.size(); i++)
        {
            FD_SET(server.idx[i], &web.FdRd);
            FD_SET(server.idx[i], &web.FdWr);
        }
        if (select(max_fd(server.idx, server.fd), &web.FdRd, &web.FdWr, NULL, NULL) < 0) 
            exit(EXIT_FAILURE);
        if (FD_ISSET(server.fd, &web.FdRd))
            AddNewClient(server.client, server.idx, web.FdRd, web.FdWr, server.fd);
        for (size_t i = 0; i < server.client.size(); i++)
        {
            if ((FD_ISSET(server.idx[i], &web.FdRd) && server.client[server.idx[i]].IsR_Rd())  || (FD_ISSET(server.idx[i], &web.FdWr) && server.client[server.idx[i]].IsR_Wr()))
                server.client[server.idx[i]].handleRequest();
        }
    }
    catch(const std::exception& e)
    { 
        std::cerr << e.what() << '\n';
    }
    std::cout << "clear\n";
}
 
int main(int ac, char **av) 
{
    WebServer web(ac, av);

    while(true)
    {
        for (size_t i = 0; i < web.servers.size(); i++)
        {
            serverOn(web, web.servers[i]);
        }
    }
    return 0;
}
