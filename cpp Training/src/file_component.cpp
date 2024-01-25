




#include "file_component.hpp"

namespace ilrd
{

FileComponent::FileComponent(const std::string& name_, size_t offset_) throw()
                                                     
:   m_name(name_),
    m_offset(offset_)
{}

} //namespace ilrd