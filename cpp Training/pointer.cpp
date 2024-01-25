#include <iostream>

class A
{
public:
    void fun(int x) 
    {
        std::cout << "fun here, x: " << x << std::endl;
        
    }

    void (A::*fun_ptr)(int) ;
};

int main()
{
    typedef void (*ptr)(A *const, int)  ;
    A a;
    ptr t= (ptr)456;
    a.fun_ptr = &A::fun;
    std::cout << "a.fun_ptr: ";
    (a.*a.fun_ptr)(5);
    std::cout << "\n" << t << "\n";

    return 0;
}