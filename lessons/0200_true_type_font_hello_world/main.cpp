#include <iostream>
#include "../../vsgl2.h"

using namespace std;
using namespace vsgl2;
using namespace vsgl2::general;
using namespace vsgl2::video;
using namespace vsgl2::utils;
using namespace vsgl2::ttf_fonts;

int main(int argc, char* argv[]) {
    //init the library
    init();
    //create the window and show it
    set_window(512,512,"Vsgl2 image drawing");
    //Write Hello World to the screen using different
    //fonts, dimensions and colors
    draw_text("baloo.ttf",50,"Hello world!",0,0,Color(255,0,0,255));
    draw_text("amatic.ttf",150,"Hello world!",0,100,Color(0,255,0,255));
    draw_text("baloo.ttf",40,"Hello world!",0,0,Color(0,0,255,100));
    draw_text("amatic.ttf",100,"Hello world!",100,300,Color(12,123,43,255));
    draw_text("baloo.ttf",70,"Hello world!",5,405,Color(100,100,100,255));
    draw_text("baloo.ttf",70,"Hello world!",0,400,Color(255,0,255,255));
    //update the screen
    update();
    //the program waits for a touch of a button
    wait_for_button_pressed();
    //close the library and clean up everything
    close();
    return 0;
}
