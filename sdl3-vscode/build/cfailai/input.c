#include "input.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>


int get_user_input(void)
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        switch (e.type)
        {
        case SDL_QUIT:
            return INPUT_EXIT;

        case SDL_KEYDOWN:
            switch (e.key.keysym.sym)
            {
            case SDLK_m:
                return INPUT_ROLL;        // CHANGED
            case SDLK_RETURN:
                return INPUT_NEXT_PLAYER;
            case SDLK_n:
                return INPUT_BUY_HOUSE;   // CHANGED
            case SDLK_s:
                return INPUT_REPAIR;      // CHANGED
            case SDLK_r:
                return INPUT_SKIP_TURN;   // CHANGED
            case SDLK_p:
                return INPUT_PAUSE;       // CHANGED
            case SDLK_e:
                return INPUT_EXIT;        // CHANGED
            default:
                break;
            }
            break;

        case SDL_MOUSEBUTTONDOWN:
            if (e.button.button == SDL_BUTTON_LEFT)
                return INPUT_MOUSE_CLICK;
            break;
        }
    }
    return -1;
}

