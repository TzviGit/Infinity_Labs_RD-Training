#include "dllLoader.hpp"
#include <iostream>
using namespace ilrd;

int main()
{
    DirMonitor bob("/home/motzvi/git/cpp/projects/final_projects/framework/shachar");
    DllLoader alice(bob);

    system("mv libshared.so shachar/");

    sleep(1);

    
    typedef int (*foo)(int);
    

    void *temp_holder = alice.LoadFunction("FOO");
    try
    {
        foo f1 = (foo)temp_holder;
        std::cout << f1(4) <<"\n";
    }
    catch(...)
    {
        std::cout << "Sorry - Function whas NOT loadable ....\n";
    }
    
        system("mv shachar/libshared.so .");


    return 0;
}