#include <iostream>
#include <iomanip>
#include "../../vsgl2.h"

using namespace std;
using namespace vsgl2;
using namespace vsgl2::general;
using namespace vsgl2::video;
using namespace vsgl2::utils;
using namespace vsgl2::ttf_fonts;

#define FOOTER 50
#define H (512 + FOOTER)
#define W 512

int main(int argc, char* argv[])
{
    int in = 0, tot = 0;
    double pi;
    int i = 0xC0u, j =0x80u;
    //init the library
    init();
    //set pixel mode in order the keep
    //all the pixels on the screen
    set_pixel_mode();
    //create the window and show it
    set_window(W,H,"Vsgl2 Pi with Montecarlo method");
    while(!done())
    {
        //draw the circle inscribed in a square
        draw_image("circle.png",0,0,512,512);
        int x = rand()%W;
        int y = rand()%W;
        draw_filled_rect(x,y,3,3,Color(255,0,0,255));
        tot++;
        int x_t = x - W/2;
        int y_t = y - W/2;
        if ( x_t*x_t + y_t*y_t < (W/2)*(W/2))
            in++;
        if (in > 0)
            pi = (((double)in)/tot*4);
        ostringstream str;
        str << setw(7) << right<< in <<
            setw(7) << right << tot <<
            u8"   \u03c0 = " <<
            fixed << setprecision(4) << right <<  pi;
        draw_filled_rect(0,get_window_height()-FOOTER,get_window_width(), FOOTER,
                         Color(0xf0,0xf9,0xff,255));
        draw_text("codepro.ttf",30,str.str(),0,H - FOOTER,Color(0,0,0,255));
        update();
        delay(1);
    }
    //close the library and clean up everything
    close();
    return 0;
}
