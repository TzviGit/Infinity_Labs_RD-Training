#ifndef __ILRD_RD110_RECTANGLE_HPP__
#define __ILRD_RD110_RECTANGLE_HPP__

#include "shape.hpp"

namespace ilrd
{

class Rectangle: public Shape
{
public:
    Rectangle(size_t position = 0);
    void Draw() const;
};

} // namespace ilrd

#endif // __ILRD_RD110_RECTANGLE_HPP__
