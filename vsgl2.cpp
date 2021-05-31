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
#include <list>
#include <fstream>
#include "version.h"
#include "vsgl2.h"

using namespace std;
using namespace AutoVersion;

namespace vsgl2
{

const int NUMBER_OF_CHANNELS = 16;

SDL_Window *window;
int width,height; //window size
SDL_Renderer *renderer;
bool isDone = false;
map<string, vsgl2_image> images;
map<string, TTF_Font*> fonts;
map<string, Mix_Chunk*> sounds;
map<int, vsgl2_animation> sprites;
const Uint8* currentKeyStates;
int mouseX, mouseY, mouseWheelX, mouseWheelY;
Mix_Music *music;
Color background_color;
unsigned int id_sprite_generator = 0;
//Variables for setting the drawing style of
//the geometrical primitives (draw_line etc.)
//in order to obtain persistence, e.g. each
//figure remains on the screen without
//having to redraw it at each cycle of the main loop
bool pixel_mode = false;
SDL_Texture *canvas;

namespace general
{
void init()
{
    SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO);
    //Set the default background color to white
    background_color = Color(255,255,255,255);
    SDL_Log("SDL init OK!");
    if (!(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) & (IMG_INIT_JPG | IMG_INIT_PNG)))
        SDL_Log("Image subsystem inizialitation error.");
    else
        SDL_Log("SDL image OK!");
    if (TTF_Init() == -1)
        SDL_Log("TTF subsystem inizialitation error.");
    else
        SDL_Log("SDL TTF OK!");
    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
        SDL_Log("SDL_mixer subsystem inizialitation error. SDL_mixer Error: %s\n",
                Mix_GetError() );
    else
        SDL_Log("SDL_mixer OK!");
    Mix_AllocateChannels(NUMBER_OF_CHANNELS);
    SDL_Log("VSGL2 version: %s Build %ld",
            AutoVersion::FULLVERSION_STRING,
            AutoVersion::BUILDS_COUNT);
    SDL_version compiled;
    SDL_version linked;
    SDL_VERSION(&compiled);
    SDL_GetVersion(&linked);
    SDL_Log("Built upon SDL2 version:\ncompiled %d.%d.%d\nlinked %d.%d.%d\n",
             compiled.major, compiled.minor, compiled.patch,
             linked.major, linked.minor, linked.patch);
}

void set_pixel_mode()
{
    pixel_mode = true;
}
void unset_pixel_mode()
{
    pixel_mode = false;
}

void close()
{
    SDL_Log("Quitting SDL...");
    //Free loaded image
    for (auto const& i: images)
        SDL_DestroyTexture( i.second.texture);
    //Free loaded sounds
    for (auto const& i: sounds)
        Mix_FreeChunk(i.second);
    //Free loaded fonts
    for (auto const& i: fonts)
        TTF_CloseFont(i.second);

    //Destroy window
    SDL_DestroyRenderer( renderer );
    SDL_DestroyWindow( window );
    //Free resources
    if (music != NULL)
        Mix_FreeMusic( music );

    //Quit various subsystems
    IMG_Quit();
    Mix_Quit();
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
    SDL_SetRenderDrawColor(renderer, background_color.c.r,
                           background_color.c.g,
                           background_color.c.b,
                           background_color.c.a);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);
    if (pixel_mode){
            canvas = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_ARGB8888,
                                       SDL_TEXTUREACCESS_TARGET, width, height);
            SDL_SetTextureBlendMode( canvas, SDL_BLENDMODE_BLEND );
            SDL_SetTextureAlphaMod( canvas, 127);
        }
}

void set_background_color(const Color& bg)
{
    background_color = bg;
    SDL_SetRenderDrawColor(renderer, background_color.c.r,
                           background_color.c.g,
                           background_color.c.b,
                           background_color.c.a);
   SDL_RenderClear(renderer);
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
    if (!pixel_mode) SDL_RenderClear(renderer);
    return isDone;
}

void undone()
{
    isDone = false;
}

