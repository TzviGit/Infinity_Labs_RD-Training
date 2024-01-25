/* ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ *                                         
 ~ Author        TZvi Moskowitz                                  ~
 * Reviewer       TODO: Nofar                                    
 ~ Description   Simple Strings source file  
 * Date December 13, 2021                      
 * Group         OL110                                           *
 ~ Company       ILRD Ramat Gan                                  ~
 * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ */

#include <cstring>
#include <cstdlib>
#include <cassert>
#include "string.hpp"


/************************************************
 * ******* CONSTRUCTORS  ***********************/

String::String(const char *str):m_cstr{nullptr}
{
    size_t length = strlen(str) + 1;
    m_cstr = new char[length];
    memmove(m_cstr, str, length);
}
       
/**** copy constructor ***/

String::String(const String& other_):m_cstr{nullptr}
{
    size_t length = other_.Length() + 1;
    m_cstr = new char[length];
    memmove(m_cstr, other_.Cstr(), length);
}

/********** DESTRUCTOR **********/
String::~String()
{
    delete[] this->m_cstr;
    m_cstr = nullptr;
}

/*****************************************
********** MEMBER FUNCS *****************/

size_t String::Length() const
{
    return strlen(Cstr());
}

const char *String::Cstr() const
{
    return m_cstr;
}

/**********************************************
*     __OPERATOR OVERLOADERS__
**********************************************/
String& String::operator=(const String& rhs_)
{
    if (Length() < rhs_.Length())
    {
        delete[] m_cstr;
        m_cstr = new char[rhs_.Length() + 1]; 
    }
    
    memmove(m_cstr, rhs_.Cstr(), rhs_.Length() + 1);

    return *this;
}

bool String::operator==(const String& rhs_) const 
{
    return (0 == strcmp(Cstr(), rhs_.Cstr()));
}

bool String::operator >(const String& rhs_) const
{
    return (strcmp(Cstr(), rhs_.Cstr()) > 0);
}

bool String::operator <(const String& rhs_) const
{
    return (strcmp(Cstr(), rhs_.Cstr()) < 0);
}

String String::operator+(const String& rhs_) const
{
    size_t newSize = Length() + rhs_.Length() + 1;
    char *newString = new char[newSize];
    strcpy(newString, m_cstr);
    strcat(newString, rhs_.Cstr());

    String result(newString);
    delete[] newString;

    return result;
}

char String::operator[](size_t index) const
{
    assert(index <= Length());
    return m_cstr[index];
}

/******************************************************** 
*       The NON_MEMBER Funcs
*********************************************************/

bool operator==(const char *leftSide_, const String& thisOnRight_)
{
    return 0 == strcmp(leftSide_, thisOnRight_.Cstr());
}

std::ostream& operator<<(std::ostream& os_, const String& str_)
{
    return os_ << str_.Cstr();
}