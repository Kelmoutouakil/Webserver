#include "Info.hpp"


Info::Info()
{

}

Info::Info(int fds)
{
    root = "/Users/ajari/Desktop/webserver";
    std::string reque;
    char buffer[BUFFER_SIZE];
    int r;

    fd = fds;
    while(true)
    {
        r = read(fd, buffer, BUFFER_SIZE - 1);
        if (r <= 0)
        {
            std::cout << "error reading" << std::endl;
            exit(0);
        }
        buffer[r] = '\0';
        reque += buffer;
        if (reque.find("\r\n\r\n") != std::string::npos)
            break;
    }
    while(true)
    {
        if (reque.find("\r\n") != reque.find("\r\n\r"))
        {
            request.push_back(reque.substr(0,reque.find("\r\n")));
            break;
        }
        request.push_back(reque.substr(0, reque.find("\r\n")));
        reque = reque.substr(reque.find("\r\n"), reque.length());
    }
    this->readRequest();
}

Info::~Info()
{

}

Info::Info(const Info &obj)
{
    *this = obj;
}

Info& Info::operator=(const Info &obj)
{
    (void)obj;
    return *this;
}

void   Info::handleRequest()
{
    std::stringstream nB;
    std::string n;

    if (M_U_V[0] == "GET")
    {
        InFile.read(buffer, std::streamsize(BUFFER_SIZE));
        if (!InFile)
        {
            std::cout << "error file accured\n";
            exit(0);
        }
        nB << std::hex << InFile.gcount();
        nB >> n;
        buffer[InFile.gcount()] = 0;
        std::string response(buffer);

        write(fd, (n + response + "\n\r").c_str(), n.length() + response.length() + 2);
    }
}

void   Info::readRequest()
{
    std::stringstream a(request[0]);
    std::string word;
    int i;
    for(i = 0; i < 3 && a >> word; i++)
        M_U_V[i] = word;
    if (i < 3)
    {
        std::cout << "error header request contain less then 3 words\n";
        exit(0);
    }
    if (M_U_V[0] == "GET")
        this->getMethode();
}

void    Info::getMethode()
{
    std::string header;
    std::string conType("Content-Type: text/html\n");

    InFile.open(root + M_U_V[1]);
    if(!InFile.is_open())
    {
        std::cout << "error opning file\n";
        exit(0);
    }
    header = M_U_V[2] + " 200 OK\n" + conType + "Transfer-Encoding: chunked\n\n";
    write(fd, header.c_str(), header.length());
}

void    Info::setUp()
{

}