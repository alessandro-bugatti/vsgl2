#include <iostream>
#include <sstream>
#include <ctime>
#include "../../vsgl2.h"

using namespace std;
using namespace vsgl2;
using namespace vsgl2::general;
using namespace vsgl2::video;
using namespace vsgl2::utils;
using namespace vsgl2::io;
using namespace vsgl2::ttf_fonts;

/** The retro game Snake
*   Arrow keys to move the snake
*/

/**
    Each single element on the field, it could be
    a segment of the snake's body or an object
*/

const int DIM = 10;
const int WIDTH = 500;
const int HEIGHT = 480;
const int FOOTER = 32;
const int MAX_X = WIDTH/DIM;
const int MAX_Y = HEIGHT/DIM;
const int MAX_SNAKE_LENGTH = 100;
const int UPDATE_TIME = 100;

enum Direction{LEFT, RIGHT, UP, DOWN};

struct Element{
    int x; //logical coordinates, not pixel
    int y;
    int dim;
    string image;
};

struct Snake
{
    Element body[MAX_SNAKE_LENGTH];
    int length;
    Direction dir;
};

void init_snake(Snake &snake, int x, int y, string image)
{
    for (int i = 0; i < 6; i++){
        snake.body[i].x = x;
        snake.body[i].y = y + i;
        snake.body[i].dim = DIM;
        snake.body[i].image = image;
    }
    snake.dir = UP;
    snake.length = 6;
}

void update_snake(Snake &snake)
{
    for (int i = snake.length - 1; i > 0; i--)
        snake.body[i] = snake.body[i-1];
    if (snake.dir == UP)
        snake.body[0].y--;
    if (snake.dir == DOWN)
        snake.body[0].y++;
    if (snake.dir == LEFT)
        snake.body[0].x--;
    if (snake.dir == RIGHT)
        snake.body[0].x++;
}

void draw_element(Element element)
{
    draw_image(element.image, element.x*DIM,
               element.y*DIM, element.dim, element.dim, 255);
}

void draw_snake(Snake snake)
{
    for (int i = 0; i < snake.length; i++)
        draw_element(snake.body[i]);
}

int main(int argc, char* argv[]) {

    int field[MAX_X][MAX_Y];
    Snake snake;
    init_snake(snake, 10, 10, "assets/images/snake.png");

    int update_time = ms_time();
    //init the library
    init();
    //create the window and show it
    set_window(WIDTH, HEIGHT + FOOTER,"Snake");
    set_background_color(Color(0,0,0,255));
    int time = ms_time();
    int fps_time = ms_time();
    int fps = 0;
    ostringstream out;
    out << "FPS: " << fps;
    //main loop
    while(!done())
    {
        fps++;
        draw_snake(snake);
        draw_text("assets/fonts/vt323.ttf",20,out.str(),0,HEIGHT,Color(255,255,255,255));
        if (ms_time() - time > UPDATE_TIME)
        {
            if (is_pressed(VSGL_UP))
                snake.dir = UP;
            if (is_pressed(VSGL_DOWN))
                snake.dir = DOWN;
            if (is_pressed(VSGL_LEFT))
                snake.dir = LEFT;
            if (is_pressed(VSGL_RIGHT))
                snake.dir = RIGHT;
            update_snake(snake);

            time = ms_time();

        }
        //Update the FPS every second
        if (ms_time() - fps_time > 1000)
        {
            out.str("");
            out << "FPS: " << fps;
            fps = 0;
            fps_time = ms_time();
        }
        update();
    }

    //close the library and clean up everything
    close();
    return 0;
}
