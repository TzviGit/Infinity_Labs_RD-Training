#include <iostream>
#include <cstring>

#include "string.hpp"

using std::cout;
using std::endl;

#define S_MSG(func) "SUCCESS: " #func " works as expected!"

/* ~ * ~ * ~ * ~ * ~ * ~ DECLARATIONS ~ * ~ * ~ * ~ * ~ * ~ */

void StringTest();
void LenTest(const char *test = "");
void CstrTest(const char *test = "");
void CctorTest(const char *test = "");
void AssignTest(const char *test = "");
void ToStrTest(const char *test = "");
void IndexTest(const char *test = "");
void CmpTest(const char *test1 = "", const char *test2 = "", char op = '=');

/* ~ * ~ * ~ * ~ * ~ * IMPLEMENTATION ~ * ~ * ~ * ~ * ~ * ~ */
int main()
{
    cout << "\t\t~ = SJ String test = ~\n\n";

    StringTest();

    cout << "\nThanks for using SJ String Test!\n\t~ = have a lovely day = ~\n";
}

void StringTest()
{
    String s("testify");

    cout << "Printing test: " << s << endl;
    "testify" == s ? cout << "comparison works!" << endl :
                     cout << "comparison fails" << endl;
    
    LenTest();
    LenTest("Hi");
    LenTest("\aZingamawhooper\t\n");

    CstrTest();
    CstrTest("Hi");
    CstrTest("\aZingamawhooper\t\n");

    CctorTest();
    CctorTest("Hi");
    CctorTest("\aZingamawhooper\t\n");

    AssignTest();
    AssignTest("Hi");
    AssignTest("\aZingamawhooper\t\n");

    ToStrTest();
    ToStrTest("Hi");
    ToStrTest("\aZingamawhooper\t\n");

    IndexTest();
    IndexTest("Hi");
    IndexTest("\aZingamawhooper\t\n");

    CmpTest("a", "b", '<');
    CmpTest("b", "a", '>');
    CmpTest("a", "a", '=');
}

void LenTest(const char *test)
{
    String str(test);

    strlen(test) == str.Length() ?
        cout << S_MSG(Length) << endl :
        cout << "FAILED to get Length of " << strlen(test) << ", instead got "
                << str.Length() << endl;
}

void CstrTest(const char *test)
{
    String str(test);

    strcmp(str.Cstr(), test) ?
        cout << "FAILED at Cstr, got " << str.Cstr() << " instead of "
                << test << endl :
        cout << S_MSG(Cstr) << endl;
}

void CctorTest(const char *test)
{
    String str(test);
    String cpy(str);

    strcmp(cpy.Cstr(), test) ?
        cout << "FAILED at Cstr, got " << cpy.Cstr() << " instead of "
                << test << endl :
        cout << S_MSG(Cctor) << endl;
}

void AssignTest(const char *test)
{
    String str(test);
    String cpy = str;
    str = str;

    strcmp(cpy.Cstr(), test) ?
        cout << "FAILED at Cstr, got " << cpy.Cstr() << " instead of "
                << test << endl :
        cout << S_MSG(Assign) << endl;
}

void ToStrTest(const char *test)
{
    String str(test);

    strcmp(str.Cstr(), test) ?
        cout << "FAILED at Cstr, got " << str.Cstr() << " instead of "
                << test << endl :
        cout << S_MSG(ToStr) << endl;
}

void IndexTest(const char *test)
{
    String str(test);
    size_t len = str.Length();

    for (size_t i = 0; i < len; ++i)
    {
        if (str[i] != test[i])
        {    
            cout << "FAILED to get str[" << i << "], got " << str[i]
                    << " instead of " << test[i] << endl;
            return;
        }            
    }
    cout << S_MSG(Index) << endl;
}

void CmpTest(const char *test1, const char *test2, char op)
{
    String s1(test1);
    String s2(test2);

    switch (op)
    {
    case '>':
        s1 > s2 ? cout << S_MSG(> operator) << endl :
                  cout << "FAILED to properly use > operator" << endl;
        break;
    case '<':
        s1 < s2 ? cout << S_MSG(< operator) << endl :
                  cout << "FAILED to properly use < operator" << endl;
        break;
    case '=':
        s1 == s2 ? cout << S_MSG(= operator) << endl :
                  cout << "FAILED to properly use = operator" << endl;
        break;
    default:
        cout << "ERROR: unexpected operator" << endl;             
    }
}