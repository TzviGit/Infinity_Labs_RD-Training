#include <iostream>

#include "singleton_strange.hpp"

using namespace ilrd;

class car
{
public:
    car():ID(0){}
    size_t GetID() {return ID;}

    void SetID(size_t num) {ID += num;}
private:
    boost::atomic<size_t> ID;
};


void *Test(void *arg)
{
    car *p_car = Singleton<car>::GetInstance();
    p_car->SetID(1);
    std::cout << p_car->GetID() << std::endl;

    return NULL;
}
int main()
{
    pthread_t consumer_id[30] = {0};
    size_t num = 0;

    for (size_t i = 0; i < 30; ++i)
    {
        int status = pthread_create(&consumer_id[i], NULL, Test, &num);
        if (status != 0)
        {
            return status;
        }
    }

    for (size_t i = 0; i < 30; ++i)
    {
        int status = pthread_join(consumer_id[i], NULL);
        if (status != 0)
        {
            return status;
        }
    }
    return 0;
}
