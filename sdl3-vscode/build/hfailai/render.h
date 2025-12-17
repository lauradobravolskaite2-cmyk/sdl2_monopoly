/*#ifndef RENDER_H
#define RENDER_H

#include <SDL.h>
#include <SDL_ttf.h>
#include "lenta.h"
#include "zaidejai.h"

int render_init(const char *fontpath, int fontsize);
void render_quit(void);

void render_board(SDL_Renderer *renderer, cell cells[], player players[], int kiekplayer,
                  SDL_Rect centerRect, SDL_Rect sidebarRect);
void render_center_cards(SDL_Renderer *renderer, SDL_Rect centerRect);

void addorupdatem(int id, const char *text, SDL_Color color);
void clear_m(void);
void drawinfo(SDL_Renderer *renderer, SDL_Rect sidebarRect);

void show_start_screen(void);
void show_ask_player_count(void);
void show_game_instructions(void);
void show_press_enter_prompt(void);
void show_player_turn(int idx, int id, int bytes, int uf, int pr);
void show_roll_result(int value);
void show_cell_bought(int bytes_after);
void show_cell_already_bought(void);
void show_wrong_cell_press(void);
void show_card_prompt(void);
void show_wrong_deck_press(void);
void show_card_drawn(const char *card_text);
void show_code_ai_penalty(int penalty_bytes, int bytes_after);
void show_memory_bonus(int bonus_bytes, int bytes_after);
void show_error_cell_offer(int repair_cost);
void show_roll_again_message(int rolled);
void show_buy_house_prompt(int cost);
void show_house_bought(int cost, int money);
void show_hotel_bought(void);
void show_house_limit_error(void);
void show_bankruptcy_message(int player_id);
void show_game_over_message(int winner_id);
void show_current_player_info(int player_id, int position, int money, int jail);
void show_memory_error_basic(void);
void show_memory_error_save_fail(void);
void show_file_read_error(void);

#endif*/

#ifndef RENDER_H
#define RENDER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "lenta.h"
#include "zaidejai.h"

// render modulio init/quit
int  render_init(const char *fontpath, int fontsize);
void render_quit(void);

// pagrindinis renderis
void render_board(SDL_Renderer *renderer, cell cells[], player players[], int kiekplayer,
                  SDL_Rect centerRect, SDL_Rect sidebarRect);

// message log (sidebar)
void clear_m(void);

// universalus pranesimas (kad nereiktu addorupdatem is kitu failu)
void show_debug_message(int id, const char *text, SDL_Color color);

// UI messages (tavo esamos)
void show_start_screen(void);
void show_ask_player_count(void);
void show_game_instructions(void);
void show_press_enter_prompt(void);
void show_player_turn(int idx, int id, int bytes, int uf, int pr);
void show_roll_result(int v);
void show_cell_bought(int a);
void show_cell_already_bought(void);
void show_wrong_cell_press(void);
void show_card_prompt(void);
void show_wrong_deck_press(void);
void show_card_drawn(const char *t);
void show_code_ai_penalty(int p, int after);
void show_memory_bonus(int bonus, int after);
void show_error_cell_offer(int c);
void show_roll_again_message(int r);
void show_buy_house_prompt(int cost);
void show_house_bought(int cost, int current_money);
void show_hotel_bought(void);
void show_house_limit_error(void);
void show_bankruptcy_message(int id);
void show_game_over_message(int id);
void show_current_player_info(int id, int pos, int money, int jail);
void show_memory_error_basic(void);
void show_memory_error_save_fail(void);
void show_file_read_error(void);

// papildomos (kad tavo zaidimo_ciklas.c būtų švarus)
void show_game_saved(void);

#endif