#include "InFile.hpp"

size_t InFile::size()
{
    std::streamsize original_position = this->tellg();
    this->seekg(0, std::ios::end);
    std::streampos t = this->tellg();
    this->seekg(original_position);
    this->close();
    return static_cast<size_t>(t);
}
