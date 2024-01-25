#include "boost/thread/thread.hpp"
#include <iostream> //cout
#include "task.hpp" //API for this we are testing

using namespace ilrd;

int Foo(double d)
{
    std::cout << " HELLO THERE.... \n ";
    return d;
}
boost::shared_ptr<Future<int> > ptr(new Future<int>);
    Task<int, double> t(Foo, 7);
    TaskWithFuture<int, double> twf(Foo, 165.3, ptr);

void Thread()
{
    twf.Run();
}
int main()
{
    TaskWithFuture<int, double> twf2(Foo, 19, ptr);
    boost::thread tr1(Thread );
   ///twf2.Run();
   // t.Run();
    tr1.join();
    std::cout << ".. .. THE VALUE IS .. .. -> " << ptr->Get() << "\n";
    twf2.Run();
    std::cout << ".. .. THE VALUE IS .. .. -> " << ptr->Get() << "\n";


  

    return 0;
}