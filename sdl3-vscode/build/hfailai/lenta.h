/*#ifndef LENTA_H
#define LENTA_H
 
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "zaidejai.h"
 
#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 800
#define BOARD_SIZE 700
#define CORNER_SIZE 100
#define CELL_SIZE 50
#define CELL_COUNT 10
#define SIDEBAR_WIDTH 300
 
typedef struct {
    SDL_FRect rect;
    SDL_Color tcolor;
    SDL_Color bcolor;
    char ttext[64];
    char btext[128];
 
    int owner;
 
    // Kad sutaptų su zaidimo_ciklas.c:
    int price;        // buvo kaina
    int house_price;  // namo kaina
    int houses;       // kiek namų ant langelio
    int type;         // 0 = property, kiti pagal poreikį
} cell;
 
typedef struct {
    SDL_FRect centerRect;
    SDL_FRect sidebarRect;
} board_layout_t;
 
void buildCells(cell cells[]);
board_layout_t compute_board_layout(void);
int detect_clicked_property(int mx, int my, cell cells[], int n);
int handle_center_card_click(int mx, int my, SDL_FRect centerRect);
 
int is_buildable_cell(int pos);
int get_house_cost(int pos);
 
#endif*/

#ifndef LENTA_H
#define LENTA_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "zaidejai.h"

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 800
#define BOARD_SIZE 700
#define CORNER_SIZE 100
#define CELL_SIZE 50
#define CELL_COUNT 10
#define SIDEBAR_WIDTH 300

typedef struct {
    SDL_FRect rect;
    SDL_Color tcolor;
    SDL_Color bcolor;
    char ttext[64];
    char btext[128];

    int owner;

    // Suderinamumas su skirtingais failais:
    int kaina;       // sena (lietuviškai)
    int price;       // nauja (zaidimo_ciklas.c)
    int house_price; // namo kaina
    int houses;      // namų sk.
    int type;        // 0=property (pas tave taip tikrinama)
} cell;

typedef struct {
    SDL_FRect centerRect;
    SDL_FRect sidebarRect;
} board_layout_t;

void buildCells(cell cells[]);
board_layout_t compute_board_layout(void);
int detect_clicked_property(int mx, int my, cell cells[], int n);
int handle_center_card_click(int mx, int my, SDL_FRect centerRect);

int is_buildable_cell(int pos);
int get_house_cost(int pos);

#endif