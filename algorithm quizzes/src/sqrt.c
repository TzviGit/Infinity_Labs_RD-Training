

#include <stdio.h>

static double SqrtImple(double number, double lower_bound, double upper_bound);

static const double ALLOWED_ERROR = 0.00000001;
double Sqrt(double number)
{
    if (number < 1)
    {
        return -1;
    }

    return SqrtImple(number, 0, number);
}

static double SqrtImple(double number, double lower_bound, double upper_bound)
{
    double mid = lower_bound + (upper_bound - lower_bound) / 2;
    double mid_squared = mid * mid;

    if (mid_squared > number + ALLOWED_ERROR)
    {
        return SqrtImple(number, lower_bound, mid);
    }
    else if (mid_squared < number - ALLOWED_ERROR)
    {
        return SqrtImple(number, mid, upper_bound);
    }
    else
    {
        return mid;
    }
}

int main(void)
{
    double number = 0;

    while (1)
    {   
        printf("please enter a number :\n\n");

        scanf("%lf", &number);

        printf("the square root of %f is %f.\n\n", number, Sqrt(number));

    }
    return 0;
}