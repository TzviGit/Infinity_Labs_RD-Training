#include <stddef.h>
#include <iostream>

class A
{
    public:
    A()
    {
       // array = new int[count];
        std::cout << "count is now:  " << ++count << "  \n";
        ++count;

        number = count;
        if (5 == number)
        {
            throw("whoops");
        }
    }

    ~A()
    {
        
        std::cout << "DESTRUCTOR # : " << number <<" \n"
                  << std::endl;
       // delete [] array;
    }
    static size_t count;
    size_t number;
    int *array;
};

size_t A::count = ;

int main()
{
    A *a;
    A *b1;
    // size_t *ptr = (size_t *)a;
    // *(ptr - 1) = 3;
    try
    {
        {
            a = new A[20];
            
        }
    }
    catch(...)
    {
        std::cerr << "HHHHH" << '\n';
        
    }
    // int b = 5;
    // std::cout << b << "\n\n\n\n\n"<<std::endl;
    //delete[] a;
    
}

// #include <iostream>

// struct A 
// {
//     static int count;
//     int n;
//     int *array;

//     A() : n(++count)
//     {
//         std::cout << "constructing " << n << '\n';
        
//         if (count >= 5)
//         {
//             throw std::runtime_error("oops");
//         }
//             array = new int[3];
//     }

//     ~A()
//     {
//         delete[] array;
//         std::cout << "destroying " << n << '\n';
//     }
// };

// int A::count;

// int main()
// {
//     A *ar = (A *)10000;
//     try
//     {
//         std::cout << "address is ->  " << ar << "\n\n";
//         ar = new A[44];
//     }
//     catch(std::exception const& ex)
//     {
//         std::cout << "address after  is ->  " << ar << "\n\n";
//         std::cerr << ex.what() << '\n';
//     }
// std::cout << "address after  is ->  " << ar << "\n\n";
//    // delete ar;
// }