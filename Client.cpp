#include "Client.hpp"
#include "WebServer.hpp"

Client::Client() {}
Client::Client(const Client &obj) { (void)obj;}

Client::~Client() {}


bool Client::IsR_Wr()
{
    return wr;
}

bool Client::IsR_Rd()
{
    return !request.size();
}

int toup(int t)
{
    if (t >= 'a' && t <= 'z')
        return t - 32;
    return t;
}

void    Client::setUp()
{
    root = "/Users/ajari/Desktop/webserver";
    std::string reque;
    char buffer[BUFFER_SIZE];
    int r;

    while(true)
    {
        r = read(fd, buffer, BUFFER_SIZE - 1);
        if (r < 0)
        {
            std::cout << "error reading" << std::endl;
            exit(0);
        }
        buffer[r] = '\0';
        reque += buffer;
        if (reque.find("\r\n\r\n") != std::string::npos)
            break;
    }
    std::cout << "request:\n" << reque << std::endl;
    this->ParseRequest(reque);
}

void   Client::ParseRequest(std::string &reque)
{
    if (reque.find("\r\n\r\n") == std::string::npos)
        throw std::runtime_error("Error : your request must end with \'\\r\\n\\r\\n\'.");
    while(true)
    {
        if ((reque.find("\r\n") == reque.find("\r\n\r")) && M_U_V[0] != "")
        {
            if (reque.find("\r\n") <= reque.find(":") + 1)
                throw std::runtime_error("Error : key value not as expected key:value");
            request[reque.substr(0, reque.find(":"))] = reque.substr(reque.find(":") + 1, reque.find("\r\n"));
            break;
        }
        else if (M_U_V[0] == "" && reque.find("\r\n") != std::string::npos)
        {
            std::stringstream a(reque.substr(0, reque.find("\r\n")));
            std::string word;
            int i;
            for(i = 0; i < 3 && a >> word; i++)
                M_U_V[i] = word;
            if (i < 3)
                throw std::runtime_error("Error : key value not as expected key:value");
        }
        else if (reque.find("\r\n") != std::string::npos)
        {
            if (reque.find("\r\n") <= reque.find(":") + 1)
                throw std::runtime_error("Error : key value not as expected key:value");
            request[reque.substr(0, reque.find(":"))] = reque.substr(reque.find(":") + 1, reque.find("\r\n"));
        }
        reque = reque.substr(reque.find("\r\n") + 2, reque.length());
    }
    if (M_U_V[0] == "GET")
        this->getMethode();
}

void    Client::getMethode()
{
    std::string header;
    std::string conType("Content-Type: video/mp4\r\n");

    InFile.open(root + M_U_V[1]);
    if(!InFile.is_open())
    {
        std::cout << "error opning file\n";
        exit(0);
    }
    header = M_U_V[2] + " 200 OK\r\n" + conType + "content-length: " + std::to_string(InFile.size()) + "\r\n\r\n";
    write(fd, header.c_str(), header.length());
}

void   Client::handleRequest()
{
    static int i;
    std::string nBytes;
    std::string response;

    //reque(request);
    if (request.empty())
        this->setUp();
    if (M_U_V[0] == "GET" && InFile)
    {
        std::cout << "helloword\n";
        wr = 1;
        InFile.read(buffer, BUFFER_SIZE - 1);
        buffer[InFile.gcount()] = 0;
        write(fd, buffer ,InFile.gcount());
        if (InFile.eof())
        {
            std::cout << "\33[1;31m ----------------------------------" << i << ">\n\33[0m";
            InFile.close();
            write(fd, "\r\n\r\n", 4);
            request.clear();
            wr = 0;
        }
        std::cout << "is_open : " << "\n";
    }

}


