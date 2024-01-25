#ifndef __ILRD_RD110_PRIORITY_QUEUE_HPP__
#define __ILRD_RD110_PRIORITY_QUEUE_HPP__

#include <queue>    // std::priority_queue

namespace ilrd
{

template<class T, class CONTAINER = std::vector<T>, class COMPARE = std::less<typename CONTAINER::value_type> >
class PriorityQueue : private std::priority_queue<T,CONTAINER,COMPARE>
{
typedef std::priority_queue<T> pqueue;

public:
    using pqueue::empty;
    using pqueue::pop;
    using pqueue::push;
    using pqueue::size;
    
    inline const T &front() const throw();
    inline T& front() throw();

private:
};

template<class T, class CONTAINER, class COMPARE >
const T& PriorityQueue<T,CONTAINER,COMPARE>::front() const throw()
{
    return pqueue::top();
}

template<class T, class CONTAINER, class COMPARE >
T& PriorityQueue<T,CONTAINER,COMPARE>::front() throw()
{
    return const_cast<T&>(pqueue::top());
}

} // namespace ilrd

#endif // __ILRD_RD110_PRIORITY_QUEUE_HPP__
