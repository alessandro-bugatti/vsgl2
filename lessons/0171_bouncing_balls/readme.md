# Bouncing balls

This lesson shows how to simulate elastic collisions between circles, drawn as basketball balls, as it can see clicking on the video below.

[![YouTube video](https://img.youtube.com/vi/N6EXDXicrjU/0.jpg)](https://youtu.be/N6EXDXicrjU)

The **main** function is composed of some initializations and the classical update-draw loop.

At first, the function `init_balls(Ball balls[], int n)` sets the position and the velocity of each one of the *MAX_BALLS* balls on the screen. Each ball is a `struct` which stores information about the ball's position (its coordinates *x* and *y*), the ball's speed (in both *x* and *y* axes) and a reference to the image to be shown, as a string.

There are two different strategies to put the balls on the screen:
- **Version 1**: the balls will put equally distribuited in space,
to avoid balls that starts "joined". The simple algorithm calculates how many balls can be standing on each row and how many rows can be on the screen, rounding to integer numbers: in this way the number of balls could be less than **MAX_BALLS**.
- **Version 2**: the balls will put using a random distribution in space, so some balls could start "joined". In this case they will probably keep to stay joined for all the simulation.
To switch between them, it is necessary add or remove comments from the preferred strategy.

The core part of this lesson is in the `update_position(Ball balls[], int n)` function. This function receives an array of `Ball` and its dimension, *n*, and updates the position of each ball, according to
its initial position, its velocity and the collisions with other balls, where necessary.
The physical laws used to update the status of each ball can be found [here](https://github.com/alessandro-bugatti/vsgl2/blob/master/lessons/0171_bouncing_balls/2dcollisions2.pdf) (thanks to Chad Berker for the clear explanation).

Finally, the balls are put on the screen using the `draw(Ball balls[], int n)` function, with a loop.
