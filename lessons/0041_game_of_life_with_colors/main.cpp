#include <iostream>
#include <algorithm>
#include <ctime>
#include "../../vsgl2.h"

using namespace std;
using namespace vsgl2;
using namespace vsgl2::general;
using namespace vsgl2::video;
using namespace vsgl2::utils;
using namespace vsgl2::ttf_fonts;
using namespace vsgl2::io;

const int DIM = 56;
const int CELL_DIMENSION = 12;

int life[DIM][DIM];
int temp[DIM][DIM];
Color colors[DIM][DIM];
bool BLENDING_ON = true;

void start(int m[][DIM])
{
    for (int i = 0; i < DIM; i++)
        for (int j = 0; j < DIM; j++)
            if (rand()%6 == 0)
                m[i][j] = 1;
            else
                m[i][j] = 0;
    //Put the border cells to zero
    for (int i = 0; i < DIM; i++)
    {
        m[0][i] = 0;
        m[i][0] = 0;
        m[DIM - 1][i] = 0;
        m[i][DIM - 1] = 0;
    }
    //Create the initial palette of colors
    for (int i = 0; i < DIM; i++)
        for (int j = 0; j < DIM; j++)
            if (i < DIM/2 && j < DIM/2)
                colors[i][j] = Color(66,133,244,255);
            else if (i < DIM/2 && j >= DIM/2)
                colors[i][j] = Color(219,68,55,255);
            else if (i >= DIM/2 && j < DIM/2)
                colors[i][j] = Color(244,160,0,255);
            else if (i >= DIM/2 && j >= DIM/2)
                colors[i][j] = Color(15,157,88,255);
}

void blend (int m[][DIM], int r, int c)
{
    int R = 0, G = 0, B = 0, counter = 0;
    for (int i = r-1; i < r - 1 + 3; i++)
        for (int j = c-1; j < c - 1 + 3; j++)
            if (m[i][j])
            {
                R += colors[i][j].c.r;
                G += colors[i][j].c.g;
                B += colors[i][j].c.b;
                counter++;
            }
    colors[r][c] = Color(R/counter, G/counter, B/counter, 255);
}

void draw(int m[][DIM])
{
    for (int i = 0; i < DIM; i++)
        for (int j = 0; j < DIM; j++)
            if (m[i][j] == 1){
                if (BLENDING_ON)
                    blend(m, i, j);
                draw_filled_rect(i*CELL_DIMENSION, j*CELL_DIMENSION,
                                 CELL_DIMENSION - 1, CELL_DIMENSION - 1,
                                 colors[i][j]);
            }
}

int neighbours(int m[][DIM], int r, int c)
{
    int counter = 0;
    for (int i = r-1; i < r - 1 + 3; i++)
        for (int j = c-1; j < c - 1 + 3; j++)
            counter += m[i][j];
    return counter - m[r][c];
}

void evolve(int m[][DIM])
{
    for (int i = 1; i < DIM-1; i++)
        for (int j = 1; j < DIM-1; j++)
        {
            int n = neighbours(m, i, j);
            //Any live cell with fewer than two live neighbors dies,
            //as if by underpopulation.
            if (m[i][j] == 1 && n < 2)
                temp[i][j] = 0;
            //Any live cell with two or three live neighbors lives
            //on to the next generation.
            if (m[i][j] == 1 && (n == 2 || n == 3))
                temp[i][j] = 1;
            //Any live cell with more than three live neighbors dies,
            //as if by overpopulation.
            if (m[i][j] == 1 && n > 3)
                temp[i][j] = 0;
            //Any dead cell with exactly three live neighbors
            //becomes a live cell, as if by reproduction.
            if (m[i][j] == 0 && n == 3)
                temp[i][j] = 1;
        }
    for (int i = 1; i < DIM-1; i++)
        for (int j = 1; j < DIM-1; j++)
            m[i][j] = temp[i][j];
}

int main(int argc, char* argv[])
{
    //init the library
    init();
    //create the window and show it
    set_window(DIM*CELL_DIMENSION, DIM*CELL_DIMENSION,"Vsgl2 Game of Life");
    srand(time(NULL));
    //Select blended or not blended
    draw_text("Sarabun-Regular.ttf",50,"Blended?(Y/n)",
              0, 0,
              Color(233,23,78,255));
    update();
    string s = read_text("Sarabun-Regular.ttf",50,
              0, 100,
              Color(233,23,78,255),1);
    if (s == "n" || s == "N")
        BLENDING_ON = false;
    start(life);
    while(!done())
    {
        draw(life);
        update();
        evolve(life);
        delay(200);
    }
    close();
    return 0;
}
