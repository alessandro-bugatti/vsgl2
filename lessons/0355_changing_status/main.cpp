#include <iostream>
#include <cstdio>
#include <ctime>
#include "../../vsgl2.h"

using namespace std;
using namespace vsgl2;
using namespace vsgl2::general;
using namespace vsgl2::io;
using namespace vsgl2::video;
using namespace vsgl2::utils;
using namespace vsgl2::ttf_fonts;

const int HOME = 0;
const int ONE_PLAYER = 1;
const int TWO_PLAYERS = 2;

bool DOWN = false;

int status = HOME;

int which_button(int x, int y)
{
    if (x >= 40 && x < 190 &&
        y >= 250 && y < 300)
            return 1;
    if (x >= 290 && x < 440 &&
        y >= 250 && y < 300)
            return 2;
    return 0;
}

void home()
{
    draw_image("images/home.png", 0,0,512,512,255);
    if (mouse_left_button_pressed() && !DOWN)
        if (which_button(get_mouse_x(), get_mouse_y()) == 1)
            {
                status = ONE_PLAYER;
                DOWN = true;
            }
        else if (which_button(get_mouse_x(), get_mouse_y()) == 2)
            {
                status = TWO_PLAYERS;
                DOWN = true;
            }
}

void one_player()
{
    draw_image("images/one_player.png", 0,0,512,512,255);
    if (mouse_left_button_pressed() && !DOWN)
        if (which_button(get_mouse_x(), get_mouse_y()) == 1)
            {
                status = HOME;
                DOWN = true;
            }
        else if (which_button(get_mouse_x(), get_mouse_y()) == 2)
            {
                status = TWO_PLAYERS;
                DOWN = true;
            }

}

void two_players()
{
    draw_image("images/two_players.png", 0,0,512,512,255);
    if (mouse_left_button_pressed() && !DOWN)
        if (which_button(get_mouse_x(), get_mouse_y()) == 1)
            {
                status = HOME;
                DOWN = true;
            }
        else if (which_button(get_mouse_x(), get_mouse_y()) == 2)
            {
                status = ONE_PLAYER;
                DOWN = true;
            }
}

void release_mouse_button()
{
    if (!mouse_left_button_pressed() && DOWN)
        DOWN = false;
}

int main(int argc, char* argv[]) {
    //init the library
    init();
    //create the window and show it
    set_window(512, 512, "Vsgl2 change status");

    set_background_color(Color(0,0,0,255));
    while(!done())
    {
        if (status == HOME)
            home();
        else if (status == ONE_PLAYER)
            one_player();
        else if (status == TWO_PLAYERS)
            two_players();
        release_mouse_button();
        update();
    }
    close();
    return 0;
}
