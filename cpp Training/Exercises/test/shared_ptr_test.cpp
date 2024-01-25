// #include "shared_ptr.hpp"
// #include <iostream>

// using namespace ilrd;
// int main()
// {
//     //std::cout << "HELLO HELLO HELLO\n";
//     int *ptr = new int(53);
//     SharedPtr<int> shared(ptr);
//     return 0;
// }


#include <iostream> // cout

#include "shared_ptr.hpp"

#define _RED_    "\033[1;31m"
#define _GREEN_  "\033[1;32m"
#define _YELLOW_ "\033[1;33m"
#define _BLUE_   "\033[1;34m"
#define _PURPLE_ "\033[1;35m"
#define _CYAN_   "\033[1;36m"
#define _WHITE_  "\033[0m"


using namespace ilrd;



class Base
{
public:
    Base(int num_ = 1) : m_base(num_) {}
    virtual ~Base() {}
    virtual int Get() { return m_base; }

private:
    int m_base;
};


class Derived : public Base
{
public:
    Derived(int num1_ = 1, int num2_ = 2) : Base(num1_), m_derived(num2_) {}
    virtual ~Derived() {}
    virtual int Get() { return m_derived; }

private:
    int m_derived;
};



static void TestSharedPtr();

static void TestCtor();
static void TestCctor();
static void TestAssignment();
static void TestUpCast();
static void TestDtor();

static void PrintResults(const void *result,
    const void *expect, const char *title);
static void PrintResults(int result,
    int expect, const char *title);


int main()
{
	TestSharedPtr();
	
	std::cout << std::endl;
	return 0;
}


static void TestSharedPtr()
{
    TestCtor();
    TestCctor();
    TestAssignment();
    TestUpCast();
    TestDtor();
}


static void TestCtor()
{
    std::cout << std::endl << _PURPLE_ <<
        "Test - SharedPtr Ctor" << _WHITE_ << std::endl;
    
    char *str = const_cast<char *>("hello world");
    SharedPtr<int> p_int(new int(1));
    SharedPtr<char *> p_str(new char *(str));
    SharedPtr<Base> p_base(new Base(2));
    SharedPtr<Derived> p_derived(new Derived(3, 4));
    
    PrintResults(*p_int,             1, "<int> operator*()");
    PrintResults(*p_str,           str, "<char *> operator*()");
    PrintResults(p_base->Get(),      2, "<Base> operator->()");
    PrintResults((*p_base).Get(),    2, "<Base> operator*()");
    PrintResults((*p_derived).Get(), 4, "<Derived> operator*()");
    PrintResults(p_derived->Get(),   4, "<Derived> operator->()");
}


static void TestCctor()
{
    std::cout << std::endl << _PURPLE_ <<
        "Test - SharedPtr Copy Ctor" << _WHITE_ << std::endl;
    
    SharedPtr<int> p_int(new int(1));
    SharedPtr<Base> p_base(new Base(2));
    SharedPtr<Derived> p_derived(new Derived(3, 4));
    
    SharedPtr<int> p_int_copy(p_int);
    SharedPtr<Base> p_base_copy(p_base);
    SharedPtr<Derived> p_derived_copy(p_derived);
    
    PrintResults(*p_int_copy,             1, "<int> operator*()");
    PrintResults(p_base_copy->Get(),      2, "<Base> operator->()");
    PrintResults((*p_base_copy).Get(),    2, "<Base> operator*()");
    PrintResults((*p_derived_copy).Get(), 4, "<Derived> operator*()");
    PrintResults(p_derived_copy->Get(),   4, "<Derived> operator->()");
}


static void TestAssignment()
{
    std::cout << std::endl << _PURPLE_ <<
        "Test - SharedPtr Assignment" << _WHITE_ << std::endl;
    
    SharedPtr<int> p_int(new int(1));
    SharedPtr<Base> p_base(new Base(2));
    SharedPtr<Derived> p_derived(new Derived(3, 4));
    
    SharedPtr<Base> p_base_copy = p_base;
    SharedPtr<Derived> p_derived_copy = p_derived;
    
    PrintResults(p_base_copy->Get(),      2, "<Base> operator->()");
    PrintResults((*p_base_copy).Get(),    2, "<Base> operator*()");
    PrintResults((*p_derived_copy).Get(), 4, "<Derived> operator*()");
    PrintResults(p_derived_copy->Get(),   4, "<Derived> operator->()");
    
    SharedPtr<int> p_int_copy, p_int_copy2;
    p_int_copy2 = p_int_copy = p_int;
    
    PrintResults(*p_int_copy,  1, "<int> p2 = p1");
    PrintResults(*p_int_copy2, 1, "<int> p3 = p2 = p1");
    
    p_int = p_int;
    PrintResults(*p_int, 1, "<int> p1 = p1");
}


static void TestUpCast()
{
    std::cout << std::endl << _PURPLE_ <<
        "Test - SharedPtr Up-Cast" << _WHITE_ << std::endl;
    
    SharedPtr<Derived> p_derived(new Derived(1, 2));
    
    SharedPtr<Base> p_base_copy(p_derived);
    PrintResults(p_base_copy->Get(),   2, "<Base> b(<Derived> d)");
    
    SharedPtr<Base> p_base_assign = p_derived;
    PrintResults(p_base_assign->Get(), 2, "<Base> b = <Derived d>");
    
    // this code should not compile: Base isn't-a Derived
    /*
    SharedPtr<Base> p_base_check(new Base(1));
    SharedPtr<Derived> p_derived_check(p_base_copy);
    */
}


static void TestDtor()
{
    std::cout << std::endl << _PURPLE_ <<
        "Test - SharedPtr Dtor" << _WHITE_ << std::endl;
    
    {
    SharedPtr<int> p_int;
    SharedPtr<int> ptrs[] = {p_int, p_int, p_int};
    
    SharedPtr<int> p_int_other;
    
    ptrs[0] = p_int_other;
    ptrs[1] = p_int_other;
    ptrs[2] = p_int_other;
    }
    
    std::cout << _CYAN_ << "check valgrind!" << _WHITE_ << std::endl;
}




// ****** UTILS ******

static void PrintResults(const void *result,
    const void *expect, const char *title)
{
    std::cout << ((result == expect) ? _GREEN_ : _RED_);
	std::cout << title << ": " << result << _WHITE_ << std::endl;
}


static void PrintResults(int result,
    int expect, const char *title)
{
    std::cout << ((result == expect) ? _GREEN_ : _RED_);
	std::cout << title << ": " << result << _WHITE_ << std::endl;
}






