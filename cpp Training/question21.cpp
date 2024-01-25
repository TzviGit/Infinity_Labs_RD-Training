
#include <iostream>

struct X
{
    int m_a;
    const int m_b;

    // Default constructor (often abbreviated as: Ctor)
    explicit X();                   // Initialization function without parameters is named.
    explicit X(int a_, int b_ = 8); // Regular (non default) Ctor.
    X(const X &other_);             // Copy initialization function is called copy costructor or CCtor.

    ~X(); // Deinitialization function is called destructor (Dtor).

    X& operator=(const X &other_); // Assignment operator.
};

// Implementation of the default Ctor. Why the X::X? The code after the colon is called an initialization list
X::X() : m_a(3), m_b(4)
{
    // m a =3:
    // m_b = 4;
    std::cout << "this:" << this << " X default Ctor. m_a:" << m_a << " m_b:"
              << m_b << std::endl;
 }

X::X(int a_, int b_) : m_a(a_), m_b(b_) // Implementation of the second ctor.
{
    std::cout << "this:" << this << " X int int Ctor. m_a:" << m_a << " m_b:" << m_b << std::endl;
}
X::X(const X &other_) : m_a(other_.m_a), m_b(other_.m_b) // Implementation of the copy Ctor.
{
    std::cout << "this:" << this << " X copy ctor. m_a: " << m_a << " m_b:" << m_b << std::endl;
}

X &X::operator=(const X &other_)
{
    m_a = other_.m_a;

    // m b= other.m_b;
    std::cout << "this:" << this << " X assignment operator. m a:" << m_a << " does not change m_b:"
              << m_b << std::endl;
    return *this;
}

X::~X()
{
    std::cout << "this:" << this << " X Dtor. m_a:"
              << m_a << " m_b:"
              << m_b << std::endl;
}

int main()
{
    X x1;
    X x2(7);
    X *px = new X(x2);
    X x3(9, 10);
    X x4(x1);

    x1 = x3;

    std::cout << "sizeof (X) : " << sizeof(X) << std::endl;

    delete px;
    px = 0;

    X *xp = new X[10];
    delete[] xp;

    return 0;
}

