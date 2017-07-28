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
using namespace vsgl2::audio;


const int N_SHIPS = 10;
const int MAX_ALIEN_BULLETS = 5;
const int N_ROWS = 4;
const int DIM = 30;
float SPEED = 0.15;
const float BULLET_SPEED = 0.65;
const float ALIEN_BULLET_SPEED = 0.25;
const int SPACE = 10;
const int V_MOVE = 20;
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int RECHARGING_TIME = 1000;

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

struct Score{
    char name[4];
    int score;
};

const char main_font[] = "vt323.ttf";
const char scores_file[] = "scores.txt";
Object alien_ships[N_ROWS][N_SHIPS];
Object tank, bullet;
Object alien_bullets[MAX_ALIEN_BULLETS];
Score best_scores[10];
int level = 1;
double spostamento_x;
int dir;
int right_border;
int points = 0;
int hit_value = 100;
int lives = 3;
int last_shot_time = 0;

void read_best_scores()
{
    FILE *in = fopen(scores_file,"r");
    if (in == NULL)
        return;
    int cont = 0;
    while (fscanf(in,"%s %d",best_scores[cont].name,
                  &best_scores[cont].score) != EOF && cont < 10)
        cont++;
}

void save_best_scores()
{
    FILE *out = fopen(scores_file,"w");
    if (out == NULL)
        return;
    int cont = 0;
    while (best_scores[cont].name[0] != '\0')
    {
        fprintf(out, "%s %d\n", best_scores[cont].name,
                best_scores[cont].score);
        cont++;
    }
}

int minimum_score()
{
    return best_scores[9].score;
}

char read_key()
{
    SDL_Event e;
    bool done = false;
    while(!done)
    {
        delay(1); //to avoid to hung the CPU
        if ( SDL_PollEvent(&e) )
        {
            if (e.type == SDL_KEYDOWN)
                return (char)e.key.keysym.sym;
        }
    }

}

void insert_name(char *s)
{
    int i;
    draw_filled_rect(0,0,get_window_width(), get_window_height(),
                         Color(0,0,0,255));
    draw_text(main_font,60,"INSERT YOUR NAME",SCREEN_WIDTH/4,
                  SCREEN_HEIGHT/4,
                  Color(255,255,255,255));
    update();
    for (i = 0; i < 3; i++)
    {
        char c = read_key();
        c = toupper(c);
        s[i] = c;
        s[i+1] = '\0';
        draw_filled_rect(0,0,get_window_width(), get_window_height(),
                         Color(0,0,0,255));
        draw_text(main_font,60,"INSERT YOUR NAME",SCREEN_WIDTH/4,
                  SCREEN_HEIGHT/4,
                  Color(255,255,255,255));
        draw_text(main_font,100,s,SCREEN_WIDTH/3,
                  SCREEN_HEIGHT/2,
                  Color(255,255,255,255));
        update();
    }
    wait_for_button_pressed();
}

void add_new_record()
{
    int i = 0;
    while (best_scores[i].score > points) i++;
    char temp[100];
    insert_name(temp);
    int pos = i;
    for (int j = 9; j > pos; j--)
    {
        best_scores[j] = best_scores[j-1];
    }
    strncpy(best_scores[pos].name,temp,3);
    best_scores[pos].score = points;
}


void draw_best_scores()
{
    int i = 0;
    int w = text_width(main_font,80,"Best scores");
    int h = text_height(main_font,80,"Best scores");

    draw_filled_rect(0,0,get_window_width(), get_window_height(),
                         Color(0,0,0,255));
    draw_text(main_font,80,"Best scores",(get_window_width()-w)/2,
                  10,
                  Color(255,255,255,255));
    for (i = 0; i < 10 ; i++)
    {
        if (best_scores[i].name[0] != '\0')
            draw_text(main_font,34,best_scores[i].name,180,
                  100 + 36*i,
                  Color(255,255,255,255));
        else
            draw_text(main_font,34,"___",180,
                  100 + 36*i,
                  Color(255,255,255,255));
        char score[100];
        snprintf(score,100,"%05d",best_scores[i].score);
        draw_text(main_font,34,score,320,
                  100 + 36*i,
                  Color(255,255,255,255));

    }
    update();
}

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
    dir = 1;
    spostamento_x = 0;
    for ( i = 0; i < N_ROWS; i++)
        for ( j = 0; j < N_SHIPS; j++)
        {
            strncpy(alien_ships[i][j].name,ship[i],30);
            alien_ships[i][j].x = SPACE + j*(DIM+SPACE);
            alien_ships[i][j].y = i*DIM + DIM;
            alien_ships[i][j].w = DIM;
            alien_ships[i][j].h = DIM;
            alien_ships[i][j].active = 1;
        }
    for ( i = 0; i < MAX_ALIEN_BULLETS; i++)
    {
        alien_bullets[i].w = 5;
        alien_bullets[i].h = 5;
        alien_bullets[i].active = 0;
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
        int i, j;
        for ( i = 0; i < N_ROWS; i++)
            for ( j = 0; j < N_SHIPS; j++)
                alien_ships[i][j].y += V_MOVE;
    }
    int i, j;
    for ( i = 0; i < N_ROWS; i++)
            for ( j = 0; j < N_SHIPS; j++)
                alien_ships[i][j].x += dir*SPEED;
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
                            alien_ships[i][j].w,
                            alien_ships[i][j].h,
                            255);

}

