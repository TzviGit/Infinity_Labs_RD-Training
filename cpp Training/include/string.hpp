/* ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ *                                         *
 ~ Author        Tzvi Moskowitz                                 ~
 * Version       1.0.0                                           *
 ~ Date          12/13/2021                                      ~
 * Reviewer      Yinon                                           *
 ~ Description   String API                                      ~
 * Group         R&D 110                                         *
 ~ Company       ILRD Ramat Gan                                  ~
 * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ */

#ifndef __ILRD_RD110_STRING_HPP__
#define __ILRD_RD110_STRING_HPP__
#include <cstring>
#include <cstddef> /* size_t */
#include <iostream> //ostream

class String
{
public:
    String(const char *str = ""); // not explicit on purpose
    String(const String& other_); // not explicit on purpose
    ~String();
    String& operator=(const String& rhs_);

    bool operator==(const String& rhs_) const;
    bool operator >(const String& rhs_) const;
    bool operator <(const String& rhs_) const;
    String operator+(const String& rhs_)const;
    char operator[](size_t index)const;
    

    size_t Length() const;
    const char *Cstr() const;
    
private:
    
    char *m_cstr;
};

bool operator==(const char *other_, const String& this_);
std::ostream& operator<<(std::ostream& os_, const String& str_);

#endif //__ILRD_RD110_STRING_HPP__