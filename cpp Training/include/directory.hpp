/********************************************************
 * 
 * Directory  API
 * 
 * Author : RD-110
 * 
 * version: 1.0.0
 * 
 * Reviewed and approved by : Binyamin
 * 
 * 28-12-21
 * 
 * OLRD 110 Ramat Gan
 * 
 * *********************************************************/

#ifndef __RD_110_DIRECTORY_H__
#define __RD_110_DIRECTORY_H__

#include <string>   /* std::string API   */
#include <vector>  /* std::vector API   */
#include "file_component.hpp"  /* API for file component */

namespace ilrd
{

 class Directory : public FileComponent
    {
    public:
        explicit Directory(const std::string &name_ = ".",
                           size_t offset_ = 0) throw(std::bad_alloc);
        ~Directory() throw();
        virtual void Print() const throw();

    private:
        std::vector<FileComponent *> m_files;
    };
    
} // ilrd

#endif // __RD_110_DIRECTORY_H__