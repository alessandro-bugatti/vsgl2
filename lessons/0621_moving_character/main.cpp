#include <iostream>
#include <ctime>
#include "../../vsgl2.h"

using namespace std;
using namespace vsgl2;
using namespace vsgl2::general;
using namespace vsgl2::video;
using namespace vsgl2::utils;
using namespace vsgl2::sprite;

int main(int argc, char* argv[]) {
    //init the library
    init();
    //create the window and show it
    set_window(512,512,"Vsgl2 moving character");

    //main loop
    start_animation("images/templerun_run.png",0,200,0,9,100,0);
    while(!done())
    {


        update();
    }

    //close the library and clean up everything
    close();
    return 0;
}

