/********************************************************************
    Author:         Tzvi Moskowitz
    Company:		Infinity
    Reviewer:       Nophar
    Date:           Jan 2, 2022
    Group:          OL110
    Description:    Reference counting string (imlementing a "copy on write" aproach)

 * *****************************************************************/


#include <cstring>
#include <exception>     //throw, try , catch
#include <iostream>      //coot , cin
#include "reference_count_string.hpp"  // API for this module
#include <cstddef>

namespace ilrd
{
    
static const int NULL_TERM = 1;

struct RCStringData
{
    size_t m_counter;
    char m_cstr[1];

    
    // ~RCStringData();
};

 RCString::RCString(const char *str) throw (std::bad_alloc)
 : m_data(CreateStringData(str))
 {

 }

RCStringData* RCString::CreateStringData(const char *str) 
{
    size_t length = strlen(str) + NULL_TERM;
    size_t offset = offsetof( RCStringData, m_cstr);
    
    RCStringData *data = static_cast<RCStringData *>
                    (operator new(offset + length * sizeof(char)));
    
    data->m_counter = 1;
    memcpy(data->m_cstr, str, length);

    return data;
}

void RCString::ProperlyAttach(const RCString& other_)
{
    m_data = other_.m_data;
    ++(m_data->m_counter);
}

RCString::RCString(const RCString& other_)
{
    ProperlyAttach(other_);
    // m_data = other_.m_data;
    // ++(m_data->m_counter);
}



void RCString::ProperlyDetach()
{

    if (0 == --(m_data->m_counter))
    {
        delete m_data;
    }
    
    m_data = NULL ;
}

RCString::~RCString()
{
    ProperlyDetach();
}

RCString& RCString::operator=(const RCString& rhs_)
{
    if (*this != rhs_)
    {
        ProperlyDetach();
        ProperlyAttach(rhs_);
    }
    return *this;
}

char RCString::operator[](size_t index) const
{
    return m_data->m_cstr[index];
}


char& RCString::operator[](size_t index) throw (std::bad_alloc)
{
    
     if (m_data->m_counter > 1)
    {
        RCStringData *data = CreateStringData(Cstr());
        ProperlyDetach();

        m_data = data;
    }

    return m_data->m_cstr[index];

}

size_t RCString::Length() const
{
    return strlen(Cstr());
}
 

bool operator==(const RCString& lhs_, const RCString& rhs_)
{
    return (strcmp(lhs_.Cstr(), rhs_.Cstr()) == 0);
}

bool operator!=(const RCString& lhs_, const RCString& rhs_)
{
    return !(lhs_ == rhs_);
}

bool operator>(const RCString& lhs_, const RCString& rhs_)
{
    return (strcmp(lhs_.Cstr(), rhs_.Cstr()) > 0);
}

bool operator<(const RCString& lhs_, const RCString& rhs_)
{
    return (strcmp(lhs_.Cstr(), rhs_.Cstr()) < 0);
}

std::ostream& operator<<(std::ostream& os_, const RCString& rhs_)
{
    return os_ << rhs_.Cstr();
}

std::istream& operator>>(std::istream& is_, RCString& rhs_) 
{                                                        
    const int MAX_BUFFER_SIZE = 1000;
    char buffer[MAX_BUFFER_SIZE];
    is_.getline(buffer, MAX_BUFFER_SIZE);

    rhs_ = buffer;

    return is_;
}



const char *RCString::Cstr() const
{
    return m_data->m_cstr;
}



} // namespace ilrd