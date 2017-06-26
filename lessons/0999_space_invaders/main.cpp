#include <iostream>
#include <ctime>
#include <vector>
#include <cstdio>
#include <cstring>
#include "../../vsgl2.h"

using namespace std;
using namespace vsgl2;
using namespace vsgl2::general;
using namespace vsgl2::video;
using namespace vsgl2::utils;
using namespace vsgl2::io;


const int N_SHIPS = 10;
const int N_ROWS = 4;
const int DIM = 30;
const float SPEED = 0.125;
const int SPACE = 10;
const int V_MOVE = 20;

const char ship[][30] ={
    "images/one.png",
    "images/two.png",
    "images/three.png",
    "images/four.png"
};

struct Object{
    int x, y;
    char name[30];
    int alive;
};

const char main_font[] = "vt323.ttf";

void splashscreen()
{
    int start = 3, start_time;
    int fade = 0, fade_time = 0, fade_dir = 1;
    int duration = 256*10; //in ms
    int w,h;
    char number[10];
    sprintf(number,"%d",start);
    w = text_width(main_font,80,"Space Invaders");
    h = text_height(main_font,80,"Space Invaders");
    draw_filled_rect(0,0,get_window_width(), get_window_height(),
                         Color(0,0,0,255));
    draw_text(main_font,80,"Space Invaders",(get_window_width()-w)/2,
                  (get_window_height()-h)/2,
                  Color(255,255,255,255));
    update();
    delay(2000);
    w = text_width(main_font,400,number);
    h = text_height(main_font,400,number);
    start_time = ms_time();
    fade_time = ms_time();
    while(start > 0 && !done())
    {
        if (ms_time() - fade_time >= duration/(256*2))
        {
            fade += fade_dir;
            if (fade >= 255)
                fade_dir = -1;
            if (fade == 0)
            {
                fade = 0;
                fade_dir = 0;
            }

            fade_time = ms_time();
        }
        if (ms_time() - start_time > duration)
        {
            start--;
            sprintf(number,"%d",start);
            w = text_width(main_font,400,number);
            h = text_height(main_font,400,number);
            fade = 0;
            fade_dir = 1;
            start_time = ms_time();
        }
        draw_filled_rect(0,0,get_window_width(), get_window_height(),
                         Color(0,0,0,255));
        draw_text(main_font,400,number,(get_window_width()-w)/2,
                  (get_window_height()-h)/2,
                  Color(255,255,255,fade));
        //update the screen
        update();
    }
}

int main(int argc, char* argv[]) {

    //init the library
    init();
    //create the window and show it
    set_window(640,480,"Vsgl2 Space Invaders");
    //main loop

    int w = get_window_width();
    int h = get_window_height();
    Object ships[N_ROWS][N_SHIPS];
    int i, j;
    for ( i = 0; i < N_ROWS; i++)
        for ( j = 0; j < N_SHIPS; j++)
        {
            strncpy(ships[i][j].name,ship[i],30);
            ships[i][j].x = SPACE + j*(DIM+SPACE);
            ships[i][j].y = i*DIM;
            ships[i][j].alive = 1;
        }
    double spostamento_x = 0;
    int spostamento_y = 0;
    int dir = 1;
    int right_border = (w - SPACE*2 - (DIM+SPACE)*N_SHIPS);
    //splashscreen();
    while(!done())
    {

        if (spostamento_x < right_border && dir == 1
            || spostamento_x > SPACE && dir == -1)
            spostamento_x += dir * SPEED;
        else
        {
            dir = -dir;
            spostamento_y += V_MOVE;
        }
        draw_filled_rect(0,0,get_window_width(),get_window_height(),Color(0,0,0,255));
        for ( i = 0; i < N_ROWS; i++)
            for ( j = 0; j < N_SHIPS; j++)
                draw_image(ships[i][j].name,
                        spostamento_x + ships[i][j].x,
                        spostamento_y + ships[i][j].y,
                        DIM,DIM,255);
        update();
    }

    //close the library and clean up everything
    close();
    return 0;
}
