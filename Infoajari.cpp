#include "Info.hpp"
#include "WebServer.hpp"

Info::Info() {}
Info::Info(const Info &obj) { (void)obj;}

Info::~Info() {}

int toup(int t)
{
    if (t > 'a' && t < 'z')
        return t - 32;
    return t;
}

void readFile(std::ifstream &InFile, std::string &response, std::string &nBytes, char buffer[])
{
    std::stringstream nB;
    std::string n;
    int i;

    for (i = 0; i < BUFFER_SIZE && !InFile.eof(); i++)
    {
        InFile.read(buffer, 1);
        buffer[1] = 0;
        response += buffer;
    }
    nB << std::hex << i;
    nB >> n;
    std::transform(n.begin(), n.end(), n.begin(), toup);
    n.insert(0, "\r\n", 2);
    nBytes = n;
}
 
void   Info::handleRequest()
{
    std::string nBytes;
    std::string response;

    reque(request);
    if (M_U_V[0] == "GET")
    {

        std::cout << "is_open : " << InFile.is_open() << "\n";
        readFile(InFile, response, nBytes, buffer);
        std::cout << "response :\n" << nBytes + "\r\n" + response << std::endl;
        write(fd, (nBytes + response + "\n\r").c_str(), nBytes.length() + response.length() + 2);
        if (InFile.eof())
        {
            InFile.close();
            fd = -1;
            write(fd, "0\r\n", 3);
            close(fd);
        }
        std::cout << "is_open : " << InFile.is_open() << "\n";
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
    std::string conType("Content-Type: text/html\r\n");

    InFile.open(root + M_U_V[1]);
    if(!InFile.is_open())
    {
        std::cout << "error opning file\n";
        exit(0);
    }
    header = M_U_V[2] + " 200 OK\r\n" + conType + "Transfer-Encoding: chunked\r\n\r\n";
    write(fd, header.c_str(), header.length());
    std::cout << header << std::endl;
}

void    Info::setUp(int fds)
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
    std::cout << "\33[1;32m" << reque << "\33[0;m\n";
    while(true)
    {
        if (reque.find("\r\n") == reque.find("\r\n\r"))
        {
            request.push_back(reque.substr(0, reque.find("\r\n")));
            break;
        }
        request.push_back(reque.substr(0, reque.find("\r\n")));
        reque = reque.substr(reque.find("\r\n") + 2, reque.length());
    }
    this->readRequest();
}