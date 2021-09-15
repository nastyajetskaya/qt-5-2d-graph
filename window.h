#ifndef WINDOW_H
#define WINDOW_H
#include <QtWidgets/QtWidgets>

#include <cmath>
#include <iostream>
#include <vector>

double zero(double x);

class Window : public QWidget
{
    Q_OBJECT

  private:
    int func_id;
    int method_id = 0;
    const char *f_name;
    double a;
    double b;
    int n;
    int scale = 1;
    int delta = 0;
    double (*f)(double);
    double (*f_derivative)(double);

    // place to keep data
    std::vector<double> x;
    std::vector<double> fx;

    std::vector<double> d;

    std::vector<double> coeffs_method1;
    std::vector<double> coeffs_method2;

    std::vector<double> bot_diag;
    std::vector<double> diag;
    std::vector<double> top_diag;

    void set_function(void);
    void input(void);
    void delta_apply(int num, double d);

  public:
    Window(QWidget *parent);

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    int parse_command_line(int argc, char *argv[]);

    void compute_method1(void);
    double res_method1(double t);

    void compute_method2(void);
    double res_method2(double t);

  public slots:
    void change_func();
    void change_method();
    void scale_plus();
    void scale_minus();
    void points_plus();
    void points_minus();
    void delta_plus();
    void delta_minus();

  protected:
    void paintEvent(QPaintEvent *event);
};

#endif
