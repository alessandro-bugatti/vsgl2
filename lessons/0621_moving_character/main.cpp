#include <iostream>
#include <ctime>
#include "../../vsgl2.h"

using namespace std;
using namespace vsgl2;
using namespace vsgl2::general;
using namespace vsgl2::video;
using namespace vsgl2::utils;
using namespace vsgl2::sprite;
using namespace vsgl2::io;


int main(int argc, char* argv[]) {
    //init the library
    init();
    //create the window and show it
    set_window(512,512,"Vsgl2 moving character");

    //main loop
    int hero = start_animation("images/templerun_run.png",0,200,0,9,100,0);
    enum Orientation {
        LEFT,
        RIGHT
    };
    Orientation orientation = RIGHT;
    while(!done())
    {
        delay(10);
        if (is_pressed(VSGL_RIGHT)){
            move_animation(hero, 1, 0);
            if (orientation == LEFT){
                toggle_flip_h_animation(hero);
                orientation = RIGHT;
            }
        }
        if (is_pressed(VSGL_LEFT)){
            if (orientation == RIGHT){
                toggle_flip_h_animation(hero);
                orientation = LEFT;
            }
            move_animation(hero, -1, 0);
        }
        update();
    }
    //close the library and clean up everything
    close();
    return 0;
}

