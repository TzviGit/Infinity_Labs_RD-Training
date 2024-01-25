/********************************************************
 * 
 * shapes.hpp - shapes API
 * 
 * Author : RD-110
 * 
 * version: 1.0.0
 * 
 * Reviewed and approved by : Yinon
 * 
 * 22-12-21
 * 
 * OLRD 110 Ramat Gan
 * 
 * *********************************************************/

#ifndef __RD_110_SHAPES_H__
#define __RD_110_SHAPES_H__

#include <cstddef> // size_t
#include <iostream> //cout

namespace ilrd
{

class Shape
{
public:
    Shape(size_t position = 0);
    inline virtual ~Shape(){};
    inline void Move(size_t movement);
    virtual void Draw() const = 0;
    size_t GetPostion() const;

protected:
    void MovePrintToPosition() const;

private:
   
   size_t m_position;

    // advanced:
    // static const char *WHITE = "\033[0m";
    // static const char *BLACK = "\033[30m";
    // static const char *RED = "\033[31m";
    // static const char *GREEN = "\033[32m";
    // static const char *YELLOW = "\033[33m";
    // static const char *BLUE = "\033[34m";
    // static const char *PINK = "\033[35m";
    // static const char *CYAN = "\033[36m";
};

inline Shape::Shape(size_t position):m_position(position)
{   
}

inline void Shape::Move(size_t position)
{
    m_position = position;
}



inline size_t Shape::GetPostion() const
{
    return m_position;
}



} // namespace ilrd

#endif // __ILRD_RD110_SHAPES_HPP__
