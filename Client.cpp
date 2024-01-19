#include "Client.hpp"
#include "WebServer.hpp"
#include "InFile.hpp"
#include "Server.hpp"

Client::Client(int Fd,Server *serv) : Serv(serv) ,fd(Fd)
{
    readMore = 1;
    location = NULL;
    check = -1;
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
    check = obj.check;
    Serv = obj.Serv;

    return *this;
}

Client::~Client()
{
    std::cout << "client destructor called \n";
}

void Header(std::map<std::string, std::string> header)
{
    std::cout << "\n\33[1;32m";
    for (std::map<std::string , std::string>::iterator i = header.begin(); i != header.end(); i++)
        std::cout << "key:" << i->first << "|value:" << i->second << "|\n";
    std::cout << "\33[0m\n";
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


bool fileExists(const std::string filePath) 
{
    std::ifstream file(filePath);
    if (file.is_open())
    {
        file.close();
        return true;
    }
    return false;
}

void  Client::PostMethod(Client obj)
{
        (void)obj;
        Out->write(body.c_str(),body.size());
        body.clear();
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
std::string findExtension(std::string t)
{
    std::map<std::string,std::vector<std::string> >::iterator it = Server::mimeTypes.find(t);
    if(it != Server::mimeTypes.end())
        return (*(it->second.begin()));
    return "";
}
void Client::OpeningFile()
{
    std::string t;
    std::string filename;
    t = findExtension(header["Content-Type"]);
    std::srand(time(NULL));
    std::string name  = std::to_string(std::rand() % 100 + 1) + "." + t;
    filename = *(location->uploads.begin() + 1);
    if(fileExists(filename))
    {
        if(filename[filename.length() - 1] != '/')
            filename +=("/" + name );
        else
            filename+= name;
    }
    else
       ServeError("403"," Forbidden\r\n");
    Out->open(filename, std::ios::out | std::ios::app);
    if(!Out->is_open())
        throw std::runtime_error("Couldn't open file ");
}
void Client::PostMethodfunc()
{
    char Store[BUFFER_SIZE];
    int total  = 0;
    int content_length;
    if(location->uploads.size() >= 2 && *(location->uploads.begin()) == "on")
    { 
        std::map<std::string,std::string>::iterator it = header.find("Content-Length");
        if(it != header.end())
        {
            if(check == -1)
            {
                OpeningFile(); 
                count  = body.size();
                check = 0;
            }
            content_length = std::stoi(header["Content-Length"]);
            if( body.size() >= (size_t)content_length)
            {
                PostMethod(*this);
                write(fd,M_U_V[2].c_str(),M_U_V[2].length());
                write(fd," 200 OK\r\n",9);
                write(fd,"Content-Type: ",14);
                write(fd,header["Content-Type"].c_str(),header["Content-Type"].length());
                write(fd,"\r\n\r\nstatus: success\n message: File successfully uploaded\r\n",58);
                throw std::runtime_error("");
            } 
            total = read(fd,Store,BUFFER_SIZE - 1);
            if  (total > 0 )
            {
                count += total;
                Store[total] = '\0';
                body.insert(body.end(),Store, Store + total);
                PostMethod(*this);
                if(count >= content_length)
                {
                    write(fd,M_U_V[2].c_str(),M_U_V[2].length());
                    write(fd," 200 OK\r\n",9);
                    write(fd,"Content-Type: ",14);
                    write(fd,header["Content-Type"].c_str(),header["Content-Type"].length());
                    write(fd,"\r\n\r\nstatus: success\n message: File successfully uploaded\r\n",58);
                    throw std::runtime_error("");
                }
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
    else
        ServeError("403", " Forbidden\r\n");
}



void    Client::DeleteMethod()
{
}


