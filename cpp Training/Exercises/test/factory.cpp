


#include <iostream>
#include <string>

#include "shape.hpp"
#include "line.hpp"
#include "circle.hpp"
#include "rectangle.hpp"
#include "shared_ptr.hpp"
//#include "singleton_strange.hpp"
#include "factory.hpp"

using namespace ilrd;

SharedPtr<Shape> CreateCircle(int param)
{
    (void)param;
    return SharedPtr<Shape>(new Circle);
}
SharedPtr<Shape> CreateLine(int param)
{
    (void)param;
    return SharedPtr<Shape>(new Line);
}
SharedPtr<Shape> CreateRectangle(int param)
{

    (void)param;
    return SharedPtr<Shape>(new Rectangle);
}

void Drow(SharedPtr<Shape> p)
{

    p->Draw();

}


int main()
{//instaed of shape& get shared_ptr?
    Factory<SharedPtr<Shape> , char, int>* ptr = Singleton<Factory<SharedPtr<Shape> , char, int> >::GetInstance();

    ptr->Add('c', CreateCircle);

    ptr->Add('l', CreateLine);

    ptr->Add('r', CreateRectangle);


    std::vector<SharedPtr<Shape> > vec_sh;

    for(size_t i = 0; i < 5; ++i)
    {
        char chr;
        std::cout << "enter key:\n c -> circle\n r -> rectangle\n l -> line" << std::endl;
        std::cin >> chr;
        vec_sh.push_back(ptr->Create(chr, 0));
    }
    std::for_each(vec_sh.begin(), vec_sh.begin() + 5, Drow);

    return 0;
}

//https://preshing.com/20120515/memory-reordering-caught-in-the-act/