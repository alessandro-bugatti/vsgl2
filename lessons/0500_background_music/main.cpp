#include <iostream>
#include <ctime>
#include "../../vsgl2.h"

using namespace std;
using namespace vsgl2;
using namespace vsgl2::general;
using namespace vsgl2::video;
using namespace vsgl2::audio;
using namespace vsgl2::utils;
using namespace vsgl2::io;


int main(int argc, char* argv[]) {

    //init the library
    init();
    //create the window and show it
    set_window(512,512,"Vsgl2 background music");
    //play background music for 5 seconds
    play_music("loopy.wav");
    delay(5000);
    //pause background music for 2 seconds
    pause_music();
    delay(2000);
    //resume playing background music for 3 seconds
    play_music("loopy.wav");
    delay(3000);
    //stop the music
    stop_music();

    wait_for_button_pressed();

    //close the library and clean up everything
    close();
    return 0;
}
