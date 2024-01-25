#ifndef __ILRD_RD110_SQUARE_HPP__
#define __ILRD_RD110_SQUARE_HPP__

#include "shape.hpp"

namespace ilrd
{

class Square: public Shape
{
public:
    Square(size_t position = 0);
    void Draw() const;
};

} // namespace ilrd

#endif // __ILRD_RD110_SQUARE_HPP__