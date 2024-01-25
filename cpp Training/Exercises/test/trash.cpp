#include <boost/core/noncopyable.hpp> // boost::noncopyable
#include "scope_lock.hpp"
#include "util.hpp"
#include <boost/thread/mutex.hpp>

#include <iostream>
int *ptr = NULL;
void deletePtr()
{
    delete[] ptr;
}
int main()
{
    ptr = new int[300];
    atexit(deletePtr);
    return 0;
}