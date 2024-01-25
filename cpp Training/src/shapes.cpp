

#include "shape.hpp"

namespace ilrd
{

    void Shape::MovePrintToPosition()const
{
    for (size_t i = 0; i < m_position; i++)
    {
        std::cout << " ";
    }
}
}