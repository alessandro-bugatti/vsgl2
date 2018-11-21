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

const int dim = 250;
const int card_width = 200;
const int card_height = 281;
const int top_padding = 100;
const int width = 768;
const int height = 512;
const int start = (width - (dim * 2 + card_width)) / 2;

void show_card_backs()
{
    draw_image("images/back.png",start,top_padding,card_width,card_height);
    draw_image("images/back.png",start + dim,top_padding,card_width,card_height);
    draw_image("images/back.png",start + 2*dim,top_padding,card_width,card_height);
}

void show_first()
{
    draw_image("images/queen.png",start,top_padding,card_width,card_height);
    draw_image("images/jack.png",start + dim,top_padding,card_width,card_height);
    draw_image("images/jack.png",start + 2*dim,top_padding,card_width,card_height);
}

void show_second()
{
    draw_image("images/jack.png",start,top_padding,card_width,card_height);
    draw_image("images/queen.png",start + dim,top_padding,card_width,card_height);
    draw_image("images/jack.png",start + 2*dim,top_padding,card_width,card_height);
}

void show_third()
{
    draw_image("images/jack.png",start,top_padding,card_width,card_height);
    draw_image("images/jack.png",start + dim,top_padding,card_width,card_height);
    draw_image("images/queen.png",start + 2*dim,top_padding,card_width,card_height);
}

void show_cards(int n)
{
    switch(n)
    {
        case 0: show_card_backs();
            break;
        case 1: show_first();
            break;
        case 2: show_second();
            break;
        case 3: show_third();
            break;
    }
}

int choosen_card(int x, int y)
{
    if (x > start && x < start + card_width &&
        y > top_padding && y < top_padding + card_height)
        return 1;
    if (x > start + dim && x < start + card_width + dim &&
        y > top_padding && y < top_padding + card_height)
        return 2;
    if (x > start + dim*2&& x < start + card_width + dim*2 &&
        y > top_padding && y < top_padding + card_height)
        return 3;
    return 0;
}

int main(int argc, char* argv[]) {
    char coordinates[20];
    //init the library
    init();
    //create the window and show it
    set_window(width, height, "Vsgl2 Guess The Card");
    srand(time(NULL));
    int computer_choice = rand()%3 + 1;
    int state = 0;
    int your_choice;
    set_background_color(Color(212,250,180,0));
    while(!done())
    {
        draw_text("arvo.ttf",50,"Where is the Queen?",30,0,
                  Color(50,50,155,255));
        show_cards(state);
        if (mouse_left_button_pressed())
        {
            your_choice = choosen_card(get_mouse_x(), get_mouse_y());
            if (your_choice != 0)
            {
                show_cards(computer_choice);
                if (your_choice == computer_choice)
                    draw_text("arvo.ttf",30,"YOU WON! Press a button to try again...",30,
                              top_padding + card_height + 20 ,
                            Color(50,175,50,255));
                else
                    draw_text("arvo.ttf",30,"YOU LOST! Press a button to try again...",30,
                      top_padding + card_height + 20,
                      Color(255,0,0,255));
                update();
                wait_for_button_pressed();
                computer_choice = rand()%3 + 1;
            }
        }
        update();
    }
    close();
    return 0;
}
