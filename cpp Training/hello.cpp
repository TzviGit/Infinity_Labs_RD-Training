#include<iostream>
// void Foo();
// void Foo(int i);

// void Foo(char c);
// inline void Tzvi()
// {
//     std::cout << "hello-world\n";
// }
namespace useless
{
    unsigned int g_wasteful[400];
    extern int g_notthere;
    void Foo() {}

    namespace wasteoftyme
    {
        void Foo() {}
    } // namespace wasteoftyme
} // namespace useless


namespace stupid
{
    void Foo() {}
    void Bar() {}
    void DoNothing(unsigned int) {}
} // namespace stupid

namespace useless
{
    void DoNothing(int) {}
} // namespace useless


using namespace useless;

void DoStuff()
{
    stupid::Bar();
    Foo();
    using stupid::Foo;
    Foo();
    DoNothing(g_wasteful[3] + 1);
}

void Foo() {}

using namespace stupid;


namespace cameon = useless::wasteoftyme;


void DoMoreStuff()
{
    cameon::Foo();
    // Foo(); // try uncommenting this line, solve the error
    Bar(); // why doesn't this line create an error
    DoNothing(g_wasteful[3] + 1);
}


namespace useless
{
    void DoUselessStuff()
    {
        DoNothing(g_wasteful[3] + 1);
    }
} // namespace useless


int main (int argc, char *argv[])
{
    //Foo();
   /*  Foo(8,8,97); */
    //Foo(static_cast< unsigned char>(97));
    //Tzvi();
    char buff[110] = {0};
    sprintf(buff, "nm %s -C", argv[0]);
    system(buff);
    return 0;
}

// void Foo()
// {
//     std::cout << "inside the voin func!  \n";
// }

// void Foo(int i)
// {
//     printf(" inside crazy func  %d  \n", i);
// }

// void Foo(char i)
// {
//     std::cout << "inside the char func : " << i << std::endl;
// }