#include <iostream>
#include "../../vsgl2.h"

using namespace std;
using namespace vsgl2;
using namespace vsgl2::general;
using namespace vsgl2::video;
using namespace vsgl2::utils;

int main(int argc, char* argv[]) {
    //init the library
    init();
    //create the window and show it
    set_window(512,512,"Vsgl2 line drawing");
    //draw a line from the top-left corner to
    //the bottom-right corner of the screen
    draw_line(0, //top-left corner
              0,
              get_window_width() - 1, //bottom-right corner
              get_window_height() - 1,
              Color(255,0,0,255)); //red line
    //update the screen
    update();
    //the program waits for any button to be pressed
    wait_for_button_pressed();
    //close the library and clean up everything
    close();
    return 0;
}
