#ifndef SCREEN_H
#define SCREEN_H

#include <iostream>
#include <cassert>
#include <SDL.h>

#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32
#define ZOOM_SCALE 10

class Screen{

    SDL_Window *screen;
    SDL_Surface *surface;
    SDL_Renderer *renderer;
    bool **screenBuffer;
    
    public:
        Screen(void);
        
        void drawPixel(int,int);
        bool drawSprite(int,int,short);
        void clearScreen(void);
        ~Screen(void);

    private:
        void UpdateScreen(void);
        void DecToBin(short,bool*);

};

#endif