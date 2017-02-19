#include <iostream>
#include <cstdio>
#include "../../vsgl2.h"

using namespace std;
using namespace vsgl2;
using namespace vsgl2::general;
using namespace vsgl2::io;
using namespace vsgl2::video;
using namespace vsgl2::utils;
using namespace vsgl2::ttf_fonts;

int main(int argc, char* argv[]) {
    char coordinates[20];
    //init the library
    init();
    //create the window and show it
    set_window(512,512,"Vsgl2 mouse movements");
    int x = get_mouse_x();
    int y = get_mouse_y();
    while(!done())
    {
        sprintf(coordinates,"(%3d, %3d)",x ,y );
        draw_text("audiowide.ttf",20,coordinates,
                  x  - text_width("audiowide.ttf", 20 ,coordinates),
                  y - text_height("audiowide.ttf", 20, coordinates),
                  Color(0,0,0,255));
        x = get_mouse_x();
        y = get_mouse_y();
        delay(2);
        update();
    }
    close();
    return 0;
}
