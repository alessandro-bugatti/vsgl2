/*
  Copyright (C) 2016 Alessandro Bugatti (alessandro.bugatti@istruzione.it)

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef VSGL2_H_INCLUDED
#define VSGL2_H_INCLUDED

/*! \mainpage Very Simple Graphic Library 2
 *
 * \section intro_sec Introduzione
 *
 * This is a very simple (as in its name) library,
 * a sort of wrapper around the very good SDL2 library.
 * It is inspired by the PyGame library, even though I have wanted
 * to stay very C-style, so no classes, no dot-notation in general.
 *
 */

/*! \file
 *  \brief Very Simple Graphic Library 2
 *  \author Alessandro Bugatti
 *  \version 0.1
 *  \date  Creation 2016/11/08
 */

#include <string>
#include <sstream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

using namespace std;

namespace vsgl2
{

#include "vsgl2_keycode.h"

class Color
{

public:
    SDL_Color c;
    Color (){}
    Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
    {
        c.r = r;
        c.g = g;
        c.b = b;
        c.a = a;
    }
};

namespace general
{

const int FULLSCREEN = SDL_WINDOW_FULLSCREEN;

/**
* \brief Use this function to initiate the library, must be called first.
*/
void init();

/**
* \brief Use this function to set the drawing mode
* in order to keep all the shapes drawn on the
* screen using the draw_* functions
* between one cycle and the following
* one during the main drawing loop
*/
void set_pixel_mode();

/**
* \brief Use this function to set the drawing mode
* to normal, e.g. the draw_* functions don't keep
* anything to the screen between one cycle
* and the following
* one during the main drawing loop
*/
void unset_pixel_mode();

/**
* \brief Close and free all the resources, must be called last.
*/
void close();

/**
* \brief Use this function to create a window with the specified dimensions and a title, optionally in full screen
* \param w the width of the window, in screen coordinates
* \param h the height of the window, in screen coordinates
* \param title the title of the window
* \param fullscreen Flag to set the application to fullscreen mode (fullscreen = 1)
* or windowed (fullscreen = 0)
*/
void set_window(int w, int h, string title, int fullscreen = 0);

/**
* \brief Use this function to change the background color
* \param bg The new background color
* \warning The alpha channel has to be configured, but it is not used
*/
void set_background_color(const Color& bg);

/**
* \brief Use this function to get the width of the current window
* \return The width of the current window
*/
int get_window_width();

/**
* \brief Use this function to get the height of the current window
* \return The height of the current window
*/
int get_window_height();

}//closing namespace general

namespace video
{
/**
* \brief Use this function to check when to stop the main loop
* \return True if the user has clicked on the X to close the window
*    or has pressed ESC to close the program
*/
bool done();

/**
* \brief Use this function to reset the isDone glabal variable to its initial state (false)
* This can be useful when you want to use the done() function more then one time
* inside a program, i.e. in a video game to restart a new game
*/
void undone();


/**
* \brief Use this function to update the screen after drawing on it
* Typically called at the end of the main loop
*/
void update();

/** \brief Use to draw a point at x,y coordinates
*   \param x The x coordinate
*   \param y The y coordinate
*   \param c The color of the point
*   \warning In the present implementation this function is very slow,
*   so using it for drawing a large amount of points should be avoided
*/
void draw_point(int x, int y, const Color &c);

/**
*   \brief Use to draw a line
*   \param x1 The x coordinate of an end point
*   \param y1 The y coordinate of an end point
*   \param x2 The x coordinate of the other end point
*   \param y2 The y coordinate of the other end point
*   \param c The color of the line
*/
void draw_line(int x1, int y1, int x2, int y2, const Color &c);

/**
*   \brief Use to draw a rectangle (only the border),
*   use instead draw_filled_rect to draw a
*   rectangle filled with color
*   \param x The x coordinate of the upper left corner
*   \param y The y coordinate of the upper left corner
*   \param w The width of the rectangle
*   \param h The height of the rectangle
*   \param c The color of the rectangle
*/
void draw_rect(int x, int y, int w, int h, const Color &c);

/**
*   \brief Use to draw a filled rectangle,
*   use instead draw_rect to draw an empty
*   rectangle with colored borders
*   \param x The x coordinate of the upper left corner
*   \param y The y coordinate of the upper left corner
*   \param w The width of the rectangle
*   \param h The height of the rectangle
*   \param c The color of the borders of the rectangle
*/
void draw_filled_rect(int x, int y, int w, int h, const Color &c);

/**
*   \brief Use to draw an image (BMP, PNG, JPG)
*   \param image The image name, either absolute or relative
*   \param x The x coordinate of the upper left corner of the image
*   inside the window coordinates
*   \param y The y coordinate of the upper left corner of the image
*   inside the window coordinates
*   \param w The width of the image as it will appear inside the window,
*   not its original value
*   \param h The height of the image as it will appear inside the window,
*   not its original value
*   \param [alpha] The transparency value for the image: with 255 (the default), the image
*   will look as the original, with 0 the image will become invisible
*   \warning The width and the height can be set as wanted, but it is better
*   to have the dimension of the image in the file the same as in w and h
*   to reduce CPU load and improve performance
*/
void draw_image(string image, int x, int y, int w, int h, uint8_t alpha = 255);

}//closing video namespace


namespace audio
{
    /**
    *   \brief Use to play a music in a loop (WAVE, MOD, MIDI, OGG, MP3, FLAC,
    *   and any file that you use a command to play with). It can be typically
    *   used to play a background music.
    *   \param music The music name, either absolute or relative
    *   \warning The music will play in background until it will be paused
    *   or stopped
    */
    void play_music(string music);
    /**
    *   \brief Use to pause the music that is currently playing
    *   \warning If a music is not  currently playing, nothing happens.
    */
    void pause_music();
    /**
    *   \brief Use to stop the music that is currently playing
    *   \warning If a music is not  currently playing, nothing happens.
    */
    void stop_music();
    /**
    *   \brief Use to play a sound (WAVE, MOD, MIDI, OGG, MP3, FLAC,
    *   and any file that you use a command to play with). It can be typically
    *   used to play a sound effect linked to an user action (pressing a button...).
    *   \param sound The sound name, either absolute or relative
    *   \warning The sound will play on its own channel, in this way it is possible to
    *   play multiple sounds at the same time, or the same sound overlapped.
    *   \attention Calling this function without leaving enough time
    *   between two calls, makes the second call useless.
    */
    void play_sound(string sound);
}//closing namespace audio


namespace io
{
/**
*   \brief Use to check if a button is pressed
*   \param key The enum value indicating the key pressed
*   \return True if **key** is pressed, false otherwise
*   \attention All the SDL scancodes (https://wiki.libsdl.org/SDL_Scancode)
*   are translated in a sort of VSGL equivalent notation
*   For example, SDL_SCANCODE_LEFT will become VSGL_LEFT
*   \warning All translated code are in vsgl2_keycode.h,
*   this is a work in progress, so actually only certain codes are translated
*   \todo Translate all the SDL2 scancodes into VSGL codes
*/
    bool is_pressed(int key);

/**
*   \brief Use to read text from the user, the return key stops the input.
*   \param font The filename of the font.
*   \param dim Font dimension
*   \param x The x coordinate of the upper left corner of the text
*   inside the window coordinates
*   \param y The y coordinate of the upper left corner of the text
*   inside the window coordinates
*   \param c The color of the text
*   \param max_length The maximum length of the text that can be written. The default value is 0, which means that there is no limit to the text length, otherwise the user can write only max_length characters. The value range is from 0 to 255, greater numbers than that will be truncated.
*   \return The string typed from the user
*   \warning This function is blocking, i.e. the program is stopped until the user presses the return key.
*/
    string read_text(string font, int dim, int x, int y, Color c, uint8_t max_length = 0);

/**
*   \brief Use to get the mouse x coordinate
*   \return The value of mouse x coordinate
*/

