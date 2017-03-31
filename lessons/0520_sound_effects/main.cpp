#include <iostream>
#include <ctime>
#include "../../vsgl2.h"

using namespace std;
using namespace vsgl2;
using namespace vsgl2::general;
using namespace vsgl2::video;
using namespace vsgl2::utils;
using namespace vsgl2::io;

int is_inside(int x, int y, int x1, int y1, int w, int h)
{
    if (x >= x1 && x <= x1+w)
        if (y >= y1 && y <= y1+h)
            return 1;
    return 0;
}


int main(int argc, char* argv[]) {

    //init the library
    init();
    //create the window and show it
    set_window(512,512,"Vsgl2 background music");
    //play background music
    //play_music("loopy.wav");
    while (!done())
    {
        if (mouse_left_button_pressed())
        {
            delay(10);
            if (is_inside(get_mouse_x(),get_mouse_y(),20,20,100,100))
                play_sound("explode.wav");
            if (is_inside(get_mouse_x(),get_mouse_y(),220,20,100,100))
                play_sound("shot.wav");
        }
        draw_filled_rect(20,20,100,100,Color(255,0,0,255));
        draw_filled_rect(220,20,100,100,Color(0,255,0,255));
        update();
    }


    //close the library and clean up everything
    close();
    return 0;
}
