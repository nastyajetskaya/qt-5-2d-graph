#include <QPainter>

#include "functions.h"
#include "window.h"

#define DEFAULT_A -10
#define DEFAULT_B 10
#define DEFAULT_N 10

Window::Window(QWidget *parent) : QWidget(parent)
{
    a = DEFAULT_A;
    b = DEFAULT_B;
    n = DEFAULT_N;

    func_id = 0;

    set_function();
}

QSize Window::minimumSizeHint() const
{
    return QSize(100, 100);
}

QSize Window::sizeHint() const
{
    return QSize(1000, 1000);
}

void Window::input(void)
{
    double k = (b - a) / (n - 1);
    x.resize(n);
    fx.resize(n);

    for (int i = 0; i < n; i++) {
        x[i] = a + i * k;
        fx[i] = f(x[i]);
    }
}

void Window::set_function(void)
{
    switch (func_id) {
    case 0:
        f_name = "f(x) = 1";
        f = f_0;
        f_derivative = f_0_derivative;
        break;
    case 1:
        f_name = "f(x) = x";
        f = f_1;
        f_derivative = f_1_derivative;
        break;
    case 2:
        f_name = "f(x) = x^2";
        f = f_2;
        f_derivative = f_2_derivative;
        break;
    case 3:
        f_name = "f(x) = x^3";
        f = f_3;
        f_derivative = f_3_derivative;
        break;
    case 4:
        f_name = "f(x) = x^4";
        f = f_4;
        f_derivative = f_4_derivative;
        break;
    case 5:
        f_name = "f(x) = e^x";
        f = f_5;
        f_derivative = f_5_derivative;
        break;
    case 6:
        f_name = "f(x) = 1/(25*x^2 + 1)";
        f = f_6;
        f_derivative = f_6_derivative;
        break;
    }
}

int Window::parse_command_line(int argc, char *argv[])
{
    if (argc != 5) {
        return -2;
    }

    if (sscanf(argv[1], "%lf", &a) != 1 || sscanf(argv[2], "%lf", &b) != 1 ||
        b - a < 1.e-6 || sscanf(argv[3], "%d", &n) != 1 ||
        sscanf(argv[4], "%d", &func_id) != 1 || n <= 0) {
        return -2;
    }

    set_function();
    return 0;
}

void Window::delta_apply(int num, double d)
{
    fx[num] += d;
}

/// change current function for drawing
void Window::change_func()
{
    func_id = (func_id + 1) % 7;

    set_function();
    update();
}

void Window::change_method()
{
    method_id = (method_id + 1) % 4;
    update();
}

void Window::scale_plus()
{
    a = a * 2;
    b = b * 2;
    scale = scale * 2;
    update();
}

void Window::scale_minus()
{
    a = a / 2;
    b = b / 2;
    scale = scale / 2;
    update();
}

void Window::points_plus()
{
    n *= 2;
    update();
}

void Window::points_minus()
{
    if (n >= 3) {
        n = n / 2;
        update();
    }
}

void Window::delta_plus()
{
    delta++;
    update();
}

void Window::delta_minus()
{
    delta--;
    update();
}

