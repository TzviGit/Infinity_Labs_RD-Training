#include <iostream>
#include "circle.hpp"

using namespace ilrd;

Circle::Circle(size_t position):Shape(position)
{
}

void Circle::Draw() const
{
    MovePrintToPosition();
    std::cout << "circle\n";
}