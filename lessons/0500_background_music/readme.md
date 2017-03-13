# Background music
This example introduces how to play a background music and how it is commonly used in games, where only one background music can be played at a time.

## Actions
The background music can be in three different states, *played*, *paused* and *stopped*. For each one of these states there is a function that has to be call to go in the correspondig status:
```c
void play_music(string music);
void pause_music();
void stop_music();
```
The first function needs the filename of the music that is going to play, where the other two don't need the parameter because they work on the music that is playing at that moment. If there is no music playing when these two functions are called, nothing will happen.

In this example there is a blank screen and the background music starts and plays for 5 seconds, then there is a 2 seconds pause, after that the music plays for another 3 seconds and finally the music is stopped and the program waits for a button to be pressed.   
