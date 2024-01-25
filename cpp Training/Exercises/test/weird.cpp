 #include <boost/core/noncopyable.hpp>
#include <queue>
#include <boost/shared_ptr.hpp>
#include <iostream>
 class Base : private boost::noncopyable
 {
public:

    ~Base()
    {
        std::cout << "DESTRUCTOR\n";
    }
    int x;
 };

 int main()
 {
     //Base b;
     std::queue<boost::shared_ptr<Base> > vec;
     boost::shared_ptr<Base>  ptr(new Base);
     vec.push(ptr);
     (vec.front() = ptr);
     return 0;
 }