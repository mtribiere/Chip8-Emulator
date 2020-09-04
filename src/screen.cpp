#include "screen.h"

Screen::Screen(void){

    //Initilaliser la fenetre SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0){
	    std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
    }

    screen = SDL_CreateWindow("Chip8 Emulator",100,100,SCREEN_WIDTH * ZOOM_SCALE,SCREEN_HEIGHT * ZOOM_SCALE,SDL_WINDOW_SHOWN);
    if(screen == nullptr){
        std::cout << "Error while creating the display" << std::endl;
        SDL_Quit(); 
    }

    renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_ACCELERATED);
    SDL_RenderClear(renderer);

    //Creer le screen buffer
    screenBuffer = new bool*[SCREEN_WIDTH];
    for(int i = 0;i<SCREEN_WIDTH;i++)
        screenBuffer[i] = new bool[SCREEN_HEIGHT];
    
    clearScreen();

}

void Screen::clearScreen(){

    //Effacer tout les pixels
    for(int i = 0;i<SCREEN_WIDTH;i++){
        for(int j = 0;j<SCREEN_HEIGHT;j++){
            screenBuffer[i][j] = false;
        }
    }

    //Mettre a jour l'affichage
    UpdateScreen();
}

void Screen::drawPixel(int x,int y){

    screenBuffer[x][y] = true;
    UpdateScreen();

}

bool Screen::drawSprite(int x,int y,short sprite){
    bool overridPixel = false;

    int mask = 0x80;

    //assert(x<SCREEN_WIDTH);

    //Pour chacun des 8 pixels
    for(int i = 0;i<8;i++){
        
        //Determiner s'il y a un pixel a dessiner
        bool asPixel = (sprite & mask);
        mask = mask>>1;

        //S'il y un overrride
        if(screenBuffer[(x+i)%SCREEN_WIDTH][y%SCREEN_HEIGHT] && asPixel)
            overridPixel = true;

        //Calculer le pixel
        screenBuffer[(x+i)%SCREEN_WIDTH][y%SCREEN_HEIGHT] = screenBuffer[(x+i)%SCREEN_WIDTH][y%SCREEN_HEIGHT] ^ asPixel;
        
    }

    //Mettre a jour l'affichage
    UpdateScreen();

    return overridPixel;

}

void Screen::UpdateScreen(void){

    //Effacer tout les pixels a l'écran
    SDL_RenderClear(renderer);

    //Pour tout les emplacement du screen buffer
    for(int i = 0;i<SCREEN_WIDTH;i++){
        for(int j = 0;j<SCREEN_HEIGHT;j++){

            //Si le pixel doit être dessiné
            if(screenBuffer[i][j]){

                SDL_Rect rect;
                rect.x = i * ZOOM_SCALE;
                rect.y = j * ZOOM_SCALE;
                rect.w = ZOOM_SCALE;
                rect.h = ZOOM_SCALE;

                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderFillRect(renderer, &rect);

            }

        }
    }

    //Definir le fond
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    //Mettre a jour l'écran
    SDL_RenderPresent(renderer);

}

//Convertion en binaire
void Screen::DecToBin(short num,bool* bin){

    for(int i=0; num>0; i++)    
    {    
        bin[i]= (num%2 == 1);    
        num = num/2;  
    }   

}

Screen::~Screen(void){

    delete[] screenBuffer;

    SDL_Quit();
}