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
    set_window(512,512,"Vsgl2 image drawing");
    //draw a BMP image
    draw_image("lena.bmp",50,50,200,200);
    //draw a PNG image
    draw_image("moire.png",250,250,300,300);
    //update the screen
    update();
    //the program waits for a touch of a button
    wait_for_button_pressed();
    //close the library and clean up everything
    close();
    return 0;
}
