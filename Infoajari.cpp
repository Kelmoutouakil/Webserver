#include "Info.hpp"
#include "WebServer.hpp"

Info::Info() {}
Info::Info(const Info &obj) { (void)obj;}

Info::~Info() {}


bool Info::IsR_Wr()
{
    return wr;
}

bool Info::IsR_Rd()
{
    return !request.size();
}

int toup(int t)
{
    if (t >= 'a' && t <= 'z')
        return t - 32;
    return t;
}

// void readFile(inFile &InFile, std::string &response, char buffer[])
// {
//     // int i = 0;
//     (void)buffer;
//     char b[BUFFER_SIZE];

//     InFile.read(b, BUFFER_SIZE - 1);
//     b[InFile.gcount()] = 0;
//     std::cout << "read :" << InFile.gcount() << std::endl;
//     response = b;
//     std::cout << response.length() << std::endl;
//     // for (i = 0; i < BUFFER_SIZE;)
//     // {
//     //     InFile.read(buffer, 1);
//     //     if (InFile.eof())
//     //         break;
//     //     i++;
//     //     buffer[1] = 0;
//     //     response += buffer;
//     // }
// }
 
void   Info::handleRequest()
{
    static int i;
    std::string nBytes;
    std::string response;

    reque(request);
    if (request.empty())
        this->setUp();
    if (M_U_V[0] == "GET" && InFile)
    {
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
        //std::cout << "is_open : " << InFile.is_open() << "\n";
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
        //std::cout << "error header request contain less then 3 words\n";
        exit(0);
    }
    if (M_U_V[0] == "GET")
        this->getMethode();
}

void    Info::getMethode()
{
    std::string header;
    std::string conType("Content-Type: video/mp4\r\n");

    InFile.open(root + M_U_V[1]);
    if(!InFile.is_open())
    {
        //std::cout << "error opning file\n";
        exit(0);
    }
    header = M_U_V[2] + " 200 OK\r\n" + conType + "content-length: " + std::to_string(InFile.size()) + "\r\n\r\n";
    write(fd, header.c_str(), header.length());
    InFile.open(root + M_U_V[1]);
    //std::cout << "header:\n" << header << std::endl;
}

void    Info::setUp()
{
    root = "/Users/ajari/Desktop/webserver";
    std::string reque;
    char buffer[BUFFER_SIZE];
    int r;

    while(true)
    {
        r = read(fd, buffer, BUFFER_SIZE - 1);
        if (r <= 0)
        {
            //std::cout << "error reading" << std::endl;
            exit(0);
        }
        buffer[r] = '\0';
        reque += buffer;
        if (reque.find("\r\n\r\n") != std::string::npos)
            break;
    }
    //std::cout << "\33[1;32m" << reque << "\33[0;m\n";
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