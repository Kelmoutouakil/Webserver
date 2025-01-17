#include "Client.hpp"
#include "WebServer.hpp"
#include "InFile.hpp"
#include "Server.hpp"

void pp(std::map<std::string, std::string> & header)
{
    for (std::map<std::string, std::string>:: iterator i = header.begin(); i != header.end(); i++)
        std::cout << i->first << ": " << i->second << "|" << std::endl;
}

Client::Client(int Fd, Server *serv) : Serv(serv) ,fd(Fd)
{
    readMore = 1;
    location = NULL;
    check = -1;
    In = new InFile();
    Out = new std::fstream();
    reserve = "";
    chunked = -1;
    flag = false;
    sum = 0;
    timeout = false;
    timing = clock();
    id = -1;
    currentdirec = "";
}

Client::Client(const Client &obj)
{
    *this = obj;
}

Client& Client::operator=(const Client &obj)
{
    for (int i = 0; i < 3; i++)
        M_U_V[i] =  obj.M_U_V[i];
    readMore = obj.readMore;
    currentdirec = obj.currentdirec;
    Read = obj.Read;
    header =  obj.header;
    body =  obj.body;
    root =  obj.root;
    Out =  obj.Out;
    In =  obj.In;
    fd = obj.fd;
    check = obj.check;
    flag = obj.flag;
    chunked = obj.chunked;
    chunkSize = obj.chunkSize;
    Serv = obj.Serv;
    iN = NULL;
    reserve = obj.reserve;
    location = obj.location;
    sum = obj.sum;
    timeout = obj.timeout;
    timing = obj.timing;
    PathCGI = obj.PathCGI;
    id = obj.id;
    filename = obj.filename;
    count = obj.count;
    query = obj.query;
    T = obj.T;
    startTime = obj.startTime;
    argvv[0] = obj.argvv[0];
    argvv[1] = obj.argvv[1];
    for (int i = 0 ; i < 3;i++)
        argv[i] = obj.argv[i];
    envv = obj.envv;
    PATH_INFO = obj.PATH_INFO;
    AfterLoc = obj.AfterLoc;
    request = obj.request;
    uri = obj.uri;
    dir = obj.dir;
    return *this;
}

Client::~Client() 
{
    
}
void  Client ::getcurrentDirectory()
{
    char currentPath[PATH_MAX];
    if (getcwd(currentPath, sizeof(currentPath)) != NULL)
        currentdirec = currentPath;
}
void   Client::handleRequest(fd_set *Rd, fd_set *Wr)                                                                          //HandleRequest
{
  
    std::string nBytes;
    std::string response;
    getcurrentDirectory();
    if (FD_ISSET(fd, Rd) || FD_ISSET(fd,Wr))
    {
        Read = FD_ISSET(fd, Rd);
        if (readMore)
        {
            if (!FD_ISSET(fd, Rd) && (clock() - T) / CLOCKS_PER_SEC > 10)
                ServeError("408", " Request Timeout\r\n");
            else if(FD_ISSET(fd, Rd))
                ReadMore();
        }
        else if (timeout)
            handlCgi(filename, 1);
        else if (M_U_V[0] == "GET" && FD_ISSET(fd, Wr))
            GetMethod();
        else if(M_U_V[0] == "POST")
            PostMethodfunc();
        else if (M_U_V[0] == "DELETE")
            DeleteMethod(M_U_V[1], 1);
    }
}