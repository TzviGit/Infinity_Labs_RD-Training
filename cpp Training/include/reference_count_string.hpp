/*  ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~
* Version       1.0.0                                           *
~ Date          16/12/2021                                      ~
~ Description   RCString API                                    ~
* Group         R&D 110                                         *
~ Company       ILRD Ramat Gan                                  ~
* ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ */

#ifndef __ILRD_RD110_RCSTRING_HPP__
#define __ILRD_RD110_RCSTRING_HPP__

#include <cstddef>      // size_t
#include <iostream>     // ostream, istream

namespace ilrd
{

    struct RCStringData;

    class RCString
    {
    public:
        RCString(const char *str = "") throw(std::bad_alloc); // not explicit on purpose
        RCString(const RCString &other_);
        ~RCString();
        RCString &operator=(const RCString &rhs_);

        char operator[](size_t index) const;
        char &operator[](size_t index) throw(std::bad_alloc);

        size_t Length() const;
        const char *Cstr() const;
        // RCStringData *GetData() const { return m_data; }
        RCStringData *CreateStringData(const char *);

    private:
        RCStringData *m_data;
        void ProperlyDetach();
        void ProperlyAttach(const RCString &other_);
        static void DecreaseCounter(RCString *str);
};

std::ostream& operator<<(std::ostream& os_, const RCString& rhs_);
std::istream &operator>>(std::istream &is_, RCString &rhs_);

bool operator==(const RCString& lhs_, const RCString& rhs_);
bool operator!=(const RCString& lhs_, const RCString& rhs_);
bool operator>(const RCString& lhs_, const RCString& rhs_);
bool operator<(const RCString& lhs_, const RCString& rhs_);


} // ilrd

#endif // __ILRD_RD110_RCSTRING_HPP__
