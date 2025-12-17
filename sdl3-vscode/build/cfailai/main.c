#include <stdio.h>
#include "render.h"
#include "zaidejai.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

static void render_setup_screen(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Rect dummyRect = {10, 10, 620, 460};  // CHANGED: SDL_Rect, ne SDL_FRect
    drawinfo(renderer, dummyRect);            // CHANGED: SDL_Rect

    SDL_RenderPresent(renderer);
}

int main(int argc, char *argv[])
{
    (void)argc; (void)argv;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL Init Error: %s\n", SDL_GetError());
        return 1;
    }

    if (render_init(NULL, 24) != 0) {          // CHANGED: default font search
        fprintf(stderr, "Font Init Error.\n");
        SDL_Quit();
        return 1;
    }

    // SDL2: SDL_CreateWindow turi daugiau parametrų
    SDL_Window *setupWindow = SDL_CreateWindow("Game Setup", 
                                              SDL_WINDOWPOS_CENTERED,
                                              SDL_WINDOWPOS_CENTERED,
                                              640, 480, 0); // CHANGED
    if (!setupWindow) { render_quit(); SDL_Quit(); return 1; }

    SDL_Renderer *setupRenderer = SDL_CreateRenderer(setupWindow, -1, 
                                                    SDL_RENDERER_ACCELERATED); // CHANGED
    if (!setupRenderer) { 
        SDL_DestroyWindow(setupWindow); 
        render_quit(); 
        SDL_Quit(); 
        return 1; 
    }

    int ats = 0; // 1 load, 2 new
    int sk = 0;  // player count
    SDL_Event e;

    show_start_screen();

    while (ats == 0) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) { ats = -1; break; }  // CHANGED: SDL_QUIT
            if (e.type == SDL_KEYDOWN) {                  // CHANGED: SDL_KEYDOWN
                if (e.key.keysym.sym == SDLK_1) ats = 1;  // CHANGED: e.key.keysym.sym
                if (e.key.keysym.sym == SDLK_2) ats = 2;
            }
        }
        if (ats == -1) break;
        render_setup_screen(setupRenderer);
        SDL_Delay(16);
    }

    if (ats == 2) {
        show_ask_player_count();
        int selected = 0;
        while (selected == 0) {
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) { selected = -1; break; }  // CHANGED
                if (e.type == SDL_KEYDOWN) {                      // CHANGED
                    switch (e.key.keysym.sym) {                   // CHANGED
                        case SDLK_2: selected = 2; break;
                        case SDLK_3: selected = 3; break;
                        case SDLK_4: selected = 4; break;
                        case SDLK_5: selected = 5; break;
                        case SDLK_6: selected = 6; break;
                        default: break;
                    }
                }
            }
            if (selected == -1) break;
            render_setup_screen(setupRenderer);
            SDL_Delay(16);
        }
        sk = selected;
    }

    SDL_DestroyRenderer(setupRenderer);
    SDL_DestroyWindow(setupWindow);

    if (ats != -1 && sk != -1)
        zaideju_duomenys(ats, sk);

    render_quit();
    SDL_Quit();
    return 0;
}