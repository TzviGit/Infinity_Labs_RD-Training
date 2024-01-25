
using namespace std;
#include <iostream>
#include <cstdlib>
#include <string.h>
#include <cstdio>
class X
{
public:
    X()
    {
        cout << "CTOR\n\n";
    }

    X(const X& x)
    {
        memcpy(arr, x.arr, 10000);
        cout << "Copy->CTOR\n\n";
    }

    ~X()
    {
        cout << "Destructor\n\n";
    }
    int arr[100000];
};

X& Foo(X& x)
{
    puts("in foo!\n");
    x.arr[5] = 10;
    puts("leaving foo\n");
    return x;
}

int main ()
{
    X x1;

    // printf("got here: line -> %d\n", __LINE__);
    // X x2;
   
   printf("got here: line -> %d\n", __LINE__);
   X x2 = Foo(x1);

    return 0;
}