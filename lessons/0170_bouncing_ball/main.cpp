#include <iostream>
#include <ctime>
#include "../../vsgl2.h"

using namespace std;
using namespace vsgl2;
using namespace vsgl2::general;
using namespace vsgl2::video;
using namespace vsgl2::utils;

#define G -0.00008
#define XFACTOR 0.95
#define YFACTOR 0.95

double xc;
double yc;
int dimx = 50 , dimy = 50;
double vx, vy;

void init_ball();
void update_position();
int scale_x(double x);
int scale_y(double y);
void rebound();

int main(int argc, char* argv[]) {
    //init the library
    init();
    //create the window and show it
    set_window(512,512,"Vsgl2 bouncing ball");
    srand(time(NULL));
    init_ball();
    //main loop
    while(!done())
    {
        delay(2);
        rebound();
        update_position();
        draw_image("background.png",0,0,512,512);
        draw_image("basketball.png",
                   scale_x(xc) - dimx/2, scale_y(yc) - dimy/2,
                   dimx,dimy);
        update();
    }

    //close the library and clean up everything
    close();
    return 0;
}

void init_ball()
{
    xc = 0;
    yc = 0;
    vx = (double)rand()/1000000;
    vy = -(double)rand()/10000000;
}

void update_position()
{
    if (vy == 0) return;
    xc = xc + vx;
    yc = yc - vy;
    vy = vy + G;
}

int scale_x(double x)
{
    return x*get_window_width();
}

int scale_y(double y)
{
    return y*get_window_height();
}

void rebound()
{
    if (xc > 1 && vx > 0)
        vx = -XFACTOR*vx;
    if (xc < 0 && vx < 0)
        vx = -XFACTOR*vx;
    if (yc > 1 && vy < 0)
        vy = -YFACTOR*vy;
    if (vy > 0.0001 && vy < 0.001 && yc > 0.99 )
        vy = vx = 0;
    SDL_Log("yc: %lf vy: %lf",yc, vy);

}
