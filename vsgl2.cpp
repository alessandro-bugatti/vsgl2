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

#include <iostream>
#include <map>
#include "version.h"
#include "vsgl2.h"

using namespace std;
using namespace AutoVersion;

namespace vsgl2
{

struct vsgl2_image{
    SDL_Texture *texture;
    uint8_t alpha;
};

SDL_Window *window;
int width,height; //window size
SDL_Renderer *renderer;
bool isDone = false;
map<string, vsgl2_image> images;
const Uint8* currentKeyStates;
int mouseX, mouseY;

namespace general
{
void init()
{
    SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO);
    if (!(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) & (IMG_INIT_JPG | IMG_INIT_PNG)))
        SDL_Log("Image subsystem inizialitation error.");
    SDL_Log("VSGL2 version: %s Build %ld",
            AutoVersion::FULLVERSION_STRING,
            AutoVersion::BUILDS_COUNT);
    SDL_Log("SDL init");
}

void close()
{
    SDL_Log("Quitting SDL...");
    //Free loaded image
    for (auto const& i: images)
        SDL_DestroyTexture( i.second.texture);
    //Destroy window
    SDL_DestroyRenderer( renderer );
    SDL_DestroyWindow( window );
    //Quit various subsystems
    IMG_Quit();
    SDL_Log("Done.");
    SDL_Quit();
}

void set_window(int w, int h, string title, int fullscreen)
{
    if (fullscreen == 1)
        fullscreen = SDL_WINDOW_FULLSCREEN;
    window = SDL_CreateWindow(
                 title.c_str(),                      // window title
                 SDL_WINDOWPOS_UNDEFINED,            // initial x position
                 SDL_WINDOWPOS_UNDEFINED,            // initial y position
                 w,                                  // width, in pixels
                 h,                                  // height, in pixels
                 SDL_WINDOW_OPENGL | fullscreen      // flags
             );

    if (window == NULL)
    {
        // In the case that the window could not be made...
        SDL_LogCritical(SDL_LOG_CATEGORY_VIDEO, "Could not create window: %s", SDL_GetError());
        exit(1);
    }
    width = w;
    height = h;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);

}

int get_window_width()
{
    return width;
}

int get_window_height()
{
    return height;
}

}

namespace video
{
bool done()
{
    SDL_RenderClear(renderer);
    return isDone;
}

void update()
{
    SDL_Event e;
    if ( SDL_PollEvent(&e) )
    {
        if (e.type == SDL_QUIT)
            isDone = true;
        else if (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_ESCAPE)
            isDone = true;
        if( e.type == SDL_MOUSEMOTION ||
           e.type == SDL_MOUSEBUTTONDOWN ||
           e.type == SDL_MOUSEBUTTONUP )
                SDL_GetMouseState( &mouseX, &mouseY );
    }
    SDL_RenderPresent(renderer);
}


void draw_point(int x, int y, const Color& c)
{
    Uint8 r, g, b, a;
    SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
    SDL_SetRenderDrawColor(renderer, c.c.r, c.c.g, c.c.b, c.c.a);
        SDL_RenderDrawPoint(renderer, x, y);
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

void draw_rect(int x, int y, int w, int h, const Color& c)
{
    Uint8 r, g, b, a;
    SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);

    SDL_SetRenderDrawColor(renderer, c.c.r, c.c.g, c.c.b, c.c.a);
    SDL_Rect rectangle;

    rectangle.x = x;
    rectangle.y = y;
    rectangle.w = w;
    rectangle.h = h;
    SDL_RenderDrawRect(renderer, &rectangle);
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

void draw_filled_rect(int x, int y, int w, int h, const Color& c)
{
    Uint8 r, g, b, a;
    SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);

    SDL_SetRenderDrawColor(renderer, c.c.r, c.c.g, c.c.b, c.c.a);
    SDL_Rect rectangle;

    rectangle.x = x;
    rectangle.y = y;
    rectangle.w = w;
    rectangle.h = h;
    SDL_RenderFillRect(renderer, &rectangle);
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

void draw_line(int x1, int y1, int x2, int y2, const Color& c)
{
    Uint8 r, g, b, a;
    SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);

    SDL_SetRenderDrawColor(renderer, c.c.r, c.c.g, c.c.b, c.c.a);
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

void draw_image(string image, int x, int y, int w, int h, uint8_t alpha)
{
    if (images.find(image)==images.end())
    {
        SDL_Surface* s = IMG_Load(image.c_str());
        if (s == NULL)
            SDL_Log("Caricamento immagine fallito: %s", SDL_GetError() );
        else
            {
                SDL_Texture * texture = SDL_CreateTextureFromSurface( renderer, s);
                vsgl2_image im;
                im.texture = texture;
                im.alpha = alpha;
                images.insert(make_pair(image,im));
                SDL_FreeSurface(s);
            }
    }
    SDL_Rect r;
    r.x= x;
    r.y = y;
    r.w = w;
    r.h = h;
    SDL_SetTextureAlphaMod( images[image].texture, images[image].alpha);
    SDL_RenderCopy(renderer, images[image].texture,NULL,&r);

}

}//closing namespace video

namespace io
{
    bool is_pressed(int key)
    {
        currentKeyStates = SDL_GetKeyboardState( NULL );
        return (bool)currentKeyStates[key];
    }

    int get_mouse_x()
    {
        return mouseX;
    }

    int get_mouse_y()
    {
        return mouseY;
    }

    bool mouse_left_button_pressed()
    {
        return SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT);
    }

    bool mouse_right_button_pressed()
    {
        return SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT);
    }


}

namespace utils
{

void wait_for_button_pressed()
{
    SDL_Event e;
    bool done = false;
    while(!done)
    {
        delay(1); //to avoid to hung the CPU
        if ( SDL_PollEvent(&e) )
        {
            if (e.type == SDL_KEYDOWN)
                done = true;
            if (e.type == SDL_QUIT)
            {
                close();
                exit(0);
            }

        }
    }

}

void delay(int milliseconds)
{
    SDL_Delay(milliseconds);
}

}//closing namespace utils

}
