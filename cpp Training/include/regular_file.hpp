/********************************************************
 * 
 * File  API
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

#ifndef __RD_110_FILE_H__
#define __RD_110_FILE_H__

#include <string>   /* std::string API   */
#include "file_component.hpp"

namespace ilrd
{

class File : public FileComponent
    {
    public:
        explicit File(const std::string &name_, size_t offset_ = 0) throw();
        ~File() throw(){}
        virtual void Print() const throw();

        inline bool IsBinary()
        {
            return m_isBinary;
        }
        
        inline void SetIsBinary( bool b)
        {
            m_isBinary = b;
        }

    private:
        bool m_isBinary;
    };

} //namespace ilrd

#endif // __RD_110_FILE_H__