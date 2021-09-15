#include "window.h"
using namespace std;

void Window::compute_method1(void)
{
    coeffs_method1.resize((n - 1) * 4);
    d.resize(n);
    int i;

    d[0] = f_derivative(x[0]);
    d[n - 1] = f_derivative(x[n - 1]);

    for (i = 1; i < n - 1; i++) {
        d[i] = ((x[i + 1] - x[i]) * (fx[i] - fx[i - 1]) / (x[i] - x[i - 1]) +
                (x[i] - x[i - 1]) * (fx[i + 1] - fx[i]) / (x[i + 1] - x[i])) /
               (x[i + 1] - x[i - 1]);
    }

    for (i = 0; i < n - 1; i++) {
        coeffs_method1[4 * i] = fx[i];
        coeffs_method1[4 * i + 1] = d[i];
        coeffs_method1[4 * i + 2] =
            (3 * (fx[i + 1] - fx[i]) / (x[i + 1] - x[i]) - 2 * d[i] -
             d[i + 1]) /
            (x[i + 1] - x[i]);
        coeffs_method1[4 * i + 3] =
            (d[i] + d[i + 1] - 2 * (fx[i + 1] - fx[i]) / (x[i + 1] - x[i])) /
            ((x[i + 1] - x[i]) * (x[i + 1] - x[i]));
    }
}

double Window::res_method1(double t)
{
    int i;
    for (i = 0; i < n - 2; i++) {
        if (t <= x[i + 1]) {
            break;
        }
    }
    double val = t - x[i];
    double res = coeffs_method1[4 * i] + coeffs_method1[4 * i + 1] * val +
                 coeffs_method1[4 * i + 2] * val * val +
                 coeffs_method1[4 * i + 3] * val * val * val;
    return res;
}
