#ifndef __ILRD_RD110_CIRCLE_HPP__
#define __ILRD_RD110_CIRCLE_HPP__

#include "shape.hpp"

namespace ilrd
{

class Circle: public Shape
{
public:
    Circle(size_t position = 0);
    void Draw() const;
};

} // namespace ilrd

#endif // __ILRD_RD110_CIRCLE_HPP__
