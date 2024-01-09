#include "WebServer.hpp"
#include "Info.hpp"


void getMethode(int fd, std::stringstream &line)
{
    std::string word;
    std::string content;
    std::string header;
  
    line >> word;
    std::cout << word << std::endl;
    std::ifstream inFile(word.substr(1, word.length()));
    while(std::getline(inFile, word))
        content += word + "\n";
    line >> word;
    header = word + " 200 OK\nContent-Type: text/html\nTransfer-Encoding: chunked\n\n";
    write(fd, header.c_str() , header.length());
    write(fd, content.c_str(), content.length());
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
        return -1;
    }
    if (listen(server_socket, 5) == -1) {
        std::cerr << "Error listening for connections" << std::endl;
        close(server_socket);
        return -1;
    }
    return server_socket;
}


int main() 
{
    std::map<int, Info> clientInfos;
    std::vector<int> clients;
    int fd = CreationBindListen();
    fd_set fdread, fdwrite;
    std::cout << "Server is listening on port " << PORT << std::endl;
    while (true)
    {
        FD_ZERO(&fdread);
        FD_ZERO(&fdwrite);
        FD_SET(fd, &fdread);
        for (size_t i = 0; i < clients.size(); i++)
        {
            FD_SET(clients[i], &fdread);
            FD_SET(clients[i], &fdwrite);
        }
        std::cout <<" FD_ISSET(fd, &fdread)"<< std::endl;
        if (select(std::max(*std::max_element(clients.begin(), clients.end()), fd) + 1, &fdread, &fdwrite, NULL, NULL)< 0) 
            exit(EXIT_FAILURE);
        std::cout << FD_ISSET(fd, &fdread)<< std::endl;
        if (FD_ISSET(fd, &fdread)) 
        {
            struct sockaddr_in client_address;
            socklen_t client_address_len = sizeof(client_address);
            std::cout << "WAITING ...\n";
            int client_socket = accept(fd, (struct sockaddr*)&client_address, &client_address_len);
            std::cout << "\33[0;31mfd : " << client_socket << std::endl << "\33[0m";
            if (client_socket == -1) {
                std::cerr << "Error accepting connection" << std::endl;
                continue;
            }
            clients.push_back(client_socket);
            clientInfos.insert(std::make_pair(client_socket, Info(client_socket)));
            FD_SET(client_socket, &fdread);
            FD_SET(client_socket, &fdwrite);
        }
        for (size_t i = 0; i < clients.size(); i++)
        {
            if (FD_ISSET(clients[i], &fdread))
                clientInfos[clients[i]].handleRequest();
            else if (FD_ISSET(clients[i], &fdwrite))
                clientInfos[clients[i]].handleRequest();
        }

        // std::cout << "selet\n";
        // std::cout << "out selet\n";
        // for (size_t i = 0;i <  d.size();i++)
        //     std::cout << d[i] << " is ready ?:" << FD_ISSET(d[i], &fds) << "\n";
        // Info info;
        // if (a.find(client_socket) == a.end())
        //     a.insert(std::make_pair(client_socket, info));
        // handle_request(client_socket);
        // for (int i = 0; i < FD_SETSIZE; ++i) {
        // if (FD_ISSET(i, &fds)) {
        //     std::cout <<  i << ", ";
        // }
        // std::cout << "\n";
        // }
    }
    close(fd);
    return 0;
}
