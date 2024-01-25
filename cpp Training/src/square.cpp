#include <iostream>
#include "square.hpp"

namespace ilrd
{

Square::Square(size_t position):Shape(position)
{
    
}

void Square::Draw() const
{
    MovePrintToPosition();
    std::cout << "square\n";
}

} // namespace ilrd