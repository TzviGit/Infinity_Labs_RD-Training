/**********************************************************
 * 
 * dispatcher_test.cpp - dispatcher testing
 * 
 * Author : Tuvia
 *
 * Reviewed and approved by : ???
 * 
 * 01-02-22
 * 
 * OLRD 110 Ramat Gan
 * 
 * *******************************************************/

#include <iostream> // cout

#include "util.hpp" // nullptr
#include "dispatcher.hpp"


#define _RED_    "\033[1;31m"
#define _GREEN_  "\033[1;32m"
#define _YELLOW_ "\033[1;33m"
#define _PURPLE_ "\033[1;35m"
#define _CYAN_   "\033[1;36m"
#define _WHITE_  "\033[0m"


using namespace ilrd;


typedef void (*func_t)(int);


class Observer
{
public:
    void Reset()       { m_num = 0;     }
    void Inc(int val_) { m_num += val_; }
    void Set(int val_) { m_num = val_;  }
    int Get()          { return m_num;  }
    ~Observer() {}

private:
    int m_num;
};


static const int s_default = 99;

static Observer ob1;
static Observer ob2;
static Observer ob3;
static Observer ob4;


static void TestDispatcherNotify();
static void TestDispatcherDieBeforeCallback();
static void TestCallbackDieBeforeDispatcher();
static void TestDispatcherInvalidCallFunction();

static void ResetObservers();
static void PrintResults(int a1, int a2, int a3, int a4, const char *title);
static void PrintTitle(const char *str1, const char *str2);

static void IncObserver1(int param);
static void IncObserver2(int param);
static void IncObserver3(int param);
static void IncObserver4(int param);

static void SetToDefault1(void);
static void SetToDefault2(void);
static void SetToDefault3(void);
static void SetToDefault4(void);



int main()
{
	TestDispatcherNotify();
	TestDispatcherDieBeforeCallback();
	TestCallbackDieBeforeDispatcher();
	TestDispatcherInvalidCallFunction();
	
	std::cout << std::endl;
	return 0;
}



static void TestDispatcherNotify()
{
    PrintTitle("Dispatcher", "Notify");
    
    Dispatcher<int> disp;
    
    CallBack<int> call1(disp, IncObserver1, SetToDefault1);
    CallBack<int> call2(disp, IncObserver2, SetToDefault2);
    CallBack<int> call3(disp, IncObserver3, SetToDefault3);
    CallBack<int> call4(disp, IncObserver4, SetToDefault4);
    
    ResetObservers();
    
    disp.Notify(1);
    PrintResults(1, 1, 1, 1, "notify 1 <1,1,1,1>");
    
    disp.Notify(2);
    PrintResults(3, 3, 3, 3, "notify 2 <1,1,1,1>");
    
    disp.Notify(3);
    PrintResults(6, 6, 6, 6, "notify 3 <1,1,1,1>");
    
    disp.Notify(2);
    PrintResults(8, 8, 8, 8, "notify 2 <1,1,1,1>");
}


static void TestDispatcherDieBeforeCallback()
{
    PrintTitle("Dispatcher", "Notify On-Destruct");
    
    Dispatcher<int> *disp = new Dispatcher<int>;
    
    CallBack<int> call1(*disp, IncObserver1, SetToDefault1);
    CallBack<int> call2(*disp, IncObserver2, SetToDefault2);
    CallBack<int> call3(*disp, IncObserver3, SetToDefault3);
    CallBack<int> call4(*disp, IncObserver4, SetToDefault4);
    
    ResetObservers();
    
    delete disp; // Dispatcher Dtor
    
    PrintResults(99, 99, 99, 99, "Dispatcher died");
    
    // CallBack Dtor
}


