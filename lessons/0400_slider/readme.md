# Slider
This example introduces how to read the keyboard and the key states, to move the slider back and forth as shown in this video.

[![YouTube video](https://img.youtube.com/vi/0jgxIKP1b-k/0.jpg)](https://www.youtube.com/watch?v=0jgxIKP1b-k)

## Reading the key states
Each key is a sort of button that can be **on** or **off**. Using the ```is_pressed``` function it is possible to detect that state to find out if a particular key is pressed or not.

The only parameter of this function is the key which we are interested in. Each key is represented by a code defined inside the [
vsgl2_keycode.h](
https://github.com/alessandro-bugatti/vsgl2/blob/master/vsgl2_keycode.h) file and it is also mapped to [SDL_Keycode](https://wiki.libsdl.org/SDL_Keycode). For letters and digits the code is VSGL_*char*, where *char* has to be substituted by the actual char (VSGL_a for key **a**, VSGL_1 for key **1**), for other keys it depends, so it is necessary to look at the table.

In this example the code below is used to check if the LEFT key is pressed to update the slider.
```c
if (is_pressed(VSGL_LEFT)){
            on -= SPEED;
            if (on < 0)
                on = 0;
        }
```

This function has to be used inside the main loop and it is clearly no blocking.
