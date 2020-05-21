#include <iostream>
#include <algorithm>
#include <ctime>
#include "../../vsgl2.h"

using namespace std;
using namespace vsgl2;
using namespace vsgl2::general;
using namespace vsgl2::video;
using namespace vsgl2::utils;

const int DIM = 64;
const int CELL_DIMENSION = 8;

int life[DIM][DIM];
int temp[DIM][DIM];


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
}

void draw(int m[][DIM])
{
    for (int i = 0; i < DIM; i++)
        for (int j = 0; j < DIM; j++)
            if (m[i][j] == 1)
                draw_filled_rect(i*CELL_DIMENSION, j*CELL_DIMENSION,
                                 CELL_DIMENSION - 1, CELL_DIMENSION - 1,
                                 Color(255,0,0,255));
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
    start(life);
    while(!done())
    {
        draw(life);
        update();
        evolve(life);
        delay(100);
    }

    close();
    return 0;
}