void update()
{
    SDL_Event e;
    //Needed to avoid that the wheel value
    //would stay to -1 or 1 after scrolling
    mouseWheelX = 0;
    mouseWheelY = 0;
    while ( SDL_PollEvent(&e) )
    {
        if (e.type == SDL_QUIT)
            isDone = true;
        else if (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_ESCAPE)
            isDone = true;
        if( e.type == SDL_MOUSEMOTION ||
                e.type == SDL_MOUSEBUTTONDOWN ||
                e.type == SDL_MOUSEBUTTONUP )
            SDL_GetMouseState( &mouseX, &mouseY );
        if( e.type == SDL_MOUSEWHEEL)
            {
                mouseWheelX = e.wheel.x;
                mouseWheelY = e.wheel.y;
            };
    }
    for(auto i = sprites.begin(); i != sprites.end();)
    {
        if (!i->second.loop && i->second.times == 0)
            i = sprites.erase(i);
        else
        {
            sprite::draw_animation(i->second);
            i++;
        }

    }
    SDL_RenderPresent(renderer);
}


void draw_point(int x, int y, const Color& c)
{
    Uint8 r, g, b, a;
    SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
    SDL_SetRenderDrawColor(renderer, c.c.r, c.c.g, c.c.b, c.c.a);
    if (pixel_mode){
        SDL_SetRenderTarget(renderer, canvas);
    }
    SDL_RenderDrawPoint(renderer, x, y);
    if (pixel_mode){
        SDL_Rect r;
        r.x= 0;
        r.y = 0;
        r.w = width;
        r.h = height;
        SDL_SetRenderTarget(renderer, NULL);
        SDL_RenderCopy(renderer, canvas, NULL, &r);
    }
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
    if (pixel_mode){
        SDL_SetRenderTarget(renderer, canvas);
    }
    SDL_RenderDrawRect(renderer, &rectangle);
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    if (pixel_mode){
        SDL_Rect r;
        r.x= 0;
        r.y = 0;
        r.w = width;
        r.h = height;
        SDL_SetRenderTarget(renderer, NULL);
        SDL_RenderCopy(renderer, canvas, NULL, &r);
    }
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
    if (pixel_mode){
        SDL_SetRenderTarget(renderer, canvas);
    }
    SDL_RenderFillRect(renderer, &rectangle);
    if (pixel_mode){
        SDL_Rect r;
        r.x= 0;
        r.y = 0;
        r.w = width;
        r.h = height;
        SDL_SetRenderTarget(renderer, NULL);
        SDL_RenderCopy(renderer, canvas, NULL, &r);
    }
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
    SDL_SetTextureBlendMode( images[image].texture, SDL_BLENDMODE_BLEND );
    SDL_SetTextureAlphaMod( images[image].texture, alpha);
    SDL_RenderCopy(renderer, images[image].texture,NULL,&r);
}

}//closing namespace video

namespace sprite
{
    void draw_animation(vsgl2_animation &animation)
    {
        if (ms_time() - animation.elapsed_time > animation.speed)
        {
            //Change animation
            animation.current = (animation.current + 1)%(animation.end - animation.begin);
            if (!animation.loop && animation.current == 0)
                animation.times--;
            animation.elapsed_time = ms_time();
        }
        if (animation.loop || animation.times != 0)
        {
            float dim_w = ((float)animation.sprite.w)/animation.sprite.columns - 1;
            float dim_h = ((float)animation.sprite.h)/animation.sprite.rows + 2;
            //cout << dim_w << " " << dim_h << endl;
            //cout << (animation.current%animation.sprite.columns) << endl;
            //cout << (animation.current/animation.sprite.columns) << endl;
            SDL_Rect clip;
            clip.x= dim_w * ((animation.begin + animation.current)%animation.sprite.columns);
            clip.y = dim_h * ((animation.begin + animation.current)/animation.sprite.columns);
            clip.w = dim_w;
            clip.h = dim_h;
            SDL_Rect sheet;
            sheet.x= animation.x;
            sheet.y = animation.y;
            sheet.w = dim_w;//animation.sprite.w;
            sheet.h = dim_h;//animation.sprite.h;

            SDL_SetTextureBlendMode( animation.sprite.texture, SDL_BLENDMODE_BLEND );
            SDL_SetTextureAlphaMod( animation.sprite.texture, 255);
            SDL_RenderCopyEx(renderer, animation.sprite.texture,&clip,&sheet, 0, NULL, (SDL_RendererFlip)(animation.h_flip | animation.v_flip));
        }
    }

