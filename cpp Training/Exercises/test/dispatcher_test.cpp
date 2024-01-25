#include "dispatcher.hpp"
#include <string>

using namespace ilrd;

struct notify_t
{
    void operator() (std::string &)
    {

    }
};

struct on_destruct_t
{
    void operator() (void)
    {

    }
};

int main()
{
    notify_t notif_1, notif_2;
    on_destruct_t destr_1, destr_2;

    Dispatcher<std::string> dispatch;

    CallBack<std::string> observ1(dispatch, notif_1, destr_1);
    CallBack<std::string> observ2(dispatch, notif_2, destr_2);

    dispatch.Subscribe(&observ1);
    dispatch.Subscribe(&observ2);

    dispatch.Notify("HELLO");

    return 0;
}