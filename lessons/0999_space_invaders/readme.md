# Merry Christmas with squares
To celebrate Christmas 2016, here is a simple application that shows an image surrounded by squares simulating snow flakes, as it can see on the YouTube video below.

[![YouTube video](https://img.youtube.com/vi/Gyp2LQgBBoA/0.jpg)](https://www.youtube.com/watch?v=Gyp2LQgBBoA)

First of all, a structure is defined to store information about a single "snow flake" in form of a C ```struct```.

```c
struct flake{
    int x;
    double y, v;
    Color c;
    flake(){}
};
```

The integer *x* is the x coordinate of the flake, that is supposed to be constant during the entire life of the flake. The *y* coordinate is instead a double, as *v*, the velocity, in order to have a fine control over the speed of each flake.

The C++ ```vector``` container is used to manage all the flakes during the program execution. All the flakes are inizialized with the following function:

```c
void init_flakes()
{
    for (int i = 0; i < N; i++){
        flake f;
        f.x = rand()%get_window_width();
        f.y = (double)rand()/RAND_MAX;
        f.v = 0.000001 + (100 + rand()%200)*0.000001;
        f.c = Color(255,255,255,100 + rand()%155);
        flakes.push_back(f);
    }
}
```
The *x* and *y* coordinates are set using the ```rand``` function, where the *v* variable assume random value between 0.000001 and 0.000299 (this values has been tested on my computer to obtain a nice effect during execution, it depends on the computer "power"). The color is white with a random alpha value to obtain a transparency effect, different for each flake.

The function ```update_flakes``` update each flake moving it along the y direction: when the flake goes behind the lower border of the screen, the *y* coordinate is set to 0 to place the flake at the upper border of the screen.

```c
void update_flakes(){
    for (auto f = flakes.begin(); f!= flakes.end(); ++f){
        f->y += f->v;
        if (f->y >= 1)
            f->y = 0;
    }
}
```

Finally, inside the main loop the Christmas image is drawn, modifying its alpha value from 0 to 255 and viceversa, to obtain the fade in/out effect.
```c
while(!done())
    {
        alpha += dir;
        if (dir && alpha/SPEED > 250)
            dir = - dir;
        if (dir == -1 && alpha < 0)
            dir = - dir;
        draw_filled_rect(0,0,w,h,Color(0,0,100,255));
        draw_image("merry.jpg",32,100,440,221);
        draw_filled_rect(32,100,440,221,Color(0,0,100,255 - (alpha/SPEED)%255));
        update_flakes();
        for (auto f = flakes.begin(); f!= flakes.end(); ++f)
            draw_filled_rect(f->x,
                             f->y * get_window_height(),
                             DIM_FLAKE,DIM_FLAKE,f->c);
        update();
    }
```
