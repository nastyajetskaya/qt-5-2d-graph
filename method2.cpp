#include "window.h"

using namespace std;

void Window::compute_method2(void)
{
    bot_diag.resize(n -1);
	diag.resize(n);
	top_diag.resize(n - 1);
	coeffs_method2.resize((n-1)*4);
    d.resize(n);

    diag[0] = 2;
	top_diag[0] = 1;
	coeffs_method2[0] = 3 * (fx[1] - fx[0]) / (x[1] - x[0]);
	bot_diag[n - 2] = 1;
	diag[n - 1] = 2;
	coeffs_method2[n - 1] = 3 * (fx[n - 1] - fx[n - 2]) / (x[n - 1] - x[n - 2]);

	for (int i = 1; i < n - 1; i++) {
		bot_diag[i - 1] = x[i + 1] - x[i];
		diag[i] = 2 * (x[i + 1] - x[i - 1]);
		top_diag[i] = x[i] - x[i - 1];
		coeffs_method2[i] = 3 * (fx[i] - fx[i - 1]) / (x[i] - x[i - 1]) * (x[i + 1] - x[i]) + 3* (fx[i + 1] - fx[i])/ (x[i + 1] - x[i]) * (x[i] - x[i - 1]);
	}

    top_diag[0] /= diag[0];
	for (int i = 1; i < n - 1; i++) {
		diag[i] -= bot_diag[i - 1] * top_diag[i - 1];
		top_diag[i] /= diag[i];
	}
	diag[n - 1] -= bot_diag[n - 2] * top_diag[n - 2];
	d[0] = coeffs_method2[0] / diag[0];
	for (int i = 1; i < n; i++)
		d[i] = (coeffs_method2[i] - bot_diag[i - 1] * d[i - 1]) / diag[i];
	for (int i = n - 2; i >= 0; i--)
		d[i] -= top_diag[i] * d[i + 1];

    for (int i = 0; i < n - 1; i++) {
        coeffs_method2[4 * i] = fx[i];
        coeffs_method2[4 * i + 1] = d[i];
        coeffs_method2[4 * i + 2] = (3 * (fx[i + 1] - fx[i]) / (x[i + 1] - x[i]) - 2 * d[i] - d[i + 1]) / (x[i + 1] - x[i]);
        coeffs_method2[4 * i + 3] = (d[i] + d[i + 1] - 2 * (fx[i + 1] - fx[i]) / (x[i + 1] - x[i])) / ((x[i + 1] - x[i]) * (x[i + 1] - x[i]));
    }
}

double Window::res_method2(double t)
{
    int i;
    for (i = 0; i < n - 2; i++) 
        if (t <= x[i + 1]) break;
    double res = coeffs_method2[4 * i] + coeffs_method2[4 * i + 1] * (t - x[i]) + coeffs_method2[4 * i + 2] *
                 (t - x[i]) * (t - x[i]) + coeffs_method2[4 * i + 3] * (t - x[i]) * (t - x[i]) * (t - x[i]);
    return res;
}


