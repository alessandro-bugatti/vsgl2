#include <iostream>
#include <ctime>
#include <cstdio>
#include "../../vsgl2.h"

using namespace std;
using namespace vsgl2;
using namespace vsgl2::general;
using namespace vsgl2::video;
using namespace vsgl2::utils;
using namespace vsgl2::io;
using namespace vsgl2::ttf_fonts;


const float SPEED = 0.0001;

int main(int argc, char* argv[]) {

    //init the library
    init();
    //create the window and show it
    set_window(512,128,"Vsgl2 slider");
    //main loop
    int w = get_window_width();
    int h = get_window_height();
    int length = w*0.9;
    int length_bar = w*0.85;

    float on = 0.5 ;
    while(!done())
    {
        draw_filled_rect((w - length)/2,
                  h*0.4,
                  length, h*0.5,
                  Color(0,0,0,255));
        draw_filled_rect((w - length_bar)/2,
                  h*0.44,
                  length_bar, h*0.42,
                  Color(100,100,100,255));
        draw_filled_rect((w - length_bar)/2,
                  h*0.44,
                  length_bar*on, h*0.42,
                  Color(200,200,200,255));
        if (is_pressed(VSGL_LEFT)){
            on -= SPEED;
            if (on < 0)
                on = 0;
        }
        if (is_pressed(VSGL_RIGHT)){
            on += SPEED;
            if (on > 1)
                on = 1;
        }
        char value[10];
        snprintf(value,10,"%.3f",on);
        draw_text("opensans.ttf",16,value,
                  (w - length)/2 +
                  w*0.8*on,h*0.15,Color(0,0,0,255));
        update();
    }

    //close the library and clean up everything
    close();
    return 0;
}