    int start_animation(string image, int x, int y, int begin, int end, int speed, int times)
    {
        string information = image.substr(0, image.find(".")) + ".txt";
        ifstream in(information);
        if (!in)
        {
            SDL_Log("Manca il file delle informazioni sulla sprite: %s", information.c_str());
            return -1;
        }
        if (images.find(image) == images.end())
        {
            SDL_Surface* s = IMG_Load(image.c_str());
            if (s == NULL)
                SDL_Log("Caricamento sprite fallito: %s", SDL_GetError() );
            else
            {
                SDL_Texture *texture = SDL_CreateTextureFromSurface( renderer, s);
                vsgl2_image im;
                im.texture = texture;
                im.alpha = 255;
                images.insert(make_pair(image,im));
                SDL_FreeSurface(s);
            }
        }
        vsgl2_sprite sp;
        sp.texture = images[image].texture;
        in >> sp.rows  >> sp.columns >> sp.w >> sp.h;
        vsgl2_animation animation;
        animation.sprite = sp;
        animation.begin = begin;
        animation.end = end;
        animation.x = x;
        animation.y = y;
        if (times == 0)
            animation.loop = true;
        else
            animation.loop = false;
        animation.times = times;
        animation.speed = speed;
        animation.current = 0;
        animation.elapsed_time = ms_time();
        animation.h_flip = SDL_FLIP_NONE;
        animation.v_flip = SDL_FLIP_NONE;
        id_sprite_generator++;
        sprites[id_sprite_generator] = animation;
        return id_sprite_generator;
    }

    void move_animation(int id, int dx, int dy)
    {
        if (sprites.find(id) != sprites.end())
        {
            sprites[id].x += dx;
            sprites[id].y += dy;
        }
    }

    void remove_animation(int id)
    {
        if (sprites.find(id) != sprites.end())
            sprites.erase(sprites.find(id));
    }

    void toggle_flip_h_animation(int id)
    {
        if (sprites.find(id) != sprites.end())
        {
            if (sprites[id].h_flip == SDL_FLIP_NONE)
                sprites[id].h_flip = SDL_FLIP_HORIZONTAL;
            else
                sprites[id].h_flip = SDL_FLIP_NONE;
        }
    }

    void flip_h_animation(int id)
    {
        if (sprites.find(id) != sprites.end())
           sprites[id].h_flip = SDL_FLIP_HORIZONTAL;
    }

    void noflip_h_animation(int id)
    {
        if (sprites.find(id) != sprites.end())
            sprites[id].h_flip = SDL_FLIP_NONE;
    }

    void toggle_flip_v_animation(int id)
    {
        if (sprites.find(id) != sprites.end())
        {
            if (sprites[id].v_flip == SDL_FLIP_NONE)
                sprites[id].v_flip = SDL_FLIP_VERTICAL;
            else
                sprites[id].v_flip = SDL_FLIP_NONE;
        }
    }

    void flip_v_animation(int id)
    {
        if (sprites.find(id) != sprites.end())
           sprites[id].v_flip = SDL_FLIP_VERTICAL;
    }

    void noflip_v_animation(int id)
    {
        if (sprites.find(id) != sprites.end())
            sprites[id].v_flip = SDL_FLIP_NONE;
    }
}

namespace audio
{
    void play_music(string file)
    {
        if (Mix_PausedMusic() == 1)
        {
            Mix_ResumeMusic();
            return;
        }
        if (music != NULL)
            Mix_FreeMusic(music);
        music = Mix_LoadMUS(file.c_str());
        if( music == NULL )
            SDL_Log( "Failed to load music! SDL_mixer Error: %s\n",
                    Mix_GetError() );
        else
            Mix_PlayMusic(music, -1);
    }

    void pause_music()
    {
        if( Mix_PlayingMusic() == 0 )
            return;
        Mix_PauseMusic();
    }

    void stop_music()
    {
        Mix_PlayMusic(music, -1);
        Mix_HaltMusic();
    }

    void play_sound(string sound)
    {
        if (sounds.find(sound)==sounds.end())
        {
            Mix_Chunk* s = Mix_LoadWAV(sound.c_str());
            if( s == NULL )
                SDL_Log("Failed to load sound effect! SDL_mixer Error: %s\n", Mix_GetError());
            else
                sounds.insert(make_pair(sound,s));
        }
        Mix_PlayChannel( -1, sounds[sound], 0 );
    }
}//closing namespace audio

