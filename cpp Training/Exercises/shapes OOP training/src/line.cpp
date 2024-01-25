#include <iostream>
#include "line.hpp"

using namespace ilrd;

Line::Line(size_t position):Shape(position)
{
}
void Line::Draw() const
{
    MovePrintToPosition();
    std::cout << "line\n";
}