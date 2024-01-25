#include "priority_queue.hpp"
#include <iostream>
using namespace ilrd;
int main()
{
    PriorityQueue<int> pq;

    for (size_t i = 0; i < 10; i++)
    {
        pq.push(9 -i);/* code */
    }

    for (size_t i = 0; i < 10; i++)
    {
        /* code */
        std::cout << pq.front() << std::endl;
        pq.pop();
    }

    return 0;
}