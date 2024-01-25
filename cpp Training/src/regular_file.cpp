

#include <iostream>

#include "regular_file.hpp"

#define overide
#define _GREEN_  "\033[1;32m"
#define _RESET_   "\033[1;0m"

namespace ilrd
{

    File::File(const std::string &name_, size_t offset_) throw()
        : FileComponent(name_, offset_)
    {

}

overide void File::Print() const throw()
{
    for (size_t i = 0; i < GetOffset(); i++)
    {
        std::cout << "    ";
    }
   
    File *fp = const_cast<File *>(this);
    if ((*fp).IsBinary())
    {
        std::cout << _GREEN_ << GetName() << _RESET_ << (*fp).IsBinary() <<"\n";
    }
    else
    {
        std::cout << _RESET_ << GetName() << "   " <<  (*fp).IsBinary() <<"\n";
    }
}



} //namespace ilrd