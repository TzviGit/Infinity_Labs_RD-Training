
#include <sys/stat.h>
#include <fcntl.h> 
#include <iostream>                 // std::cout
#include <cstring> 
#include <cctype>                 // strcmp
#include <dirent.h>                 // directory entry functions
#include <algorithm>                // std::for_each
#include "directory.hpp"
#include "file_component.hpp"
#include "regular_file.hpp"

#define overide
#define _BLUE_ "\33[34;1m"
#define _RESET_ "\033[1;0m"

static bool NotParentDir(const char *name);

static int StrCmpCaseIgnore(const char *s1, const char *s2);

namespace ilrd
{


struct SortFunctor
{
    bool operator()(FileComponent *lhs_, FileComponent *rhs_) 
    {
        std::string s1 = lhs_->GetName();
        std::string s2 = rhs_->GetName();

        return StrCmpCaseIgnore(s1.c_str(), s2.c_str()) <= 0;
    }
}alpabetical;

Directory::Directory(const std::string& name_, size_t offset_)
                                                        throw(std::bad_alloc)
:   FileComponent(name_, offset_)
{
    DIR *folder = opendir(name_.data());
    if (NULL == folder)
    {
        std::cout << name_ << " [error opening dir]\n";
        exit(1);
    }

    struct dirent *entry;
    while(NULL != (entry = readdir(folder)) )
    {
        struct stat filestat;
        stat(entry->d_name,&filestat);
        
        if (entry->d_type == DT_DIR)
        {
            if (NotParentDir(entry->d_name))
            {
                std::string new_name = (name_ + "/" + entry->d_name);
                m_files.push_back(new Directory(new_name, offset_ + 1));
            }       
        }
        else
        {
            File *f1 = new File(entry->d_name, offset_ + 1);
           
            (*f1).SetIsBinary(!(strchr(entry->d_name, '.'))? 1 : 0);

            m_files.push_back(f1);
        }
        
    }
    closedir(folder);

    std::sort(m_files.begin(), m_files.end(), alpabetical);
}

struct DeleteFunctor_
{
    void operator() (FileComponent *ptr) { delete ptr; }
} DeleteFunctor;


overide Directory::~Directory() throw()
{
    //std::foreach is a template function which recieves template <typename func>
    //so this syntax is ok
    std::for_each(m_files.begin(), m_files.end(), DeleteFunctor);
}

struct PrintFunctor_
{
    void operator() (FileComponent *ptr) { ptr->Print(); }
} PrintFunctor;


overide void Directory::Print() const throw()
{
    for (size_t i = 0; i < GetOffset(); i++)
    {
        std::cout << "    ";
    }

    size_t name_place = GetName().find_last_of("/") + 1;
    std::cout <<    _BLUE_ <<
                    GetName().substr(name_place) <<
                    _RESET_"\n";

    std::for_each(m_files.begin(), m_files.end(), PrintFunctor);
}

} //namespace ilrd


static bool NotParentDir(const char *name)
{
    return (strcmp(".", name) != 0) && (strcmp("..", name) != 0) && 
    (strcmp("./", name) != 0) && (strcmp("../", name) != 0);
}

static int StrCmpCaseIgnore(const char *s1, const char *s2)
{

    while (*s1 && (tolower(*s1) == tolower(*s2)))
		{
		    ++s1;
			++s2;
		}

	return (tolower(*s1) - tolower(*s2));
}
