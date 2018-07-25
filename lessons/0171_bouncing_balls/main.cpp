#include <iostream>
#include <ctime>
#include <cmath>
#include "../../vsgl2.h"

using namespace std;
using namespace vsgl2;
using namespace vsgl2::general;
using namespace vsgl2::video;
using namespace vsgl2::utils;

const int MAX_BALLS= 30;
const int RAGGIO = 30;

struct Ball{
    double x, y;
    double vx, vy;
    string image;
};

void init_balls(Ball balls[], int n);
void update_position(Ball balls[], int n);
void draw(Ball balls[], int n);


int main(int argc, char* argv[]) {
    Ball balls[MAX_BALLS];
    //init the library
    init();
    //create the window and show it
    set_window(512,512,"Vsgl2 bouncing balls");
    srand(time(NULL));
    init_balls(balls, MAX_BALLS);
    //main loop
    while(!done())
    {
        delay(1);
        update_position(balls, MAX_BALLS);
        draw(balls, MAX_BALLS);
        update();
    }

    //close the library and clean up everything
    close();
    return 0;
}



void init_balls(Ball balls[], int n)
{
    /*
    To switch between version 1 and 2, manually
    commenting or removing comments from the version
    you want
    */
    //Version 1 parameters
    unsigned int area = MAX_BALLS * get_window_width() / get_window_height();
    unsigned int length = sqrt(area);
    unsigned int width = area/length;
    for (int i = 0;i < n ;i++)
    {
        //Version 1: equally distribuited in space,
        //to avoid balls that starts "joined",
        //the number of balls could be less than MAX_BALLS

        balls[i].x = (i%width)*(get_window_width()/width);
        balls[i].y = (i/length)*(get_window_height()/length);

        //Version 2: random distribuited in space,
        //some balls could start "joined"
        //balls[i].x = rand()%(get_window_width() - 2*RAGGIO) + RAGGIO;
        //balls[i].y = rand()%(get_window_height() - 2*RAGGIO) + RAGGIO;

        balls[i].vx = (double)rand()/100000;
        balls[i].vy = (double)rand()/100000;
        balls[i].image = "basketball.png";
    }
}

void update_position(Ball balls[], int n)
{
    int i, j;
    for (i = 0;i < n ;i++)
    {
        balls[i].x += balls[i].vx ;
        balls[i].y += balls[i].vy;
        if (balls[i].x >= get_window_width() - RAGGIO || balls[i].x < 0)
            balls[i].vx = -balls[i].vx;
        if (balls[i].y >= get_window_height() - RAGGIO || balls[i].y < 0)
            balls[i].vy = -balls[i].vy;
    }
    for (i = 0;i < n ;i++)
        for (j = i+1;j < n ;j++)
        {
            double c1 = (balls[i].x - balls[j].x)*
                (balls[i].x - balls[j].x);
            double c2 = (balls[i].y - balls[j].y)*
                (balls[i].y - balls[j].y);
            //Controllo di collisione
            if (c1 + c2 < 1*(RAGGIO)*(RAGGIO))
            {
                double unx, uny, utx, uty;
                double d;
                double v1n, v1t, v2n, v2t;
                double vp1n, vp1t, vp2n, vp2t;
                d = sqrt((balls[j].x - balls[i].x)*(balls[j].x - balls[i].x) +
                    (balls[j].y - balls[i].y)*(balls[j].y - balls[i].y));
                unx = (balls[j].x - balls[i].x)/d;
                uny = (balls[j].y - balls[i].y)/d;
                utx = -uny;
                uty = unx;
                v1n = unx*balls[i].vx + uny*balls[i].vy;
                v1t = utx*balls[i].vx + uty*balls[i].vy;
                v2n = unx*balls[j].vx + uny*balls[j].vy;
                v2t = utx*balls[j].vx + uty*balls[j].vy;
                vp1n = v2n;
                vp1t = v1t;
                vp2n = v1n;
                vp2t = v2t;
                balls[i].vx = vp1n * unx + vp1t * utx;
                balls[i].vy = vp1n * uny + vp1t * uty;
                balls[j].vx = vp2n * unx + vp2t * utx;
                balls[j].vy = vp2n * uny + vp2t * uty;
            }
        }

}

void draw(Ball balls[], int n)
{
    for (int i = 0;i < n;i++)
        draw_image(balls[i].image, balls[i].x, balls[i].y,
                   RAGGIO, RAGGIO, 255);
}
