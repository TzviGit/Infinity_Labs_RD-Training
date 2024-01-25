#ifndef __ILRD_RD110_LINE_HPP__
#define __ILRD_RD110_LINE_HPP__

#include "shape.hpp"

namespace ilrd
{

class Line: public Shape
{
public:
    Line(size_t position = 0);
    void Draw() const;
};

} // namespace ilrd

#endif // __ILRD_RD110_LINE_HPP__
