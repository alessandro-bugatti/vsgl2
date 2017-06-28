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
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const char ship[][30] ={
    "images/one.png",
    "images/two.png",
    "images/three.png",
    "images/four.png"
};

struct Object{
    double x, y;
    int w,h;
    char name[30];
    int active;
};


const char main_font[] = "vt323.ttf";
Object alien_ships[N_ROWS][N_SHIPS];
Object tank, bullet;
double spostamento_x = 0;
int spostamento_y = 0;
int dir = 1;
int right_border;

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

void init_ships()
{
    int i, j;
    for ( i = 0; i < N_ROWS; i++)
        for ( j = 0; j < N_SHIPS; j++)
        {
            strncpy(alien_ships[i][j].name,ship[i],30);
            alien_ships[i][j].x = SPACE + j*(DIM+SPACE);
            alien_ships[i][j].y = i*DIM;
            alien_ships[i][j].w = DIM;
            alien_ships[i][j].h = DIM;
            alien_ships[i][j].active = 1;
        }
}

void update_ships()
{
    if (spostamento_x < right_border && dir == 1
            || spostamento_x > SPACE && dir == -1)
            spostamento_x += dir * SPEED;
        else
        {
            dir = -dir;
            spostamento_y += V_MOVE;
        }
    int i, j;
    for ( i = 0; i < N_ROWS; i++)
            for ( j = 0; j < N_SHIPS; j++){
                alien_ships[i][j].x += dir*SPEED;
                alien_ships[i][j].y = spostamento_y + DIM*i;
            }
}

void draw_ships()
{
    int i, j;
    for ( i = 0; i < N_ROWS; i++)
            for ( j = 0; j < N_SHIPS; j++)
                if (alien_ships[i][j].active)
                    draw_image(alien_ships[i][j].name,
                            alien_ships[i][j].x,
                            alien_ships[i][j].y,
                            DIM,DIM,255);

}

void init_tank()
{
    strncpy(tank.name, "images/tank.png",30);
    tank.x = DIM;
    tank.y = SCREEN_HEIGHT*0.9;
    tank.active = 1;
    bullet.h = 5;
    bullet.w = 5;
    bullet.active = 0;
}

void draw_tank()
{
    draw_image(tank.name, tank.x, tank.y, DIM, DIM, 255);
}

void shot()
{
    if (!bullet.active)
    {
        bullet.x = tank.x + tank.w/2;
        bullet.y = tank.y;
        bullet.active = 1;
    }
}

void read_input()
{
    if (is_pressed(VSGL_LEFT))
        tank.x -= 1;
    if (is_pressed(VSGL_RIGHT))
        tank.x += 1;
    if (is_pressed(VSGL_W))
        shot();
}

bool collide(Object a, Object b)
{
    return !(a.y > b.y + b.h ||
             a.y + a.h < b.y ||
             a.x > b.x + b.w ||
             a.x + a.w < b.x);
}

void update_bullet()
{
    if (bullet.active)
        bullet.y -= 0.25;
    if (bullet.y <= 0)
        bullet.active = 0;
}

void draw_bullet()
{
    if (bullet.active)
        draw_filled_rect(bullet.x,bullet.y,5,5,Color(255,255,255,255));
}

bool striked()
{
    int i, j;
    for (i = 0; i < N_ROWS; i++)
        for (j = 0; j < N_ROWS; j++)
        if (bullet.active && alien_ships[i][j].active &&
            collide(bullet,alien_ships[i][j])){
            alien_ships[i][j].active = 0;
            bullet.active = 0;
            bullet.y = tank.y;
            return true;
        }
    return false;
}

int main(int argc, char* argv[]) {

    //init the library
    init();
    //create the window and show it
    set_window(640,480,"Vsgl2 Space Invaders");
    int i, j;
    int w = get_window_width();
    int h = get_window_height();
    right_border = (w - SPACE*2 - (DIM+SPACE)*N_SHIPS);
    init_ships();
    init_tank();

    //splashscreen();
    while(!done())
    {
        update_ships();
        read_input();
        update_bullet();
        draw_filled_rect(0,0,get_window_width(),get_window_height(),Color(0,0,0,255));
        draw_ships();
        draw_tank();
        draw_bullet();
        striked();
        update();
    }

    //close the library and clean up everything
    close();
    return 0;
}
