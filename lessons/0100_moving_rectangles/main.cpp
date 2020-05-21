#include <iostream>
#include <ctime>
#include "../../vsgl2.h"

using namespace std;
using namespace vsgl2;
using namespace vsgl2::general;
using namespace vsgl2::video;
using namespace vsgl2::utils;

void update(double &x, double &y, double vx, double vy)
{
    x = x + vx;
    y = y + vy;
}

void draw(double x, double y)
{
    draw_filled_rect(x,y,20,20, Color(255,0,0,255));
    draw_filled_rect(get_window_width() - x,y,20,20, Color(0,255,0,255));
}

int main(int argc, char* argv[]) {
    //Rectangle position
    double x;
    double y;
    //Speed along x and y axis
    double vx, vy;

    x = 0;
    y = get_window_height()/2;
    vx = 0.1;
    vy = 0.1;
    //init the library
    init();
    //create the window and show it
    set_window(512,512,"Vsgl2 moving rectangles");

    //main loop

    while(!done() && y < get_window_height())
    {
        delay(2);
        update(x,y,vx,vy);
        draw(x,y);
        update();
    }

    //close the library and clean up everything
    close();
    return 0;
}

