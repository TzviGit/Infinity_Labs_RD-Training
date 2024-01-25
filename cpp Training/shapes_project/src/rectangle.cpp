#include <iostream>
#include "rectangle.hpp"

using namespace ilrd;

Rectangle::Rectangle(size_t position):Shape(position)
{
}

void Rectangle::Draw() const
{
    MovePrintToPosition();
    std::cout << "rectangle\n";
}