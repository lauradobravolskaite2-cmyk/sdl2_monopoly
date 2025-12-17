#include "colors.h"
#include <SDL.h>
#include <stdlib.h>

SDL_Color *P_bought;
SDL_Color *P_house;
SDL_Color *P_hotel;

SDL_Color white = {255, 255, 255, 255};
SDL_Color middle = {244, 208, 202, 255};

SDL_Color assembly = {176, 157, 192, 255};
SDL_Color html = {213, 156, 162, 255};
SDL_Color python = {168, 168, 168, 255};
SDL_Color fortran = {241, 179, 206, 255};
SDL_Color java = {182, 191, 220, 255};
SDL_Color ada = {241, 224, 179, 255};
SDL_Color cpp = {232, 188, 166, 255};
SDL_Color c = {189, 155, 135, 255};

SDL_Color rand_color = {188, 138, 151, 255};
SDL_Color system_logs = {139, 108, 96, 255};

void player_colors(int kiekplayer)
{
    P_bought = (SDL_Color*)malloc(sizeof(SDL_Color) * kiekplayer);
    P_house = (SDL_Color*)malloc(sizeof(SDL_Color) * kiekplayer);
    P_hotel = (SDL_Color*)malloc(sizeof(SDL_Color) * kiekplayer);
    
    SDL_Color colors[] = {
        {255, 0, 0, 255},    // Red
        {0, 0, 255, 255},    // Blue
        {0, 255, 0, 255},    // Green
        {255, 255, 0, 255},  // Yellow
        {255, 0, 255, 255},  // Magenta
        {0, 255, 255, 255}   // Cyan
    };
    
    for (int i = 0; i < kiekplayer; i++) {
        P_bought[i] = colors[i % 6];
        P_house[i] = colors[i % 6];
        P_hotel[i] = colors[i % 6];
    }
}

void free_player_colors(void)
{
    free(P_bought);
    free(P_house);
    free(P_hotel);
    P_bought = NULL;
    P_house = NULL;
    P_hotel = NULL;
}