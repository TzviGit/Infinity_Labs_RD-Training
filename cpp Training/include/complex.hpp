
/* ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ *
 ~ Author        R&D 110                 ~
 * Version       1                       *
 ~ Date          14,12,2021              ~
 * Reviewer      Yinon                   *
 ~ Description   Complex Numbers API     ~
 * Group         R&D 110                 *
 ~ Company       ILRD Ramat Gan          ~
 * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ */

#ifndef __ILRD_RD110_COMPLEX_HPP__
#define __ILRD_RD110_COMPLEX_HPP__

#include <iostream> // istream, ostream
namespace ilrd
{
class Complex
{
public:
    inline Complex(double r_ = 0, double im_ = 0);// Ctor not explicit by design
    // default cCtor
    // default Dtor
    // default assignment operator

    inline Complex& operator+=(const Complex& rhs_);
    inline Complex& operator-=(const Complex& rhs_);
    inline Complex& operator*=(const Complex& rhs_);
    inline Complex& operator/=(const Complex& rhs_);    // must not divide by 0

    inline void SetReal(double r_);
    inline void SetImaginary(double im_);

    inline double GetReal() const;
    inline double GetImaginary() const;

private:
    double m_r;
    double m_im;
};

Complex::Complex(double r_, double im_): m_r(r_), m_im(im_)
{
    // m_r = r_;
    // m_im = im_;
}

Complex& Complex::operator+=(const Complex& rhs_)
{
    m_r += rhs_.GetReal();
    m_im += rhs_.GetImaginary();

    return *this;
}

Complex& Complex::operator-=(const Complex& rhs_)
{
    m_r -= rhs_.GetReal();
    m_im -= rhs_.GetImaginary();
    
    return *this;
}

Complex& Complex::operator*=(const Complex& rhs_)
{
    double real = (GetReal() * rhs_.GetReal()) - (GetImaginary() * rhs_.GetImaginary());
    double imaginary = (GetReal() * rhs_.GetImaginary()) + (GetImaginary() * rhs_.GetReal());
    m_r = real;
    m_im = imaginary;

    return *this;
}

Complex& Complex::operator/=(const Complex& rhs_)
{
    double denomMagnitude = (rhs_.GetReal() * rhs_.GetReal()) + (rhs_.GetImaginary() * rhs_.GetImaginary());
    double real = ((GetReal() * rhs_.GetReal()) + (GetImaginary() * rhs_.GetImaginary())) / denomMagnitude;
    double imaginary = ((GetImaginary() * rhs_.GetReal()) - (GetReal() * rhs_.GetImaginary())) / denomMagnitude;
    m_r = real;
    m_im = imaginary;

    return *this;
}



void Complex::SetReal(double r_)
{
    m_r = r_;
}

void Complex::SetImaginary(double im_)
{
    m_im = im_;
}

double Complex::GetReal() const
{
    return m_r;
}

double Complex::GetImaginary() const
{
    return m_im;
}

inline const Complex operator+(const Complex& lhs_, const Complex& rhs_)
{
    return Complex((lhs_.GetReal() + rhs_.GetReal()), (lhs_.GetImaginary() + rhs_.GetImaginary()));
}

inline const Complex operator-(const Complex& lhs_, const Complex& rhs_)
{
    return Complex((lhs_.GetReal() - rhs_.GetReal()), (lhs_.GetImaginary() - rhs_.GetImaginary()));
}

inline const Complex operator*(const Complex& lhs_, const Complex& rhs_)
{
    double real = (lhs_.GetReal() * rhs_.GetReal()) - (lhs_.GetImaginary() * rhs_.GetImaginary());
    double imaginary = (lhs_.GetReal() * rhs_.GetImaginary()) + (lhs_.GetImaginary() * rhs_.GetReal());
    
    return Complex(real, imaginary);
}

inline const Complex operator/(const Complex& lhs_, const Complex& rhs_)
{
    double denomMagnitude = (rhs_.GetReal() * rhs_.GetReal()) + (rhs_.GetImaginary() * rhs_.GetImaginary());
    double real = ((lhs_.GetReal() * rhs_.GetReal()) + (lhs_.GetImaginary() * rhs_.GetImaginary())) / denomMagnitude;
    double imaginary = ((lhs_.GetImaginary() * rhs_.GetReal()) - (lhs_.GetReal() * rhs_.GetImaginary())) / denomMagnitude;
    
    return Complex(real, imaginary);
}

inline bool operator==(const Complex& lhs_, const Complex& rhs_)
{
    return (lhs_.GetReal() == rhs_.GetReal()) && (lhs_.GetImaginary() == rhs_.GetImaginary());
}

inline bool operator!=(const Complex& lhs_, const Complex& rhs_)
{
    return !(lhs_ == rhs_);
}

inline std::ostream& operator<<(std::ostream& os_, const Complex& rhs_)
{
    return os_ << rhs_.GetReal() << " + " << rhs_.GetImaginary() << "i";
}
inline const std::istream& operator>>(std::istream& is_, Complex& rhs_)
{
    double r_ = 0, im_ = 0;
    is_ >> r_ >> im_;
    
    rhs_.SetReal(r_);
    rhs_.SetImaginary(im_);
    
    return is_;
}



} // namespace ilrd








#endif /*__ILRD_RD110_COMPLEX_HPP__*/

