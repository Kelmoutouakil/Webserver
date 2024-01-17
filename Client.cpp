#include "Client.hpp"
#include "WebServer.hpp"
#include "InFile.hpp"
#include "Server.hpp"

Client::Client(int Fd,Server *serv) : Serv(serv) ,fd(Fd)
{
    readMore = 1;
    location = NULL;
    In = new InFile();
    Out = new std::ofstream();
}

Client::Client(const Client &obj)
{
    std::cout << "copy constructor called\n";
    *this = obj;
}

Client& Client::operator=(const Client &obj)
{
    for (int i = 0; i < 3; i++)
        M_U_V[i] =  obj.M_U_V[i];
    readMore = obj.readMore;
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

void    Client::ServeError(const std::string &Error, const std::string &reason)
{
    std::string response (M_U_V[2]);
    if (!response.length())
        response = "HTTP/1.0";
    response = response + " " + Error + reason + "content-type: txt/html\r\n"; 
    In->open(Serv->errorPages[Error].c_str());
    if (!In->is_open())
        throw std::runtime_error("Error in opning error file\n");
    std::string body((std::istreambuf_iterator<char>(*In)), std::istreambuf_iterator<char>());
    int size = In->size();
    std::cout << "size : " << size << std::endl;
    In->read(buffer, size);
    response = response +  "content-length: " + std::to_string(size) + "\r\n\r\n" + body;
    write(fd, response.c_str(), response.length());
    throw std::runtime_error(Error);
}

void   Client::ParseFirstLine(std::string line)
{
    std::stringstream first(line);

    for (int i = 0;i < 3 && first >> M_U_V[i];i++)
        ;
    if (!M_U_V[0][0] || !M_U_V[1][0]|| !M_U_V[1][0])
        ServeError("400", " Bad Request\r\n");
    while (M_U_V[1].length() && Serv->locations.find(M_U_V[1]) == Serv->locations.end())
        M_U_V[1].pop_back();
    for(std::map<std::string , Location>::iterator i = Serv->locations.begin(); i != Serv->locations.end();i++)
        std::cout << "\33[1;32mlocation :" << ">" << i->first << "<" << "\n\33[1;32m";
    if (Serv->locations.find(M_U_V[1]) != Serv->locations.end())
    {
        location = &Serv->locations[M_U_V[1]];
        if (location->root.back() == '/')
            location->root.erase(location->root.end() - 1);
        if (M_U_V[1].back() != '/')
            M_U_V[1].append("/");
        location->root.append(M_U_V[1]);
    }
    else
        ServeError("404", " Not found\r\n");
    std::cout << "\33[1;31m hi under serveError\n\33[0m"; 
    request.erase(request.begin() + 2 , request.end());
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
   (void)obj;
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
    (void)obj;
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
                ServeError("403"," Forbidden\r\n");
            Out->open(filename, std::ios::out | std::ios::app);
            if(!Out->is_open())
                throw std::runtime_error("Couldn't open file ");
            std::map<std::string,std::string> ::iterator it = header.find("Content-Length");
            if(it != header.end())
            {
                content_length = std::stoi(header["Content-Length"]);
                if(total == read(fd,Store,BUFFER_SIZE) > 0 )
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
                    if(total == read(fd,Store,BUFFER_SIZE) > 0 )
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
        ServeError("404", " Not Found\r\n");
}

void    Client::GetMethod()
{
    if (!In->is_open())
    {
        for(size_t i = 0;i < location->index.size(); i++)
        {
            if (access((location->root + location->index[i]).c_str(), R_OK) != -1)
            {
                In->open((location->root + location->index[i]).c_str());
                if (!In->is_open())
                {
                    std::cout << "\33[1;31m not open\n";
                    throw std::runtime_error("input file not open ?");
                }
                std::cout << "\33[1;31mhi------#:" <<  location->root << location->index[i] << "\33[0m\n";
                break;
            }
            if (i == location->index.size() - 1)
            {
                std::cout << "\33[1;31mhello------#:" <<  location->root << " " << location->index[i] << "\33[0m\n";
                ServeError("405", " Not Found\r\n");
            }
        }
    }
    if (!In->fail())
    {
        std::cout << " :" << In->fail() << "\n";
        In->read(buffer, BUFFER_SIZE - 1);
        std::cout << " :" << In->fail() << "\n";
        if (In->fail())
            throw std::runtime_error("error in the input file GET\n");
        std::cout << "-----> : " << fd << "\n";
        std::cout << ">\n";
        buffer[In->gcount()] = 0;
        std::cout << "--->###>" << buffer << "<\n" << std::endl;
        write(1, buffer, (size_t)In->gcount());
        std::cout << std::flush;
        // std::cout << "<after wrtie at fd    1\n";
        write(fd, buffer, (size_t)In->gcount());
        if (In->eof())
        {

        }
            throw std::runtime_error("hello");
    }
    else
        throw std::runtime_error("error in the input file GET\n");
    std::cout << "out GET\n";
        
}

void    Client::DeleteMethod()
{
}

void Client::ReadMore()
{
    std::string line;

    int r = read(fd, buffer, BUFFER_SIZE - 1);
    if (!r)
        ServeError("400", " Bad Request\r\n");
    buffer[r] = 0;
    request += buffer;
    if (request.find("\r\n\r\n") != std::string::npos)
    {
        readMore = 0;
        ParseFirstLine(request.substr(0, request.find("\r\n")));
        if (request == "\r\n")
            return ;
        while(request.find("\r\n\r\n") != std::string::npos)
        {
            line = request.substr(0, request.find("\r\n"));
            if (!(line.find(":") != std::string::npos && (line.length() - std::count(line.begin(), line.end(), ' ') >= 3)))
                ServeError("400", " Bad Request\r\n");
            header[line.substr(0, line.find(":"))] = line.substr(line.find(":" + 1, line.length()));
            request.erase(request.begin() + 2, request.end());
        }
        if (request.length() > 2)
            body = request.substr(2, request.length());
    }
}

void   Client::handleRequest(fd_set *Rd, fd_set *Wr)
{
    std::string nBytes;
    std::string response;

    if (FD_ISSET(fd, Rd) || FD_ISSET(fd, Wr))
    {
        std::cout << "hello fd:" <<  fd << std::endl;
        if (readMore)
            ReadMore();
        else if (M_U_V[0] == "GET")
        {
            std::cout << "\33 [1;32mhello\n\33[0m";
            GetMethod();
        }
        else if(M_U_V[0] == "POST")
            PostMethodfunc();
        else if (M_U_V[0] == "DELETE")
            DeleteMethod();
    }
}

