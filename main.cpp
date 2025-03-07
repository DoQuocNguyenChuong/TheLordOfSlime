#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "graphics.h"
#include "defs.h"

using namespace std;

int main(int argc, char *argv[])
{
    Graphics graphics;
    graphics.init();

    Sprite slime;
    SDL_Texture* slimeTexture = graphics.loadTexture(SLIME_SPRITE_FILE);
    slime.init(slimeTexture, SLIME_FRAMES, SLIME_CLIPS);

    bool quit = false;
    SDL_Event e;
    while( !quit ) {
        while( SDL_PollEvent( &e ) != 0 ) {
            if( e.type == SDL_QUIT ) quit = true;
        }
        slime.tick();

        graphics.prepareScene();
        graphics.render(150, 100, slime);
        graphics.presentScene();
        SDL_Delay(100);
    }

	SDL_DestroyTexture( slimeTexture ); slimeTexture = nullptr;

    graphics.quit();
    return 0;
}