namespace io
{
bool is_pressed(int key)
{
    currentKeyStates = SDL_GetKeyboardState( NULL );
    return (bool)currentKeyStates[key];
}

string read_text(string font, int dim, int x, int y, Color c, uint8_t max_length)
{
    std::string inputText;
    SDL_Event e;
    bool done = false;
    SDL_Surface *screen = SDL_GetWindowSurface(window);
    SDL_Rect r;
    r.x= 0;
    r.y = 0;
    r.w = width;
    r.h = height;
    SDL_Texture *texture =
    SDL_CreateTextureFromSurface(renderer, screen);
    SDL_FreeSurface(screen);
    while( !done )
    {
        delay(1); //to avoid to hung the CPU
        bool renderText = false;

        //Handle events on queue
        while( SDL_PollEvent( &e ) != 0 )
        {
            //User requests quit
            if( e.type == SDL_QUIT )
            {
                close();
                exit(0);
            }
            else if( e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN )
            {
                done = true;
            }
            else if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_BACKSPACE )
            {
                if (inputText.size()>0)
                    inputText.pop_back();
                renderText = true;
            }
            else if(  e.type == SDL_TEXTINPUT && (inputText.size() < max_length || max_length == 0) )
            {
                inputText += e.text.text;
                renderText = true;
            }
        }
        if( renderText )
        {
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer,texture,NULL,&r);
            if (inputText.size() > 0)
                draw_text(font,dim,inputText.c_str(),  x,  y,  c);
            update();
        }
    }
    SDL_DestroyTexture(texture);
    return inputText;
}

int get_mouse_x()
{
    return mouseX;
}

int get_mouse_y()
{
    return mouseY;
}

int get_mouse_wheel_x()
{
    return mouseWheelX;
}

int get_mouse_wheel_y()
{
    return mouseWheelY;
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

namespace ttf_fonts
{

string load_font(string font, int dim)
{
    ostringstream font_identifier;
    font_identifier << font << dim;
    //The string made by the font name and the dimension is used as
    //an identifier to load the font in memory in order  to have only one
    //version in memory shared between different strings
    if (fonts.find(font_identifier.str())== fonts.end())
    {
        TTF_Font* gFont = TTF_OpenFont( font.c_str(), dim );
        if( gFont == NULL )
        {
            SDL_Log("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError() );
            SDL_Log(font.c_str());
            return "";
        }
        fonts.insert(make_pair(font_identifier.str(),gFont));
    }
    return font_identifier.str();
}

void draw_text(string font, int dim, string text, int x, int y, Color c)
{
    string font_identifier = load_font(font, dim);
    if (font_identifier == "") return;
    SDL_Surface* textSurface = TTF_RenderUTF8_Solid( fonts[font_identifier],
                               text.c_str(), c.c);
    if( textSurface == NULL )
    {
        SDL_Log("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
    }
    else
    {
        SDL_Texture* texture = SDL_CreateTextureFromSurface( renderer,
                   textSurface );
        if( texture == NULL )
        {
            SDL_Log("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
        }
        else
        {
            SDL_Rect r;
            r.x= x;
            r.y = y;
            r.w = textSurface->w;
            r.h = textSurface->h;
            SDL_SetTextureAlphaMod( texture, c.c.a);
            SDL_RenderCopy(renderer, texture,NULL,&r);
            SDL_FreeSurface( textSurface );
            SDL_DestroyTexture(texture);
        }
    }
}

int text_width(string font, int dim, string text)
{
    string font_identifier = load_font(font, dim);
    if (font_identifier == "") return -1;
    SDL_Surface* textSurface = TTF_RenderText_Solid( fonts[font_identifier],
                               text.c_str(), Color(0,0,0,0).c);
    if( textSurface == NULL )
    {
        SDL_Log("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
        return -1;
    }
    int w = textSurface->w;
    SDL_FreeSurface( textSurface );
    return w;
}

int text_height(string font, int dim, string text)
{
    string font_identifier = load_font(font, dim);
    if (font_identifier == "") return -1;
    SDL_Surface* textSurface = TTF_RenderText_Solid( fonts[font_identifier],
                               text.c_str(), Color(0,0,0,0).c);
    if( textSurface == NULL )
    {
        SDL_Log("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
        return -1;
    }
    int h = textSurface->h;
    SDL_FreeSurface( textSurface );
    return h;
}

} //END NAMESPACE ttf_fonts

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

unsigned int ms_time()
{
    return SDL_GetTicks();
}

int take_screenshot(string filename)
{
    SDL_RWops* file = SDL_RWFromFile(filename.c_str(),"w");
    int saved = SDL_SaveBMP_RW(SDL_GetWindowSurface(window), file, 1);
    SDL_RWclose(file);
    return saved;
}

void hide_mouse_cursor()
{
    SDL_ShowCursor(SDL_DISABLE);
}

void show_mouse_cursor()
{
    SDL_ShowCursor(SDL_ENABLE);
}


}//closing namespace utils

}
