#include <iostream>
#include <ctime>
#include "../../vsgl2.h"

using namespace std;
using namespace vsgl2;
using namespace vsgl2::general;
using namespace vsgl2::video;
using namespace vsgl2::utils;
using namespace vsgl2::io;
using namespace vsgl2::ttf_fonts;

/** Ask for user name using the read_text function
*   and write the input to the screen
*/

int main(int argc, char* argv[]) {

    //init the library
    init();
    //create the window and show it
    set_window(512,512,"Vsgl2 read text");
    //main loop
    //draw the question
    draw_text("lato.ttf",32,"Enter your name:",
              0, 32, Color(54,128,255,255));
    update();
    //read the user's name, waiting for the RETURN key
    //to end the input
    string name = read_text("lato.ttf",32,
                            0,64, Color(54,128,255,255),10);
    //greet the user
    draw_text("lato.ttf",64,"Hello " + name,
              0, 128, Color(54,128,255,255));
    update();
    wait_for_button_pressed();
    //close the library and clean up everything
    close();
    return 0;
}
