/**********************************************************
 * 
 * priority_queue_test.cpp - priority_queue testing
 * 
 * Author : Tuvia
 *
 * Reviewed and approved by : ???
 * 
 * 22-01-22
 * 
 * OLRD 110 Ramat Gan
 * 
 * *******************************************************/

#include <iostream> // cout
#include <queue>    // std::priority_queue

#include "priority_queue.hpp"


#define _RED_    "\033[1;31m"
#define _GREEN_  "\033[1;32m"
#define _YELLOW_ "\033[1;33m"
#define _PURPLE_ "\033[1;35m"
#define _CYAN_   "\033[1;36m"
#define _WHITE_  "\033[0m"


using namespace ilrd;


// ************ class Name ************

class Name
{
public:
    explicit Name(const char *str_) : m_str(str_) {}
    bool operator<(const Name& rhs_) const { return m_str < rhs_.m_str; }
    bool operator==(const Name& rhs_) const { return m_str == rhs_.m_str; }
    std::string Cstr() const { return m_str; }

private:
    std::string m_str;
};

std::ostream& operator<<(std::ostream& os_, const Name& rhs_)
{
    return os_ << rhs_.Cstr();
}

// ************ end of class ************


static void TestPriorityQueue();
static void TestPriorityQueueWithObjects();

template <typename T>
static void TestPop(PriorityQueue<T>& pq, const T& expect);
template <typename T>
static void TestEmpty(PriorityQueue<T>& pq, bool i);

static void PrintTitle(const char *str1, const char *str2);


int main()
{
	TestPriorityQueue();
	TestPriorityQueueWithObjects();
	
	std::cout << std::endl;
	return 0;
}


static void TestPriorityQueue()
{
    PrintTitle("Test - Priority Queue", "int");
    
    PriorityQueue<int> pq;
    
    pq.push(1);
    pq.push(3);
    pq.push(4);
    pq.push(2);
    
    TestPop(pq, 4);
    TestPop(pq, 3);
    TestPop(pq, 2);
    TestPop(pq, 1);
}



static void TestPriorityQueueWithObjects()
{
    PrintTitle("Test - Priority Queue", "Name");
    
    PriorityQueue<Name> pq;
    
    Name n1("col");
    Name n2("avi");
    Name n3("dan");
    Name n4("ben");
    
    pq.push(n1);
    pq.push(n2);
    pq.push(n3);
    pq.push(n4);
    
    TestPop(pq, Name("dan"));
    TestPop(pq, Name("col"));
    TestPop(pq, Name("ben"));
    TestPop(pq, Name("avi"));
}




// ************ Utils ************

template <typename T>
static void TestPop(PriorityQueue<T>& pq, const T& expect)
{
    T result = pq.front();
    pq.pop();
    
    std::cout << _YELLOW_ << "pop: ";
    std::cout << ((expect == result) ? _GREEN_ : _RED_);
    std::cout << result << _WHITE_ << std::endl;
}


template <typename T>
static void TestEmpty(PriorityQueue<T>& pq, bool i)
{
    const char *print = (pq.empty()) ? "true" : "false";
    
    std::cout << std::endl << _YELLOW_ << "empty: ";
    std::cout << ((pq.empty() == i) ? _GREEN_ : _RED_);
    std::cout << print << _WHITE_ << std::endl;
}


static void PrintTitle(const char *str1, const char *str2)
{
    std::cout << std::endl << _PURPLE_ << str1 << " - " 
        _CYAN_ << str2 << _WHITE_ << std::endl;
}






