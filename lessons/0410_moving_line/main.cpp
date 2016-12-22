#include <iostream>
#include <ctime>
#include "../../vsgl2.h"

using namespace std;
using namespace vsgl2;
using namespace vsgl2::general;
using namespace vsgl2::video;
using namespace vsgl2::utils;
using namespace vsgl2::io;

/** Move the ends of the line using the keyboard
*   WASD keys move the left end,
*   while the arrow keys move the right end
*/

int main(int argc, char* argv[]) {

    //init the library
    init();
    //create the window and show it
    set_window(512,512,"Vsgl2 moving line");
    //main loop
    float x1, y1, x2, y2;
    float delta = 0.0002;
    x1 = 0.25;
    y1 = 0.5;
    x2 = 0.75;
    y2 = 0.5;
    int w = get_window_width();
    int h = get_window_height();
    while(!done())
    {
        if (is_pressed(VSGL_A))
            x1 -= delta;
        if (is_pressed(VSGL_D))
            x1 += delta;
        if (is_pressed(VSGL_W))
            y1 -= delta;
        if (is_pressed(VSGL_S))
            y1 += delta;
        if (is_pressed(VSGL_LEFT))
            x2 -= delta;
        if (is_pressed(VSGL_RIGHT))
            x2 += delta;
        if (is_pressed(VSGL_UP))
            y2 -= delta;
        if (is_pressed(VSGL_DOWN))
            y2 += delta;
        draw_line(x1*w,y1*h,x2*w,y2*h,Color(255,0,0,255));
        draw_filled_rect(x1*w-2,y1*w-2,4,4,Color(0,255,0,255));
        draw_filled_rect(x2*w-2,y2*w-2,4,4,Color(0,0,255,255));
        update();
    }

    //close the library and clean up everything
    close();
    return 0;
}