    int get_mouse_x();

/**
*   \brief Use to get the mouse y coordinate
*   \return The value of mouse y coordinate
*/
    int get_mouse_y();

/**
*   \brief Use to get the mouse wheel horizontal value,
*   \return The amount scrolled horizontally,
*   positive (1) to the right and negative (-1) to the left
*/

    int get_mouse_wheel_x();

/**
*   \brief Use to get the mouse wheel vertical value,
*   \return The amount scrolled vertically,
*    positive (1) away from the user and negative (-1) toward the user
*/
    int get_mouse_wheel_y();

/**
*   \brief Use to check if the mouse left button is pressed
*   \return True if the left button is pressed, false otherwise
*/

    bool mouse_left_button_pressed();

/**
*   \brief Use to check if the mouse right button is pressed
*   \return True if the right button is pressed, false otherwise
*/

    bool mouse_right_button_pressed();
}//closing io namespaces

namespace ttf_fonts
{
/**
*   \brief Use this function to display a text on the screen with the
*   desired font and dimension
*   \param font The filename of the font.
*   \param dim Font dimension
*   \param text The text to be displayed
*   \param x The x coordinate of the upper left corner of the text
*   inside the window coordinates
*   \param y The y coordinate of the upper left corner of the text
*   inside the window coordinates
*   \param c The color of the text
*   \warning The font file have to be visible from the program
*/

