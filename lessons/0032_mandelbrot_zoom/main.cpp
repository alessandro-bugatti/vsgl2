#include <iostream>
#include <algorithm>
#include "../../vsgl2.h"

using namespace std;
using namespace vsgl2;
using namespace vsgl2::general;
using namespace vsgl2::video;
using namespace vsgl2::utils;

int RADIUS = 10E12;
const int MAX_ITERATIONS = 100;

const double X_BEGIN = -2;
const double Y_BEGIN = -1.25;
const double X_END= 0.5;
const double Y_END= 1.25;
const double X_RANGE = X_END - X_BEGIN;
const double Y_RANGE = Y_END - Y_BEGIN;


struct Complex{
    double real, imm;
};

Complex sum(Complex r, Complex s)
{
    Complex result;
    result.real = r.real + s.real;
    result.imm= r.imm + s.imm;
    return result;
}

Complex multiply(Complex a, Complex b)
{
    Complex result;
    result.real = a.real*b.real - a.imm*b.imm;
    result.imm = a.real*b.imm + a.imm*b.real;
    return result;
}

double mod(Complex c)
{
    return sqrt(c.real*c.real + c.imm*c.imm);
}

int diverge(Complex C)
{
    int i;
    Complex Z;
    Z.real = 0;
    Z.imm = 0;
    for (i = 0; i < MAX_ITERATIONS && mod(Z) < RADIUS; i++)
        Z = sum(multiply(Z,Z),C);
    return i;
}

int main(int argc, char* argv[]) {
    int i, j;
    bool need_update = true;
    double x_begin = X_BEGIN ;
    double y_begin = Y_BEGIN ;
    double x_end = X_END;
    double y_end = Y_END;
    double x_range = X_RANGE ;
    double y_range = Y_RANGE;
    init();
    //create the window and show it
    set_window(640,640,"Vsgl2 Mandelbrot with zooming");
    while(1)
    {
        if (is_pressed(VSGL_E))
            break;
        if (mouse_left_button_pressed())
        {
            double x = get_mouse_x()*x_range/get_window_width() + x_begin;
            double y = get_mouse_y()*y_range/get_window_height() + y_begin;
            x_range /= 10;
            y_range /= 10;
            x_begin = x - x_range/2;
            y_begin = y - y_range/2;
            need_update = true;
        }
        if (mouse_right_button_pressed())
        {
            double x = get_mouse_x()*x_range/get_window_width() + x_begin;
            double y = get_mouse_y()*y_range/get_window_height() + y_begin;
            x_range *= 10;
            y_range *= 10;
            x_begin = x - x_range/2;
            y_begin = y - y_range/2;
            need_update = true;
        }
        if(need_update)
        {
        //Iterate on a region of the plane to
        //draw the Mandelbrot set

            for (i = 0; i < get_window_height() ; i++)
                for (j = 0; j < get_window_width() ; j++)
                    {
                        Complex c;
                        c.real = i*x_range/get_window_width() + x_begin;
                        c.imm = j*y_range/get_window_height() + y_begin;
                        int n = diverge(c);
                        draw_point(i,j,
                                   Color(128 + n*128/MAX_ITERATIONS ,
                                         n*128/MAX_ITERATIONS,
                                         n*768/MAX_ITERATIONS%256,
                                         255));
                    }
                need_update = false;
        }
        update();
    }
    //close the library and clean up everything
    close();
    return 0;
}
