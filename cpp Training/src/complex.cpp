#include "complex.hpp"

using namespace ilrd;
int main()
{
    Complex z1(2, 4);
    Complex z2 = z1;
    Complex z3 = z1 * z2;

    std::cout << z3 << "\n";
    std::cout << z1 << "\n";
    std::cout << z2 << "\n";
    return 0;
}