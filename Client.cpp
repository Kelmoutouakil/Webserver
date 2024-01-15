#include "Client.hpp"
#include "WebServer.hpp"
#include "InFile.hpp"
#include "Server.hpp"

Client::Client(int Fd,Server *serv) : Serv(serv) ,fd(Fd)
{
    location = NULL;
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
    header =  obj.header;
    body =  obj.body;
    root =  obj.root;
    Out =  obj.Out;
    In =  obj.In;
    fd = obj.fd;
    Serv = obj.Serv;

    return *this;
}

void Client::ReadMore()
{
    int r = read(fd, buffer, BUFFER_SIZE - 1);
    if (!r)
        ServeError("400", " Bad Request\r\n");
    buffer[r] = 0;
    request += buffer;
    if (request.find("\r\n\r\n") != std::string::npos)
        ParseRequest();
}

Client::~Client() 
{
    std::cout << "client destructor called \n";
}

void    Client::ServeError(const std::string &Error, const std::string &reason)
{
    std::string response (M_U_V[2]);

    if (!response.length())
        response = "HTTP/1.0";
    response = response + " " + Error + reason + "content-type: txt/html\r\n"; 
    In->open(Serv->errorPages[Error].c_str());
    if (In->is_open())
        throw std::runtime_error("Error in opning error file\n");
    int size = In->size();
    response += "content-length: " + std::to_string(size) + "\r\n";
    In->read(buffer, size);
    buffer[size] = 0;
    write(fd, response.c_str(), response.length());
    throw std::runtime_error(Error);
}

void   Client::ParseFirstLine(std::string line)
{
    std::stringstream first(line);

    for (int i = 0;first >> M_U_V[i];i++)
        ;
    if (!M_U_V[0][0] || !M_U_V[1][0]|| !M_U_V[1][0])
        ServeError("400", " Bad Request\r\n");
    while (M_U_V[1].length() && Serv->errorPages.find(M_U_V[1]) != Serv->errorPages.end())
        ;
    if (Serv->locations.find(M_U_V[1]) != Serv->locations.end())
        location = &Serv->locations[M_U_V[1]];
    else
        ServeError("404", " Not found\r\n");
    request.substr(request.find("\r\n") + 2 , request.length());
}

void   Client::ParseRequest()
{
    std::string line;

    ParseFirstLine(request.substr(0, request.find("\r\n")));
    while(request.find("\r\n\r\n") != std::string::npos)
    {
        line = request.substr(0, request.find("\r\n"));
        if (!(line.find(":") != std::string::npos && (line.length() - std::count(line.begin(), line.end(), ' ') >= 3)))
            ServeError("400", " Bad Request\r\n");
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

bool fileExists(const std::string& filePath) 
{
    struct stat buffer;
    return (stat(filePath.c_str(), &buffer) == 0);
}

void  Client::PostMethod(Client obj)
{
   
     if(body.length() >= BUFFER_SIZE)
    {
        *Out << body.substr(0,BUFFER_SIZE);
        body.erase(0,BUFFER_SIZE);
    }
    else
    {    
        *Out<< body;
        body.clear();
    }
}

void Client::ChunckedMethod(Client obj)
{
    std::string line;
    size_t i;
    size_t chunkSize;
    size_t len = body.length();
    size_t totalSize = BUFFER_SIZE;
    try
    {
        while(len > 0 &&  totalSize > 0)
        { 
            i = body.find("\r\n");
            line = body.substr(0,i);
            i+= 2;
            chunkSize = std::stoi(line, nullptr, 16);
            if (chunkSize == 0)
            break;
            *Out<< body.substr(i ,chunkSize);;
            body.erase(0, i  + 2+ chunkSize);
            len -= i + 2 + chunkSize;
            totalSize -= i + 2 + chunkSize;
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }  
}

void Client::PostMethodfunc()
{
    char Store[BUFFER_SIZE];
    int total  = 0;
    int content_length;
    std::string filename;
    if(location)
    {
        if(location->uploads.size() >= 2 && *(location->uploads.begin()) == "on")
        {
            filename = *(location->uploads.begin() + 1);
            if(fileExists(filename))
                filename += " file.txt";
            else
                std::cout<< "Path not valid\n";
            Out->open(filename, std::ios::out | std::ios::app);
            if(!Out->is_open())
                std::cout<< " error opening file\n";
            std::map<std::string,std::string> ::iterator it = header.find("Content-Length");
            if(it != header.end())
            {
                content_length = std::stoi(header["Content-Length"]);
                if(total = read(fd,Store,BUFFER_SIZE) > 0 )
                {
                    count += total;
                    Store[total] = '\0';
                    body.append(Store);
                    if(count >= content_length)
                        fd = -1;
                    PostMethod(*this);    
                }
            }
            else if(header.find("Transfert_Encoding") != header.end())
            {
                if(header["Transfert_Encoding"] == "chunked")
                {
                    if(total = read(fd,Store,BUFFER_SIZE) > 0 )
                    {
                        Store[total] = '\0';
                        body.append(Store);
                        ChunckedMethod(*this);
                    }
                }
            }
        }
    }
    else
        std::cout<< "location not founnd\n";
}

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
        else if (M_U_V[0] == "GET" && In)
            GetMethode();
        else if(M_U_V[0] == "POST")
            PostMethodfunc();
        else if (M_U_V[0] == "DELETE")
            DeleteMethode();
    }
}

