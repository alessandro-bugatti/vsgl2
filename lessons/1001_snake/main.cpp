#include <iostream>
#include <ctime>
#include "../../vsgl2.h"

using namespace std;
using namespace vsgl2;
using namespace vsgl2::general;
using namespace vsgl2::video;
using namespace vsgl2::utils;
using namespace vsgl2::io;

/** The retro game Snake
*   Arrow keys to move the snake
*/

const int SQUARE = 10;
const int MAX_LENGTH = 30;
const int DIM_X = 50;
const int DIM_Y = 50;
const int EMPTY = 0;
const int SNAKE = 1;
const int BONUS = 2;
const int UPDATE = 100;
const Color COLOR_BG = Color(0,0,0,255);
const Color COLOR_SNAKE = Color(255,0,0,255);
const Color COLOR_BONUS = Color(0,0,0,255);
enum direction{UP, DOWN, LEFT, RIGHT} dir;
struct unit{
    int x,y;
};

unit snake[MAX_LENGTH];

Color field[DIM_X][DIM_Y];

void init_snake(unit snake[], int n)
{
    for (int i=0; i < n; i++)
    {
        snake[i].x = DIM_X/2 - i;
        snake[i].y = DIM_Y/2;
    }
}

int move_snake(unit snake[], int n, int DIR)
{
    unit head;
    switch(DIR){
        case LEFT:
            head.x = snake[0].x - 1;
            head.y = snake[0].y;
            break;
        case RIGHT:
            head.x = snake[0].x + 1;
            head.y = snake[0].y;
            break;
        case UP:
            head.x = snake[0].x;
            head.y = snake[0].y - 1;
            break;
        case DOWN:
            head.x = snake[0].x;
            head.y = snake[0].y + 1;
            break;
    }
    for (int i = n - 1; i > 0; i--)
    {
        snake[i].x = snake[i-1].x;
        snake[i].y = snake[i-1].y;
    }
    snake[0] = head;
}

void add_unit(unit snake[], int n)
{
    snake[n].x = snake[n-1].x;
    snake[n].y = snake[n-1].y;
}

void draw_field(unit snake[], int n)
{
    for (int i = 0; i < DIM_X ; i++)
        for (int j = 0; j < DIM_Y; j++)
            field[i][j] = COLOR_BG;
    for (int i = 0; i < n; i++)
        field[snake[i].x][snake[i].y] = COLOR_SNAKE;

    for (int i = 0; i < DIM_X ; i++)
        for (int j = 0; j < DIM_Y; j++)
            draw_filled_rect(i*SQUARE, j*SQUARE, SQUARE, SQUARE, field[i][j]);
}

int main(int argc, char* argv[]) {

    int length = 5;
    direction dir = LEFT;
    int update_time = ms_time();
    //init the library
    init();
    //create the window and show it
    set_window(512,512,"Snake");

    //main loop
    int w = get_window_width();
    int h = get_window_height();
    init_snake(snake, length);
    int temp = 2;
    while(!done())
    {
        //Input management
        if (is_pressed(VSGL_UP) && dir != DOWN)
            dir = UP;
        if (is_pressed(VSGL_DOWN) && dir != UP)
            dir = DOWN;
        if (is_pressed(VSGL_LEFT) && dir != RIGHT)
            dir = LEFT;
        if (is_pressed(VSGL_RIGHT) && dir != LEFT)
            dir = RIGHT;
        if (ms_time() - update_time > UPDATE)
        {
            if (temp%50 == 1)
            {
                add_unit(snake,length);
                length++;
            }
            temp++;
            move_snake(snake, length, dir);
            update_time = ms_time();
        }
        //update screen
        draw_field(snake, length);
        update();
    }

    //close the library and clean up everything
    close();
    return 0;
}
