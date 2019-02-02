#include <iostream>
#include <ctime>
#include <cstdio>
#include "../../vsgl2.h"
#include "my_library.h"

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
    set_window(512,256,"Vsgl2 buttons");
    //main loop
    int w = get_window_width();
    int h = get_window_height();
    int cont = 0, cont2 = 0;
    int flag = 0;
    int t = ms_time(), t2 = ms_time();
    while(!done())
    {
        char value[10];
        snprintf(value,10,"%d",cont);
        draw_text("opensans.ttf",66,value,
                  (w - 300)/2 , h*0.3 ,Color(0,0,0,255));
        draw_text("opensans.ttf",22,"Press 1 to reset",
                  (w - 400)/2 , h*0.7 ,Color(0,0,0,255));

        char value2[10];

        snprintf(value2,10,"%d",cont2);
        draw_text("opensans.ttf",66,value2,
                  (w + 100)/2 , h*0.3 ,Color(0,0,0,255));
        draw_text("opensans.ttf",22,"Press 2 to reset",
                  w/2 , h*0.7 ,Color(0,0,0,255));
        if (ms_time() - t > 1000)
        {
            cont++;

            t = ms_time();
        }
        if (ms_time() - t2 > 100)
        {
            cont2++;

            t2 = ms_time();
        }
        if (is_pressed_once(VSGL_1))
            cont = 0;
        if (is_pressed_once(VSGL_2))
            cont2 = 0;



        update();
    }

    //close the library and clean up everything
    close();
    return 0;
}
