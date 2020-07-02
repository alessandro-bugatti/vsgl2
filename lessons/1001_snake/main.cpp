#include <iostream>
#include <sstream>
#include <ctime>
#include <fstream>
#include "../../vsgl2.h"

using namespace std;
using namespace vsgl2;
using namespace vsgl2::general;
using namespace vsgl2::video;
using namespace vsgl2::utils;
using namespace vsgl2::io;
using namespace vsgl2::ttf_fonts;
using namespace vsgl2::audio;

/** The retro game Snake
*   Arrow keys to move the snake
*/

/**
    Each single element on the field, it could be
    a segment of the snake's body or an object
*/

const int DIM = 8;
const int WIDTH = 512;
const int HEIGHT = 512;
const int FOOTER = 32;
const int MAX_X = WIDTH/DIM;
const int MAX_Y = HEIGHT/DIM;
const int MAX_SNAKE_LENGTH = 500;
const int UPDATE_TIME = 100;

enum Direction{LEFT, RIGHT, UP, DOWN};

enum Object{NOTHING, SNAKE, WALL, APPLE, LEMON};

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
    int bonus;
};

struct Field{
    Object cells[MAX_X][MAX_Y];
};

void init_snake(Snake &snake, int x, int y, string image, int len, Field &f)
{
    for (int i = 0; i < len; i++){
        snake.body[i].x = x;
        snake.body[i].y = y + i;
        snake.body[i].dim = DIM;
        snake.body[i].image = image;
        f.cells[x][y+1] = SNAKE;
    }
    snake.dir = UP;
    snake.length = len;
    snake.bonus = 0;
}

void update_snake(Snake &snake, Field &f)
{
    int xt, yt;
    xt = snake.body[snake.length - 1].x;
    yt = snake.body[snake.length - 1].y;
    if (snake.bonus > 0)
    {
        snake.body[snake.length] = snake.body[snake.length - 1];
        snake.length++;
        snake.bonus--;
    }
    else if(snake.bonus < 0)
    {
        xt = snake.body[snake.length-1].x;
        yt = snake.body[snake.length-1].y;
        f.cells[xt][yt] = NOTHING;
        xt = snake.body[snake.length-2].x;
        yt = snake.body[snake.length-2].y;
        f.cells[xt][yt] = NOTHING;
        snake.length--;
        snake.bonus++;
    }
    else
        f.cells[xt][yt] = NOTHING;
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
    int x, y;
    x = snake.body[0].x;
    y = snake.body[0].y;
    f.cells[x][y] = SNAKE;

}

void init_element(Element &e, int x, int y, string image, Object o, Field &f)
{
    e.dim = DIM;
    e.x = x;
    e.y = y;
    e.image = image;
    f.cells[x][y] = o;
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


Object check_collisions(Field &f, Snake &snake)
{
    int x, y;
    x = snake.body[0].x;
    y = snake.body[0].y;
    if (snake.dir == UP)
        y = snake.body[0].y - 1;
    if (snake.dir == DOWN)
        y = snake.body[0].y + 1;
    if (snake.dir == LEFT)
        x = snake.body[0].x - 1;
    if (snake.dir == RIGHT)
        x = snake.body[0].x + 1;
    return f.cells[x][y];
}

//Only for debug purposes
void draw_field(Field &f)
{
    for (int i = 0; i < MAX_X; i++)
        for (int j = 0; j < MAX_Y; j++)
            if (f.cells[i][j] == SNAKE)
        draw_element({i,j,DIM,"assets/images/snake.png"});
}

void load_walls(string filename, Field &f)
{
    ifstream in(filename);
    if (!in)
    {
        cout << "Error in walls' file opening" << endl;
        return;
    }
    for (int i = 0; i < MAX_X; i++)
        for (int j = 0; j < MAX_Y; j++)
        {
            int temp;
            in >> temp;
            if (temp)
                f.cells[j][i] = WALL;
        }
    in.close();
}

void draw_walls(Field &f)
{
    for (int i = 0; i < MAX_X; i++)
        for (int j = 0; j < MAX_Y; j++)
            if (f.cells[i][j] == WALL)
                draw_filled_rect(i*DIM, j*DIM, DIM, DIM, Color(255,0, 0, 255));
}

int main(int argc, char* argv[]) {

    Field field;
    for (int i = 0; i < MAX_X; i++)
        for (int j = 0; j < MAX_Y; j++)
            field.cells[i][j] = NOTHING;
    Snake snake;
    Element apple, lemon;
    int score = 0;
    init_snake(snake, 10, 10, "assets/images/snake.png", 15, field);
    init_element(apple, rand()%MAX_X, rand()%MAX_Y, "assets/images/apple.png", APPLE, field );
    init_element(lemon, rand()%MAX_X, rand()%MAX_Y, "assets/images/orange.png", LEMON, field );
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
    out << "FPS: " << fps << " Score: " << score;
    load_walls("walls.txt", field);
    play_music("assets/sounds/blazer.wav");
    //main loop
    while(!done())
    {
        fps++;
        draw_snake(snake);
        //draw_field(field);
        draw_element(apple);
        draw_element(lemon);
        draw_walls(field);
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
            Object o = check_collisions(field, snake);
            if (o == SNAKE || o == WALL)
                break;
            if (o == APPLE)
            {
                play_sound("assets/sounds/apple.mp3");
                score += 100;
                out.str("");
                out << "FPS: " << fps << " Score: " << score;
                snake.bonus = 10;
                apple.x = rand()%MAX_X;
                apple.y = rand()%MAX_Y;
                field.cells[apple.x][apple.y] = APPLE;
            }

            if (o == LEMON)
            {
                play_sound("assets/sounds/orange.mp3");
                score += 50;
                out.str("");
                out << "FPS: " << fps << " Score: " << score;
                snake.bonus = -10;
                lemon.x = rand()%MAX_X;
                lemon.y = rand()%MAX_Y;
                field.cells[lemon.x][lemon.y] = LEMON;
            }
            update_snake(snake, field);

            time = ms_time();

        }
        //Update the FPS every second
        if (ms_time() - fps_time > 1000)
        {
            out.str("");
            out << "FPS: " << fps << " Score: " << score;
            fps = 0;
            fps_time = ms_time();
        }
        update();
    }

    //close the library and clean up everything
    close();
    return 0;
}
