#include <cmath>

static double f_0(double x)
{
    x = 1;
    return x;
}

static double f_0_derivative(double x)
{
    x = 0;
    return x;
}

static double f_1(double x)
{
    return x;
}

static double f_1_derivative(double x)
{
    x = 1;
    return x;
}

static double f_2(double x)
{
    return x * x;
}

static double f_2_derivative(double x)
{
    return 2 * x;
}

static double f_3(double x)
{
    return x * x * x;
}

static double f_3_derivative(double x)
{
    return 3 * x * x;
}

static double f_4(double x)
{
    return x * x * x * x;
}

static double f_4_derivative(double x)
{
    return 4 * x * x * x;
}

static double f_5(double x)
{
    return exp(x);
}

static double f_5_derivative(double x)
{
    return exp(x);
}

static double f_6(double x)
{
    return 1. / (25 * x * x + 1);
}

static double f_6_derivative(double x)
{
    return -50. * x / ((25 * x * x + 1) * (25 * x * x + 1));
}