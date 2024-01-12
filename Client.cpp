#include "Client.hpp"
#include "WebServer.hpp"
#include "InFile.hpp"
Client::Client(int fdsocket)
{
    root = "/Users/ajari/Desktop/webserver";
    std::string request, n;
    char buffer[BUFFER_SIZE];
    fd = fdsocket;
    In = new InFile();
    Out = new std::ofstream();
    std::cout << "constructor\n";
    while(true)
    {
        std::cout << "clear fd: " << fd << "\n";
        int r = read(fd, buffer, BUFFER_SIZE - 1);
        std::cout << "clear\n";
        if (r < 0)
            throw std::runtime_error("Error reading form client socket:" + std::to_string(fd));
        buffer[r] = '\0';
        request += buffer;
        if (request.find("\r\n\r\n") != std::string::npos)
        {
            std::cout << "hello world\n";
            break;
        }
    }
    std::stringstream first(request.substr(0, request.find("\r\n")));
    request = request.substr(request.find("\r\n") + 2, request.length());
    for (int i = 0;i < 3 && first >> M_U_V[i];i++)
        ;
    if (!M_U_V[0][0] || !M_U_V[1][0]|| !M_U_V[1][0])
        throw std::runtime_error("the first line of the header must have three word\n");
    if  (M_U_V[0] == "GET")
    {
        this->openFileSendHeader();
        return ;
    }
    this->ParseRequest(request);
}

Client::Client(const Client &obj)
{
    *this = obj;
}

Client& Client::operator=(const Client &obj)
{
    for (int i = 0; i < 3; i++)
        M_U_V[i] =  obj.M_U_V[i];
    header =  obj.header;
    body =  obj.body;
    root =  obj.root;
    Out =  obj.Out;
    In =  obj.In;
    fd = obj.fd;

    return *this;
}

Client::~Client() 
{
    std::cout << "client destructor called \n";
}

int toup(int t)
{
    if (t >= 'a' && t <= 'z')
        return t - 32;
    return t;
}

void   Client::ParseRequest(std::string &request)
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
void  Client::PostMethode(Client& obj)
{
    Server o;
    std::map<std::string,Location>::iterator it = o.locations.find(M_U_V[1]);
    if(it != o.locations.end())
    {
        std::vector<std::string>::iterator ite = (it->second).uploads.begin();
        for(;ite != (it->second).uploads.end();ite++)
        {
            if(*ite == "on")
            {
                std::string file = *(ite + 1);
                Out->open(file+"html",std::ios_base::out | std::ios_base::app);
                if(Out->is_open())
                {

                }
            }
        }
    }
}

void   Client::handleRequest(fd_set *Rd, fd_set *Wr)
{
    static int i;
    std::string nBytes;
    std::string response;

    std::cout << "hello fd:" <<  fd << std::endl;
    if (FD_ISSET(fd, Rd) || FD_ISSET(fd, Wr))
    {
        if (M_U_V[0] == "GET" && In)
        {
            std::cout << "is open:" << In << "\n" << In->is_open() << "\n";
            In->read(buffer, BUFFER_SIZE - 1);
            buffer[In->gcount()] = 0;
            write(fd, buffer ,In->gcount());
            if (In->eof())
            {
                std::cout << "\33[1;31m ----------------------------------" << i << ">\n\33[0m";
                write(fd, "\r\n\r\n", 4);
                close(fd);
                fd = -1;
            }
            std::cout << "is_open : " << "\n";
        }
        else if(M_U_V[0] == "POST")
        {
            
            PostMethode(*this);
        }
    }
}


