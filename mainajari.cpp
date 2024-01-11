#include "WebServer.hpp"
#include "Client.hpp"
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

int CreationBindListen(void)
{
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        std::cerr << "Error creating socket" << std::endl;
        return -1;
    }
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);
    int reuse = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1) {
        perror("setsockopt"); 
        close(server_socket);
        exit(EXIT_FAILURE);
    }
    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        std::cerr << errno << std::endl;
        close(server_socket);
        exit(-1);
    }
    if (listen(server_socket, 5) == -1) {
        std::cerr << "Error listening for connections" << std::endl;
        close(server_socket);
        exit(-1);
    }
    return server_socket;
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

int main() 
{
    std::map<int, Client> client;
    std::vector<int> idx;
    fd_set FdRd, FdWr;
    int fd = CreationBindListen();
    //std::cout << "Server is listening on port " << PORT << std::endl;
    int i = 0;
    while (true)
    {
        try
        {
            FD_ZERO(&FdRd);
            FD_ZERO(&FdWr);
            FD_SET(fd, &FdRd);
            for (size_t i = 0; i < idx.size(); i++)
            {
                FD_SET(idx[i], &FdRd);
                FD_SET(idx[i], &FdWr);
            }
            if (select(max_fd(idx, fd), &FdRd, &FdWr, NULL, NULL) < 0) 
                exit(EXIT_FAILURE);
            if (FD_ISSET(fd, &FdRd))
                AddNewClient(client, idx, FdRd, FdWr, fd);
            for (size_t i = 0; i < client.size(); i++)
            {
                if ((FD_ISSET(idx[i], &FdRd) && client[idx[i]].IsR_Rd())  || (FD_ISSET(idx[i], &FdWr) && client[idx[i]].IsR_Wr()))
                    client[idx[i]].handleRequest();
            }
        }
        catch(const std::exception& e)
        {
            
            std::cerr << e.what() << '\n';
        }
        i++;
    }
    close(fd);
    return 0;
}
