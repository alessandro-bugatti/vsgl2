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
    set_window(768,432,"Vsgl2 image drawing");
    //First iteration: no transparency
    draw_image("lena.bmp",350,50,200,200);
    draw_image("zen.jpg",0,0,768,432);
    //update the screen
    update();
    //the program waits for a touch of a button
    wait_for_button_pressed();
    //Second iteration: transparency with a black background
    draw_image("lena.bmp",350,50,200,200);
    draw_image("zen.jpg",0,0,768,432,120);
    //update the screen
    update();
    //the program waits for a touch of a button
    wait_for_button_pressed();
    //Third iteration: same transparency with a white background;
    draw_filled_rect(0,0,get_window_width(), get_window_height(),Color(255,255,255,255));
    draw_image("lena.bmp",350,50,200,200);
    draw_image("zen.jpg",0,0,768,432,120);
    //update the screen
    update();
    //the program waits for a touch of a button
    wait_for_button_pressed();

    //close the library and clean up everything
    close();
    return 0;
}