void init_tank()
{
    strncpy(tank.name, "images/tank.png",30);
    tank.x = DIM;
    tank.y = SCREEN_HEIGHT*0.9;
    tank.w = 30;
    tank.h = 15;
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
        play_sound("sounds/shot.wav");
    }
}

bool recharged()
{
    if (ms_time() - last_shot_time > RECHARGING_TIME)
        return true;
    return false;
}

int which_ship_shooting()
{
    int ship = rand()%N_SHIPS;
    int i, sum = 0;
    //check if at least one ship is still alive in the column
    //selected by ship variable
    for (i = 0; i < N_ROWS; i++)
        sum += alien_ships[i][ship].active;
    //no one ship is alive in the column
    //selected by ship variable,so return -1
    if (sum == 0)
        return -1;
    //else return column selected
    return ship;
}

void alien_shot()
{
    if (!recharged())
        return;
    last_shot_time = ms_time();
    int column = which_ship_shooting();
    if (column == -1)
        return;
    int i;
    for (int i = 0; i < MAX_ALIEN_BULLETS; i++)
        if (!alien_bullets[i].active)
        {
            alien_bullets[i].x = rand()%N_SHIPS*(DIM+SPACE) + alien_ships[0][0].x;
            alien_bullets[i].y = alien_ships[N_ROWS-1][0].y + DIM;
            alien_bullets[i].active = 1;
            play_sound("sounds/shot.wav");
            return;
        }
}

void read_input()
{
    if (is_pressed(VSGL_LEFT))
        tank.x -= 1;
    if (is_pressed(VSGL_RIGHT))
        tank.x += 1;
    if (is_pressed(VSGL_SPACE))
        shot();
}

bool collide(Object a, Object b)
{
    return !(a.y > b.y + b.h ||
             a.y + a.h < b.y ||
             a.x > b.x + b.w ||
             a.x + a.w < b.x);
}

void update_tank()
{
    if (tank.x < 0)
        tank.x = 0;
    if (tank.x + tank.w > SCREEN_WIDTH)
        tank.x = SCREEN_WIDTH - tank.w;
}

void update_bullet()
{
    if (bullet.active)
        bullet.y -= BULLET_SPEED;
    if (bullet.y <= alien_ships[0][0].y)
        bullet.active = 0;
}

void draw_bullet()
{
    if (bullet.active)
        draw_filled_rect(bullet.x,bullet.y,bullet.w,bullet.h,
                         Color(0,255,0,255));
}

void update_alien_bullets()
{
    int i;
    for (int i = 0; i < MAX_ALIEN_BULLETS; i++)
    {
        if (alien_bullets[i].active)
            alien_bullets[i].y += ALIEN_BULLET_SPEED;
        if (alien_bullets[i].y >= SCREEN_HEIGHT)
            alien_bullets[i].active = 0;
    }

}

void draw_alien_bullets()
{
    int i;
    for (int i = 0; i < MAX_ALIEN_BULLETS; i++)
       if (alien_bullets[i].active)
        draw_filled_rect(alien_bullets[i].x,alien_bullets[i].y,
                         alien_bullets[i].w,alien_bullets[i].h,
                         Color(255,255,255,255));
}

