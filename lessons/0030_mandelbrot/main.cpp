#include <iostream>
#include <algorithm>
#include "../../vsgl2.h"

using namespace std;
using namespace vsgl2;
using namespace vsgl2::general;
using namespace vsgl2::video;
using namespace vsgl2::utils;

const int RADIUS = 10;
const int MAX_ITERATIONS = 40;

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
    for (i = 0; i < MAX_ITERATIONS; i++)
    {
        if (mod(Z) > RADIUS)
            return 1;
        Z = sum(multiply(Z,Z),C);
    }
    return 0;
}

int main(int argc, char* argv[]) {
    int i, j;
    init();
    //create the window and show it
    set_window(640,640,"Vsgl2 Mandelbrot");
    //Iterate on a region of the plane to
    //draw the Mandelbrot set
    for (i = 0; i < get_window_height() ; i++)
        for (j = 0; j < get_window_width() ; j++)
            {
                Complex c;
                c.real = i*X_RANGE/get_window_width() + X_BEGIN;
                c.imm = j*Y_RANGE/get_window_height() + Y_BEGIN;
                if (diverge(c))
                    draw_point(i,j, Color(255,255,255,255));
                else
                    draw_point(i,j, Color(0,0,0,255));
            }
    //update the screen
    update();
    //the program waits for any button to be pressed
    wait_for_button_pressed();
    //close the library and clean up everything
    close();
    return 0;
}