/// render graph
void Window::paintEvent(QPaintEvent * /* event */)
{
    QPainter painter(this);
    double x1, x2, x3, y1, y2, y3;
    double max_y, min_y;
    double delta_x = (b - a) / n;
    QPen pen_black(Qt::black, 0, Qt::SolidLine);
    QPen pen_red(Qt::red, 0, Qt::SolidLine);
    QPen pen_green(Qt::green, 0, Qt::SolidLine);
    QPen pen_blue(Qt::blue, 0, Qt::SolidLine);
    QPen pen_magenta(Qt::magenta, 0, Qt::SolidLine);
    QPen pen_cyan(Qt::darkCyan, 0, Qt::SolidLine);

    // calculate min and max for current function
    max_y = min_y = 0;
    for (x1 = a; x1 - b < 1.e-6; x1 += delta_x) {
        y1 = f(x1);
        if (y1 < min_y) {
            min_y = y1;
        }
        if (y1 > max_y) {
            max_y = y1;
        }
    }

    // save current Coordinate System
    painter.save();

    // make Coordinate Transformations
    painter.translate(0.5 * width(), 0.5 * height());
    painter.scale(width() / (b - a), -height() / (max_y - min_y));
    painter.translate(-0.5 * (a + b), -0.5 * (min_y + max_y));

    // draw approximated line for graph
    int i;
    double max1 = 0, max2 = 0;
    if (method_id != 3) {
        painter.setPen(pen_black);
        x1 = a;
        y1 = f(x1);
        for (x2 = x1 + delta_x, i = 1; x2 - b < 1.e-3; x2 += delta_x, i++) {
            for (x3 = x1 + delta_x * 1e-2; x3 - x2 < 1e-3;
                 x3 += delta_x * 1e-2) {
                y3 = f(x3);
                painter.drawLine(QPointF(x1, y1), QPointF(x3, y3));
                x1 = x3;
                y1 = y3;
            }
            y2 = f(x2);
            if (i == n / 2) {
                y2 += delta * 1e-1 *
                      (abs(max_y) > abs(min_y) ? abs(max_y) : abs(min_y));
            }
            x1 = x2;
            y1 = y2;
        }
        x2 = b;
        y2 = f(x2);
        painter.drawLine(QPointF(x1, y1), QPointF(x2, y2));
    }

    input();
    delta_apply(n / 2 - 1,
                delta * 1e-1 *
                    (abs(max_y) > abs(min_y) ? abs(max_y) : abs(min_y)));

    // draw first method
    if ((method_id == 0 || method_id == 2)) {
        painter.setPen(pen_magenta);
        compute_method1();
        x1 = a;
        y1 = res_method1(x1);
        for (x2 = x1 + delta_x; x2 - b < 1e-3; x2 += delta_x) {
            for (x3 = x1 + delta_x * 1e-2; x3 - x2 < 1e-3;
                 x3 += delta_x * 1e-2) {
                y3 = res_method1(x3);
                painter.drawLine(QPointF(x1, y1), QPointF(x3, y3));
                x1 = x3;
                y1 = y3;
            }
            y2 = res_method1(x2);
            x1 = x2;
            y1 = y2;
        }
        x2 = b;
        y2 = res_method1(x2);
        painter.drawLine(QPointF(x1, y1), QPointF(x2, y2));
    }
    // draw second method
    if (method_id == 1 || method_id == 2) {
        painter.setPen(pen_cyan);
        compute_method2();
        x1 = a;
        y1 = res_method2(x1);
        for (x2 = x1 + delta_x; x2 - b < 1e-3; x2 += delta_x) {
            for (x3 = x1 + delta_x * 1e-2; x3 - x2 < 1e-3;
                 x3 += delta_x * 1e-2) {
                y3 = res_method2(x3);
                painter.drawLine(QPointF(x1, y1), QPointF(x3, y3));
                x1 = x3;
                y1 = y3;
            }
            y2 = res_method2(x2);
            x1 = x2;
            y1 = y2;
        }
        x2 = b;
        y2 = res_method2(x2);
        painter.drawLine(QPointF(x1, y1), QPointF(x2, y2));
    }

    if (method_id == 3) {
        // diff first method
        painter.setPen(pen_green);
        compute_method1();
        x1 = a;
        y1 = f(x1) - res_method1(x1);
        max1 = y1;
        for (x2 = x1 + delta_x, i = 1; x2 - b < 1e-3; x2 += delta_x, i++) {
            for (x3 = x1 + delta_x * 1e-2; x3 - x2 < 1e-3;
                 x3 += delta_x * 1e-2) {
                y3 = f(x1) - res_method1(x3);
                painter.drawLine(QPointF(x1, y1), QPointF(x3, y3));
                if (y3 > max1) {
                    max1 = y3;
                }
                x1 = x3;
                y1 = y3;
            }
            y2 = f(x2) - res_method1(x2);
            if (i == n / 2) {
                y2 += delta * 1e-1 *
                      (abs(max_y) > abs(min_y) ? abs(max_y) : abs(min_y));
            }
            x1 = x2;
            y1 = y2;
        }
        x2 = b;
        y2 = f(x2) - res_method1(x2);
        painter.drawLine(QPointF(x1, y1), QPointF(x2, y2));
        if (y2 > max1) {
            max1 = y2;
        }

        // diff second method
        painter.setPen(pen_red);
        compute_method2();
        x1 = a;
        y1 = f(x1) - res_method2(x1);
        max2 = y1;
        for (x2 = x1 + delta_x, i = 1; x2 - b < 1e-3; x2 += delta_x, i++) {
            for (x3 = x1 + delta_x * 1e-2; x3 - x2 < 1e-3;
                 x3 += delta_x * 1e-2) {
                y3 = f(x1) - res_method2(x3);
                painter.drawLine(QPointF(x1, y1), QPointF(x3, y3));
                if (y3 > max2) {
                    max2 = y3;
                }
                x1 = x3;
                y1 = y3;
            }
            y2 = f(x2) - res_method2(x2);
            if (i == n / 2) {
                y2 += delta * 1e-1 *
                      (abs(max_y) > abs(min_y) ? abs(max_y) : abs(min_y));
            }
            x1 = x2;
            y1 = y2;
        }
        x2 = b;
        y2 = f(x2) - res_method2(x2);
        painter.drawLine(QPointF(x1, y1), QPointF(x2, y2));
        if (y2 > max2) {
            max2 = y2;
        }
    }

    // draw axis
    painter.setPen(pen_black);
    painter.drawLine(a, 0, b, 0);
    painter.drawLine(0, max_y, 0, min_y);

    // restore previously saved Coordinate System
    painter.restore();

    // render function name
    painter.setPen("black");
    painter.drawText(10, 50, QString("Function: %1").arg(f_name));
    painter.drawText(10, 65, QString("Number of points: %1").arg(n));
    painter.drawText(10, 80, QString("View methtod %1").arg(method_id + 1));
    painter.drawText(10, 95,
                     QString("Scale:%1 x:[%2, %3] f:[%4, %5]")
                         .arg(scale)
                         .arg(a)
                         .arg(b)
                         .arg(min_y)
                         .arg(max_y));
    painter.drawText(10, 110, QString("Delta: %1").arg(delta));
    painter.setPen("magenta");
    painter.drawText(10, 20, QString("The first method is purple"));
    painter.setPen("darkCyan");
    painter.drawText(10, 35, QString("The second method is blue"));
    if (method_id == 3) {
        painter.setPen("darkGreen");
        painter.drawText(10, 125,
                         QString("Diff. first method: %1 (green)").arg(max1));
        painter.setPen("red");
        painter.drawText(10, 140,
                         QString("Diff. second method: %1 (red) ").arg(max2));
    }
}