bool alien_striked()
{
    int i, j;
    for (i = 0; i < N_ROWS; i++)
        for (j = 0; j < N_SHIPS; j++)
        if (bullet.active && alien_ships[i][j].active &&
            collide(bullet,alien_ships[i][j])){
            alien_ships[i][j].active = 0;
            bullet.active = 0;
            bullet.y = tank.y;
            play_sound("sounds/explode.wav");
            return true;
        }
    return false;
}

bool tank_striked()
{
    int i, j;
    for (int i = 0; i < MAX_ALIEN_BULLETS; i++)
       if (alien_bullets[i].active &&
            collide(alien_bullets[i],tank)){
            play_sound("sounds/explode.wav");
            return true;
        }
    return false;
}

void draw_points()
{
    char s[20];
    sprintf(s,"Level %d %05d", level, points);
    draw_text(main_font,20,s,get_window_width()*8/10,
                  10,
                  Color(255,255,255,255));
}

void draw_lives()
{
    for (int i = 0; i < lives; i++)
        draw_image(tank.name, i*DIM, 10, DIM/2, DIM/2, 255);
}

bool lost_life()
{
    int i, j;
    for (i = 0; i < N_ROWS; i++)
        for (j = 0; j < N_SHIPS; j++)
        if (alien_ships[i][j].active &&
            (alien_ships[i][j].y + alien_ships[i][j].h) > tank.y)
            return true;
    if (tank_striked()) return true;
    return false;
}

bool level_completed()
{
    int i, j;
    for (i = 0; i < N_ROWS; i++)
        for (j = 0; j < N_SHIPS; j++)
            if (alien_ships[i][j].active == 1)
                return false;
    return true;
}

void game_over()
{
    int w = text_width(main_font,80,"Game over");
    int h = text_height(main_font,80,"Game over");

    draw_filled_rect(0,0,get_window_width(), get_window_height(),
                         Color(0,0,0,255));
    draw_text(main_font,80,"Game over",(get_window_width()-w)/2,
                  (get_window_height()-h)/2,
                  Color(255,255,255,255));
    char score[100];
    snprintf(score,100,"Score: %05d",points);
    w = text_width(main_font,60,score);
    draw_text(main_font,60,score,(get_window_width()-w)/2,
                  (get_window_height()-h)/2 + 100,
                  Color(255,255,255,255));
    update();

}

void repeat_game()
{
    int w = text_width(main_font,80,"Play again?");
    int h = text_height(main_font,80,"Play again?");

    draw_filled_rect(0,0,get_window_width(), get_window_height(),
                         Color(0,0,0,255));
    draw_text(main_font,80,"Play again?",(get_window_width()-w)/2,
                  (get_window_height()-h)/2,
                  Color(255,255,255,255));
    w = text_width(main_font,80,"[y/n]");
    h = text_height(main_font,80,"[y/n]");
    draw_text(main_font,80,"[y/n]",(get_window_width()-w)/2,
                  (get_window_height()-h)/2 + 80,
                  Color(255,255,255,255));
    update();

}

int main(int argc, char* argv[]) {

    //init the library
    init();
    //create the window and show it
    set_window(640,480,"Vsgl2 Space Invaders");
    int i;
    right_border = (get_window_width() - SPACE*2 - (DIM+SPACE)*N_SHIPS);
    read_best_scores();
    play_music("sounds/loopy.wav");
    splashscreen();
    do{
        lives = 3;
        points = 0;
        undone();
        init_ships();
        init_tank();
        while(!done() && lives > 0)
        {
            update_ships();
            read_input();
            alien_shot();
            update_tank();
            update_bullet();
            update_alien_bullets();
            draw_filled_rect(0,0,get_window_width(),get_window_height(),
                             Color(0,0,0,255)); //the background
            draw_ships();
            draw_tank();
            draw_bullet();
            draw_alien_bullets();
            if (alien_striked())
                points += hit_value;
            if (lost_life())
            {
                init_ships();
                init_tank();
                lives--;
            }
            if (level_completed())
            {
                init_ships();
                init_tank();
                SPEED += 0.05;
                lives++;
                level++;
            }
            draw_points();
            draw_lives();
            update();
        }
        game_over();
        wait_for_button_pressed();
        if (points >= minimum_score())
            add_new_record();
        draw_best_scores();
        wait_for_button_pressed();
        repeat_game();
    }while('y' == read_key());
    stop_music();
    save_best_scores();
    //close the library and clean up everything
    close();
    return 0;
}

