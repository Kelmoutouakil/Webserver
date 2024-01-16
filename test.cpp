#include<iostream>
#include<sstream>
#include<fstream>
#include<map>
#include<vector>
std::map<std::string,std::vector<std::string> > funcMimeTypes(std::string filename)
{
    std::ifstream buffer(filename);
    if(!buffer.is_open())
        throw std::runtime_error("could not open filename");
    std::string line;
    std::string save;
    while(std::getline(buffer,line))
    {
        if(line.empty())
            continue;
        save.append(line);
    }

    // std::cout<< save << "\n";
    std::map<std::string,std::vector<std::string> >Map;
    std::vector<std::string> extension;
    std::istringstream ss(save);
    std::string key;
    while(ss >> key && key.find('{') == std::string::npos)
        continue;
    while(1)
    {
        if( ss >> key && key.find('}') == std::string::npos)
        {
            std::string value;
        while(ss >> value)
        {
            if(value[value.length() - 1] == ';')
            {
                extension.push_back(value.substr(0,value.length() - 1));
                break;
            }
            if(value == ";")
                break;
            extension.push_back(value);
        }
        Map[key] = extension;
         extension.clear();
        }
        else 
            break;
       
    }
    return Map;
}
int main()
{
    try
    {
        std::map<std::string,std::vector<std::string> > Map = funcMimeTypes("mime.types");
    std::map<std::string,std::vector<std::string> >:: iterator it = Map.begin();
    for(;it != Map.end(); it++)
    {
        std::cout << " key : " << it->first << "  value:   " ;
        std::vector<std::string> ::iterator ite = it->second.begin();
        for(;ite != it->second.end() ; ite++)
            std::cout<< *ite << " ";
        std::cout<< "\n"; 
    }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    
}