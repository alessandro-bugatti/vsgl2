#include <iostream>
#include "../../vsgl2.h"

using namespace std;
using namespace vsgl2;
using namespace vsgl2::general;
using namespace vsgl2::video;
using namespace vsgl2::utils;

const double DELTA = 0.1;

int main(int argc, char* argv[]) {
    //init the library
    init();
    double zoom = 1.0;
    int x_center, y_center, w = 100, h = 100;
    //create the window and show it
    set_window(512,512,"Vsgl2 mouse wheel zooming");
    x_center = get_window_width()/2;
    y_center = get_window_height()/2;
    while(!done())
    {
        //draw a BMP image
        draw_image("lena.bmp",x_center-w*zoom/2,y_center - h*zoom/2,
                   w*zoom,h*zoom);
        if (get_mouse_wheel_y() < 0)
            zoom -= DELTA;
        if (get_mouse_wheel_y() > 0)
            zoom += DELTA;

        //update the screen
        update();
    }
    //close the library and clean up everything
    close();
    return 0;
}
