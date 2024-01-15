#include "Client.hpp"
#include "WebServer.hpp"
#include "InFile.hpp"
#include "Server.hpp"

Client::Client(int Fd,Server *serv) : Serv(serv) ,fd(Fd)
{
    In = new InFile();
    Out = new std::ofstream();
}

Client::Client(const Client &obj)
{
    *this = obj;
}
 
Client& Client::operator=(const Client &obj)
{
    for (int i = 0; i < 3; i++)
        M_U_V[i] =  obj.M_U_V[i];
    request = obj.request;
    header =  obj.header;
    body =  obj.body;
    root =  obj.root;
    Out =  obj.Out;
    In =  obj.In;
    fd = obj.fd;
    Serv = obj.Serv;

    return *this;
}

Client::~Client() 
{
    std::cout << "client destructor called \n";
}

void Client::ReadMore()
{
    int r = read(fd, buffer, BUFFER_SIZE - 1);
    if (!r)
        throw Serv->errorPages.find("400");
    
}

void   Client::ParseFirstLine(std::string line)
{
    std::stringstream first(line);

    for (int i = 0;i < 3 && first >> M_U_V[i];i++)
    ;
    if (!M_U_V[0][0] || !M_U_V[1][0]|| !M_U_V[1][0])
        throw Serv->errorPages.find("400");
    if (M_U_V[0] != "GET" && M_U_V[0] != "POST" && M_U_V[0] != "DELETE")
        throw Serv->errorPages.find("405");
    while (M_U_V[1].length() && Serv->locations.find(M_U_V[1]) !=  Serv->locations.end())
        M_U_V[1].pop_back();
    if (Serv->locations.find(M_U_V[1]) != Serv->locations.end())
    {
        M_U_V[1] =  Serv->locations[M_U_V[1]].root + M_U_V[1];
        location = &Serv->locations[M_U_V[1]];
    }
    M_U_V[1] = Serv->root + M_U_V[1];
}

void   Client::ParseHeader(std::string &request)
{
    std::string line;
    
    if (request.find("\r\n\r\n") == std::string::npos)
        throw std::runtime_error("Error : your requestst must end with \'\\r\\n\\r\\n\'.");
    while(request.find("\r\n\r\n") != std::string::npos)
    {
        line = request.substr(0, request.find("\r\n"));
        if (!(line.find(":") != std::string::npos && (line.length() - std::count(line.begin(), line.end(), ' ') >= 3)))
            throw std::runtime_error("Error : key value not as expected key:value");
        header[line.substr(0, line.find(":"))] = line.substr(line.find(":" + 1, line.length()));
        request = request.substr(request.find("\r\n") + 2, request.length());
    }
    if (request.length() > 2)
        body = request.substr(2, request.length());
}

void    Client::openFileSendHeader()
{
    std::string header;
    std::string conType("Content-Type: video/mp4\r\n");

    In->open(root + M_U_V[1]);
    std::cout << "hi\n" << In << "\n";
    if(!In->is_open())
    {
        std::cout << "error opning file\n";
        exit(0);
    }
    header = M_U_V[2] + " 200 OK\r\n" + conType + "content-length: " + std::to_string(In->size()) + "\r\n\r\n";
    write(fd, header.c_str(), header.length());
}
// void  Client::PostMethod(Client obj)
// {

//     if(body.length() >= BUFFER_SIZE)
//     {
//         *Out << body.substr(0,BUFFER_SIZE);
//         body.erase(0,BUFFER_SIZE);
//     }
//     else
//     {
//         *Out<< body;
//         body.clear();
//     }
// }
            
// void Client::ChunckedMethod(Client obj)
// {
    
//     int i = 0;
//     std::string line;
//     size_t chunkSize;
//     size_t len = body.length();
//     size_t totalSize = BUFFER_SIZE;
//     while(len > 0 &&  totalSize > 0)
//     {
//         i = 0;
//         while(body[i] && body[i] != '\r')
//             line.push_back(body[i++]);
//         i+=2;
//         chunkSize = std::stoi(line,NULL,16);
//         if(chunkSize == 0)
//         {
//             fd = -1;
//             break;
//         }
//         *Out << body.substr(i,chunkSize);
//         body.erase(0,i + chunkSize);
//         len  -= i + chunkSize;
//         totalSize -= i+ chunkSize;
//     }
// }
void   Client::handleRequest(fd_set *Rd, fd_set *Wr)
{
    static int i;
    std::string nBytes;
    std::string response;

    std::cout << "hello fd:" <<  fd << std::endl;
    if (FD_ISSET(fd, Rd) || FD_ISSET(fd, Wr))
    {
        if (request.find("\r\n\r\n") == std::string::npos)
            ReadMore();
    //    else if(M_U_V[0] == "POST")
    //     {
    //         char Store[BUFFER_SIZE];
    //         int total  = 0;
    //         int content_length;
    //         std::map<std::string,std::string> ::iterator it = header.find("Content-Length");
    //         if(it != header.end())
    //         {
    //             content_length = std::stoi(header["Content-Length"]);
    //             if(total = read(fd,Store,BUFFER_SIZE) > 0 )
    //             {
    //                 count += total;
    //                 Store[total] = '\0';
    //                 body.append(Store);
    //                 if(count >= content_length)
    //                     fd = -1;
    //                 PostMethod(*this);     
    //             }
    //         }
    //         else if(header.find("Transfert_Encoding") != header.end())
    //         {
    //             if(header["Transfert_Encoding"] == "chunked")
    //             {
    //                 if(total = read(fd,Store,BUFFER_SIZE) > 0 )
    //                 {
    //                     Store[total] = '\0';
    //                     body.append(Store);
    //                     ChunckedMethod(*this);
    //                 }
    //             }
    //         }
    //     } 
    
    }
}

