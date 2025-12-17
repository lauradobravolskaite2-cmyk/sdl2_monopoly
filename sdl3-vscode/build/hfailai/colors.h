#ifndef COLORS_H
#define COLORS_H

#include <SDL.h>

extern SDL_Color *P_bought;
extern SDL_Color *P_house;
extern SDL_Color *P_hotel;

extern SDL_Color white;
extern SDL_Color middle;

extern SDL_Color assembly;
extern SDL_Color html;
extern SDL_Color python;
extern SDL_Color fortran;
extern SDL_Color java;
extern SDL_Color ada;
extern SDL_Color cpp;
extern SDL_Color c;

extern SDL_Color rand_color;
extern SDL_Color system_logs;

void player_colors(int kiekplayer);
void free_player_colors(void);

#endif