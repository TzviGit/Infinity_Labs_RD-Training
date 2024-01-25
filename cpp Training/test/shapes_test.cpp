
#include <list>
#include "circle.hpp"
#include "line.hpp"
#include "rectangle.hpp"
#include "square.hpp"

using namespace ilrd;
int main()
{
    std::list<Shape *> shapesList;

    shapesList.push_back(new Circle(31));
    shapesList.push_back(new Square(10));
    
    // s1.Move(50);
    // c1.Move(31);
    for (std::list<Shape *>::iterator iter = shapesList.begin(); iter != shapesList.end(); ++iter)
    {
        (**iter).Draw();
    }

    return 0;
}