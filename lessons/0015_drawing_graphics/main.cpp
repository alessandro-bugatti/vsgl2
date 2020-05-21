#include <iostream>
#include <algorithm>
#include "../../vsgl2.h"

using namespace std;
using namespace vsgl2;
using namespace vsgl2::general;
using namespace vsgl2::video;
using namespace vsgl2::utils;

const int M = 50;

void draw_data(float v1[], float v2[], int n, int y, int r)
{
    int i;
    float delta = (float)get_window_width()/(n - 1);
    float max_value = *max_element(v1, v1 + n);
    float min_value = *min_element(v1, v1 + n);
    float range = max_value - min_value;

    for (i = 0; i < n-1; i++)
    {
        int ya = y - (v1[i] - min_value)/range*r;
        int yb = y - (v1[i+1] - min_value)/range*r;
        draw_line(delta*i, ya, delta*(i+1), yb, Color(255,0,0,255));
    }
    for (i = 0; i < n-1; i++)
    {
        int ya = y - (v2[i] - min_value)/range*r;
        int yb = y - (v2[i+1] - min_value)/range*r;
        draw_line(delta*i, ya, delta*(i+1), yb, Color(0,0,255,255));
    }
}

int main(int argc, char* argv[]) {
    float data[M], filtered_data[M];
    int i;
    for (i = 0; i < M; i++)
        data[i] = 5 + rand()%10/10.0;
    //data[20] = 7;
    for (i = 1; i < M-1; i++)
        filtered_data[i] = (data[i] +
        data[i+1] + data[i-1])/3;
    filtered_data[0] = (2*data[0] + data[1])/3;
    filtered_data[M-1] = (2*data[M-1] + data[M-2])/3;
    //init the library
    init();
    //create the window and show it
    set_window(756,512,"Vsgl2 graphics drawing");
    draw_data(data, filtered_data, M, get_window_height(),get_window_height());
    //update the screen
    update();
    //the program waits for any button to be pressed
    wait_for_button_pressed();
    //close the library and clean up everything
    close();
    return 0;
}
