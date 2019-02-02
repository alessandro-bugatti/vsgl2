#include <iostream>
#include <cstdio>
#include "../../vsgl2.h"

using namespace std;
using namespace vsgl2;
using namespace vsgl2::general;
using namespace vsgl2::video;
using namespace vsgl2::utils;
using namespace vsgl2::ttf_fonts;

int main(int argc, char* argv[]) {
    int start = 5, start_time;
    int fade = 0, fade_time = 0, fade_dir = 1;
    int duration = 256*10;
    int w,h;
    char number[10];
    //init the library
    init();
    //create the window and show it
    set_window(512,512,"Vsgl2 countdown");
    start_time = ms_time();
    snprintf(number,10,"%d",start);
    w = text_width("audiowide.ttf",400,number);
    h = text_height("audiowide.ttf",400,number);

    while(start > 0 && !done())
    {
        if (ms_time() - fade_time >= duration/(256*2))
        {
            fade += fade_dir;
            if (fade >= 225)
                fade_dir = -1;
            if (fade == 0)
            {
                fade = 0;
                fade_dir = 0;
            }

            fade_time = ms_time();
        }
        if (ms_time() - start_time > duration)
        {
            start--;
            snprintf(number,10,"%d",start);
            w = text_width("audiowide.ttf",400,number);
            h = text_height("audiowide.ttf",400,number);
            fade = 0;
            fade_dir = 1;
            start_time = ms_time();
        }
        draw_filled_rect(0,0,get_window_width(), get_window_height(),
                         Color(0,0,123,255));
        draw_text("audiowide.ttf",400,number,(get_window_width()-w)/2,
                  (get_window_height()-h)/2,Color(122,144,203,fade));
        //update the screen
        update();
    }
    //close the library and clean up everything
    close();
    return 0;
}
