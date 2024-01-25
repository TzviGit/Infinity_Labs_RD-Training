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
#include <vector>     /* std::vector API    */

namespace ilrd
{
    
class FileComponent
{
public:
    explicit FileComponent(const std::string& name_ = ".", size_t offset_ = 0); 
    virtual ~FileComponent();
    virtual void Print() const = 0;
    std::string GetName() const;
    
private:
    std::string m_name;
    size_t m_offset;
};

class File : public FileComponent
{
public:
    explicit File(std::string name_, size_t offset_ = 0);
    ~File();
    virtual void Print() const;
};

class Directory : public FileComponent
{
public:
    explicit Directory(std::string name_, size_t offset_ = 0);
    ~Directory();
    virtual void Print() const;

private:
    std::vector<FileComponent *> m_files;    
};
    
} // ilrd

#endif /* __RD_110_FILE_COMPONENT_H__ *