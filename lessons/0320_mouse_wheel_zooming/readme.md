# Mouse wheel zooming
This example shows how to use the mouse wheel to obtain zooming effects on an image.

[![YouTube video](https://img.youtube.com/vi/5z9XHCqz2kY/0.jpg)](https://youtu.be/5z9XHCqz2kY)

There are two functions related to the mouse wheel:
```c
int get_mouse_wheel_x();

int get_mouse_wheel_y();
```
The first one is related to the horizontal scrool of the mouse wheel, where the second one is related to the vertical scrool (with a normal mouse only the second version is relevant, because normal mice haven't got an horizontal scrool).  

Each function returns a value that shows if a there is a scrolling in progress: a negative value means that the scrolling is in one direction, a positive one means that it is in the other direction.

Using the ```get_mouse_wheel_y``` function inside the main loop, the program increases or decreases the **zoom** value. Then the ```draw_image``` function uses this value to show the Lena image at different zoom levels, as it can see on the video above.