static void TestCallbackDieBeforeDispatcher()
{
    PrintTitle("Dispatcher", "CallBack die before dispatcher");
    
    Dispatcher<int> *disp = new Dispatcher<int>;
    
    CallBack<int> *c1 = new CallBack<int>(*disp, IncObserver1, SetToDefault1);
    CallBack<int> *c2 = new CallBack<int>(*disp, IncObserver2, SetToDefault2);
    CallBack<int> *c3 = new CallBack<int>(*disp, IncObserver3, SetToDefault3);
    CallBack<int> *c4 = new CallBack<int>(*disp, IncObserver4, SetToDefault4);
    
    ResetObservers();
    
    disp->Notify(1);
    PrintResults(1, 1, 1, 1, "notify 1 <1,1,1,1>");
    
    delete c2;
    disp->Notify(1);
    PrintResults(2, 1, 2, 2, "notify 1 <1,0,1,1>");
    
    delete c3;
    disp->Notify(1);
    PrintResults(3, 1, 2, 3, "notify 1 <1,0,0,1>");
    
    delete c1;
    disp->Notify(1);
    PrintResults(3, 1, 2, 4, "notify 1 <0,0,0,1>");
    
    delete c4;   // CallBack Dtor
    delete disp; // Dispatcher Dtor
    
    PrintResults(3, 1, 2, 4, "Dispatcher   died ");
}


static void TestDispatcherInvalidCallFunction()
{
    PrintTitle("Dispatcher", "CallBack functions are NULL");
    
    Dispatcher<int> *disp = new Dispatcher<int>;
    
    CallBack<int> call1(*disp, IncObserver1, nullptr);
    CallBack<int> call2(*disp, nullptr, SetToDefault2);
    CallBack<int> call3(*disp, IncObserver3, nullptr);
    CallBack<int> call4(*disp, nullptr, SetToDefault4);
    
    ResetObservers();
    
    disp->Notify(1);
    PrintResults(1, 0, 1, 0, "notify 1 <1, nil, 1, nil>");
    
    disp->Notify(2);
    PrintResults(3, 0, 3, 0, "notify 2 <1, nil, 1, nil>");
    
    delete disp;
    PrintResults(3, 99, 3, 99, "   Dispatcher    died    ");
}




///////////////////////////////////////////////////////////
//                        Utils                          //
///////////////////////////////////////////////////////////

static void ResetObservers()
{
    ob1.Reset();
    ob2.Reset();
    ob3.Reset();
    ob4.Reset();
}


static void PrintResults(int a1, int a2, int a3, int a4, const char *title)
{
    std::cout << _YELLOW_ << title << ": ";
    
    std::cout << ((ob1.Get() == a1) ? _GREEN_ : _RED_);
    std::cout << ob1.Get() << "  ";
    std::cout << ((ob2.Get() == a2) ? _GREEN_ : _RED_);
    std::cout << ob2.Get() << "  ";
    std::cout << ((ob3.Get() == a3) ? _GREEN_ : _RED_);
    std::cout << ob3.Get() << "  ";
    std::cout << ((ob4.Get() == a4) ? _GREEN_ : _RED_);
    std::cout << ob4.Get() << "  ";
    
    std::cout << _WHITE_ << std::endl;
}


static void PrintTitle(const char *str1, const char *str2)
{
    std::cout << std::endl << _PURPLE_ << str1 << " - " 
        _CYAN_ << str2 << _WHITE_ << std::endl;
}





///////////////////////////////////////////////////////////
//                   Event functions                     //
///////////////////////////////////////////////////////////


static void IncObserver1(int param)
{ ob1.Inc(param); }

static void IncObserver2(int param)
{ ob2.Inc(param); }

static void IncObserver3(int param)
{ ob3.Inc(param); }

static void IncObserver4(int param)
{ ob4.Inc(param); }


static void SetToDefault1(void)
{ ob1.Set(s_default); }

static void SetToDefault2(void)
{ ob2.Set(s_default); }

static void SetToDefault3(void)
{ ob3.Set(s_default); }

static void SetToDefault4(void)
{ ob4.Set(s_default); }