    void draw_text(string font, int dim, string text, int x, int y, Color c);

/**
*   \brief Use this function to retrieve the width of a string, given
*   the font, the dimension and the text. It can be useful because
*   each character has a different size in general, so it could be very
*   tricky to calculate the actual width of a string.
*   \param font The filename of the font.
*   \param dim Font dimension
*   \param text The string to be displayed
*   \return The width in pixel
*/
    int text_width(string font, int dim, string text);
/**
*   \brief Use this function to retrieve the height of a string, given
*   the font, the dimension and the text. It can be useful because
*   each character has a different size in general, so it could be very
*   tricky to calculate the actual height of a string.
*   \param font The filename of the font.
*   \param dim Font dimension
*   \param text The string to be displayed
*   \return The height in pixel
*/

    int text_height(string font, int dim, string text);

}

namespace utils
{

/**
*   \brief Use this function to wait for any button to be pressed
*   \warning This freezes the program while waiting for the
*   button to be pressed so that no drawing or updating
*   can occur at the same time
*/
void wait_for_button_pressed();

/**
*   \brief Use this function to stop the execution for a given amount of milliseconds
*   \param milliseconds Amount of time the program is stopped, in milliseconds
*/
void delay(int milliseconds);

/**
*   \brief Use this function to get the time from SDL initialization in milliseconds
*   \return Amount of time from SDL initialization in milliseconds
*   \warning This value wraps if the program runs for more than ~49 days
*/
unsigned int ms_time();

/**
*   \brief Use this function to take a screenshot of the window
*   \param filename The filename where the screenshot will be saved
*   \return 0 on success, negative otherwise
*   \note The image is saved in BMP format
*/
int take_screenshot(string filename);

/**
*   \brief Use this function to hide the mouse cursor
*   \note The default state of the mouse cursor after
*   init is visible.
*/

void hide_mouse_cursor();

/**
*   \brief Use this function to show the mouse cursor
*   \note The default state of the mouse cursor after
*   init is visible.
*/

void show_mouse_cursor();

}//closing namaspace utility

}

//This is here to avoid using of namespace by the user of the library
using namespace vsgl2;
using namespace vsgl2::general;
using namespace vsgl2::video;
using namespace vsgl2::audio;
using namespace vsgl2::io;
using namespace vsgl2::ttf_fonts;
using namespace vsgl2::utils;
#endif //VSGL2_H_INCLUDED
