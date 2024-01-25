/********************************************************
 * 
 * File Component API
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

#ifndef __RD_110_FILE_COMPONENT_H__
#define __RD_110_FILE_COMPONENT_H__

#include <string>   /* std::string API   */

namespace ilrd
{
    
class FileComponent
{
public:
    explicit FileComponent(const std::string& name_ = ".", size_t offset_ = 0) throw(); 
    virtual ~FileComponent() {}
    virtual void Print() const = 0;
    inline std::string GetName() const throw();
  
protected:
    inline size_t GetOffset() const throw();
    

private:
    std::string m_name;
    size_t m_offset;
};

inline size_t FileComponent::GetOffset() const throw()
{
    return m_offset;
}

inline std::string FileComponent::GetName() const throw()
{
    return m_name;
}

} // ilrd

#endif // __RD_110_FILE_COMPONENT_H__ 