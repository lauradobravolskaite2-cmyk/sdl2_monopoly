/*#include "render.h"
#include "colors.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL_ttf.h>


#define maxm 40
#define maxmilg 256

typedef struct {
    int id;
    char text[maxmilg];
    SDL_Color color;
} info_t;

static info_t m[maxm];
static int kiekm = 0;
static TTF_Font *fontptr = NULL;

int render_init(const char *fontpath, int fontsize)
{
    if (TTF_Init() != 0) {
        printf("TTF_Init Error: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    if (fontpath) {
        fontptr = TTF_OpenFont(fontpath, fontsize);
    } else {

        const char *fonts[] = {
            "/System/Library/Fonts/Helvetica.ttc",
            NULL
        };
        for (int i = 0; fonts[i]; i++) {
            fontptr = TTF_OpenFont(fonts[i], fontsize);
            if (fontptr) break;
        }
    }

    if (!fontptr) {
        printf("TTF_OpenFont Error: %s\n", TTF_GetError());
        return 2;
    }

    return 0;
}

void render_quit(void)
{
    if (fontptr) {
        TTF_CloseFont(fontptr);
        fontptr = NULL;
    }
    TTF_Quit();
}


void addorupdatem(int id, const char *text, SDL_Color color)
{
    for (int i = 0; i < kiekm; i++) {
        if (m[i].id == id) {
            strncpy(m[i].text, text, maxmilg-1);
            m[i].text[maxmilg-1] = '\0';
            m[i].color = color;
            return;
        }
    }
    if (kiekm < maxm) {
        m[kiekm].id = id;
        strncpy(m[kiekm].text, text, maxmilg-1);
        m[kiekm].text[maxmilg-1] = '\0';
        m[kiekm].color = color;
        kiekm++;
        return;
    }
    for (int i = 1; i < maxm; i++) m[i-1] = m[i];
    m[maxm-1].id = id;
    strncpy(m[maxm-1].text, text, maxmilg-1);
    m[maxm-1].text[maxmilg-1] = '\0';
    m[maxm-1].color = color;
}

void clear_m(void) { kiekm = 0; }

static void draw_text(SDL_Renderer *r, const char *txt, int x, int y, SDL_Color c)
{
    if (!txt || !*txt || !fontptr) return;
    
    SDL_Surface *s = TTF_RenderText_Solid(fontptr, txt, c);
    if (!s) return;
    
    SDL_Texture *t = SDL_CreateTextureFromSurface(r, s);
    if (!t) { SDL_FreeSurface(s); return; }
    
    SDL_Rect dst = {x, y, s->w, s->h};
    SDL_RenderCopy(r, t, NULL, &dst);
    SDL_DestroyTexture(t);
    SDL_FreeSurface(s);
}

static void draw_wrapped(SDL_Renderer *r, const char *txt, int x, int y, int maxw, SDL_Color c)
{
    if (!txt || !*txt) return;
    char *copy = strdup(txt);
    if (!copy) return;

    char *line = strtok(copy, "\n");
    int yy = y;
    while (line) {
        draw_text(r, line, x, yy, c);
        yy += 18;
        line = strtok(NULL, "\n");
    }
    free(copy);
    (void)maxw;
}

void drawinfo(SDL_Renderer *renderer, SDL_Rect sidebarRect)
{
    int y = sidebarRect.y + 10;
    for (int i = 0; i < kiekm; i++) {
        draw_wrapped(renderer, m[i].text, sidebarRect.x + 10, y, sidebarRect.w - 20, m[i].color);
        y += 60;
        if (y > sidebarRect.y + sidebarRect.h - 60) break;
    }
}

    // Draw cell border
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &cell_rect);

    // Draw top text (ttext) - Ensure visible contrast
    if (c->ttext[0] != '\0') {
        // Determine text color based on background brightness
        SDL_Color text_color;
        int bg_brightness = c->tcolor.r + c->tcolor.g + c->tcolor.b;
        
        // If background is dark, use white text; otherwise use black
        if (bg_brightness < 128 * 3) {  // Dark background
            text_color = (SDL_Color){255, 255, 255, 255};  // White
        } else {
            text_color = (SDL_Color){0, 0, 0, 255};  // Black
        }
        
        // Position text in top section
        int text_x = cell_rect.x + 5;
        int text_y = cell_rect.y + 5;  // Small offset from top
        
        draw_text(renderer, c->ttext, text_x, text_y, text_color);
    }

    // Draw bottom text (btext) - Ensure visible contrast
    if (c->btext[0] != '\0') {
        // Determine text color based on background brightness
        SDL_Color text_color;
        int bg_brightness = c->bcolor.r + c->bcolor.g + c->bcolor.b;
        
        // Bottom background is white, so use black text
        text_color = (SDL_Color){0, 0, 0, 255};  // Black for white background
        
        // Position text in bottom section
        int text_x = cell_rect.x + 5;
        int text_y = cell_rect.y + top_height + 5;  // Small offset from top of bottom section
        
        // If btext has newlines, we need to handle them
        char *copy = strdup(c->btext);
        if (copy) {
            char *line = strtok(copy, "\n");
            int line_y = text_y;
            while (line && line_y < cell_rect.y + cell_rect.h - 10) {
                draw_text(renderer, line, text_x, line_y, text_color);
                line_y += 16;  // Line spacing
                line = strtok(NULL, "\n");
            }
            free(copy);
        } else {
            draw_text(renderer, c->btext, text_x, text_y, text_color);
        }
    }
}

static void draw_player_token(SDL_Renderer *renderer, cell *c, int player_index)
{
    if (!P_bought) return;
    
    // Convert SDL_FRect to SDL_Rect
    SDL_Rect cell_rect = {
        (int)c->rect.x,
        (int)c->rect.y,
        (int)c->rect.w,
        (int)c->rect.h
    };
    
    int token_size = 12;
    int col = player_index % 3;
    int row = player_index / 3;
    int x = cell_rect.x + 5 + col*(token_size+4);
    int y = cell_rect.y + cell_rect.h - token_size - 5 - row*(token_size+4);

    SDL_Color token = P_bought[player_index < 6 ? player_index : (player_index%6)];
    SDL_SetRenderDrawColor(renderer, token.r, token.g, token.b, 255);
    SDL_Rect tr = {x, y, token_size, token_size};
    SDL_RenderFillRect(renderer, &tr);
    SDL_SetRenderDrawColor(renderer, 0,0,0,255);
    SDL_RenderDrawRect(renderer, &tr);
}

void render_center_cards(SDL_Renderer *renderer, SDL_Rect centerRect)
{
    int rectW = centerRect.w / 3;
    int rectH = centerRect.h / 6;

    int cx1 = centerRect.x + centerRect.w / 3;
    int cy1 = centerRect.y + centerRect.h / 2;
    int cx2 = centerRect.x + 2 * centerRect.w / 3;
    int cy2 = centerRect.y + centerRect.h / 2;

    SDL_Rect r1 = {cx1 - rectW/2, cy1 - rectH/2, rectW, rectH};
    SDL_Rect r2 = {cx2 - rectW/2, cy2 - rectH/2, rectW, rectH};

    SDL_SetRenderDrawColor(renderer, rand_color.r, rand_color.g, rand_color.b, 255);
    SDL_RenderFillRect(renderer, &r1);
    SDL_SetRenderDrawColor(renderer, 0,0,0,255);
    SDL_RenderDrawRect(renderer, &r1);
    draw_text(renderer, "rand()", r1.x + 8, r1.y + 8, (SDL_Color){0,0,0,255});

    SDL_SetRenderDrawColor(renderer, system_logs.r, system_logs.g, system_logs.b, 255);
    SDL_RenderFillRect(renderer, &r2);
    SDL_SetRenderDrawColor(renderer, 0,0,0,255);
    SDL_RenderDrawRect(renderer, &r2);
    draw_text(renderer, "system logs", r2.x + 8, r2.y + 8, (SDL_Color){0,0,0,255});
}

void render_board(SDL_Renderer *renderer, cell cells[], player players[], int kiekplayer,
                  SDL_Rect centerRect, SDL_Rect sidebarRect)
{
    for (int i = 0; i < 44; i++) {
        render_cell(renderer, &cells[i], kiekplayer);
        for (int p = 0; p < kiekplayer; p++)
            if (players[p].poz_l == i)
                draw_player_token(renderer, &cells[i], p);
    }

    SDL_SetRenderDrawColor(renderer, middle.r, middle.g, middle.b, 255);
    SDL_RenderFillRect(renderer, &centerRect);
    render_center_cards(renderer, centerRect);

    SDL_SetRenderDrawColor(renderer, 40,40,40,255);
    SDL_RenderFillRect(renderer, &sidebarRect);
    SDL_SetRenderDrawColor(renderer, 100,100,100,255);
    SDL_RenderDrawRect(renderer, &sidebarRect);

    drawinfo(renderer, sidebarRect);
}

// --- UI messages ---
void show_start_screen(void){ addorupdatem(1,"1 - testi\n2 - naujas",(SDL_Color){255,255,255,255}); }
void show_ask_player_count(void){ addorupdatem(2,"Pasirinkite zaideju skaiciu 2-6",(SDL_Color){255,255,255,255}); }
void show_game_instructions(void){
    addorupdatem(3,"Valdymas:\nm - mesti\nEnter - baigti ejima\nn - pirkti nama\ns - remontas\nr - praleisti\np - instrukcijos\ne - iseiti\nPele - pirkti laukeli",(SDL_Color){255,255,255,255});
}
void show_press_enter_prompt(void){ addorupdatem(4,"Spausk Enter",(SDL_Color){200,200,200,255}); }
void show_player_turn(int idx, int id, int bytes, int uf, int pr){ (void)idx;(void)uf;(void)pr; char b[128]; snprintf(b,sizeof(b),"ZAIDEJAS %d\nBaitu: %d\nm - mesti",id,bytes); addorupdatem(10+idx,b,(SDL_Color){255,255,255,255}); }
void show_roll_result(int v){ char b[64]; snprintf(b,sizeof(b),"Isridenote: %d",v); addorupdatem(51,b,(SDL_Color){255,255,255,255}); }
void show_cell_bought(int a){ char b[80]; snprintf(b,sizeof(b),"Nupirkta! Liko: %d",a); addorupdatem(100,b,(SDL_Color){0,255,0,255}); }
void show_cell_already_bought(void){ addorupdatem(99,"Jau nupirkta!",(SDL_Color){255,0,0,255}); }
void show_wrong_cell_press(void){ addorupdatem(101,"Nestovite ant sito laukelio",(SDL_Color){255,0,0,255}); }
void show_card_prompt(void){ addorupdatem(200,"Spausk ant kortu kalades centre",(SDL_Color){255,255,255,255}); }
void show_wrong_deck_press(void){ addorupdatem(201,"Netinkama kalade",(SDL_Color){255,0,0,255}); }
void show_card_drawn(const char *t){ char b[256]; snprintf(b,sizeof(b),"Kortele:\n%s",t?t:""); addorupdatem(202,b,(SDL_Color){255,255,255,255}); }
void show_code_ai_penalty(int p, int after){ char b[128]; snprintf(b,sizeof(b),"Bauda: -%d\nLiko: %d",p,after); addorupdatem(300,b,(SDL_Color){255,255,255,255}); }
void show_memory_bonus(int bonus, int after){ char b[128]; snprintf(b,sizeof(b),"Bonus: +%d\nLiko: %d",bonus,after); addorupdatem(310,b,(SDL_Color){255,255,255,255}); }
void show_error_cell_offer(int c){ char b[128]; snprintf(b,sizeof(b),"ERROR!\ns - moketi %d\nr - praleisti",c); addorupdatem(500,b,(SDL_Color){255,255,255,255}); }
void show_roll_again_message(int r){ char b[80]; snprintf(b,sizeof(b),"Mesk dar karta! pries tai %d",r); addorupdatem(600,b,(SDL_Color){255,255,255,255}); }
void show_buy_house_prompt(int cost){ char b[128]; snprintf(b,sizeof(b),"Pirkti nama? kaina %d\nn - pirkti",cost); addorupdatem(700,b,(SDL_Color){255,255,0,255}); }
void show_house_bought(int cost, int money){ char b[128]; snprintf(b,sizeof(b),"Namas nupirktas -%d\nLiko %d",cost,money); addorupdatem(701,b,(SDL_Color){0,255,0,255}); }
void show_hotel_bought(void){ addorupdatem(702,"Turite 4 namus (projektas)!",(SDL_Color){0,255,0,255}); }
void show_house_limit_error(void){ addorupdatem(703,"Negalima 2 namu per ejima",(SDL_Color){255,0,0,255}); }
void show_bankruptcy_message(int id){ char b[64]; snprintf(b,sizeof(b),"Zaidejas %d BANKRUTAVO",id); addorupdatem(800,b,(SDL_Color){255,0,0,255}); }
void show_game_over_message(int id){ char b[64]; snprintf(b,sizeof(b),"LAIMEJO %d",id); addorupdatem(900,b,(SDL_Color){0,255,0,255}); }
void show_current_player_info(int id, int pos, int money, int jail){ char b[128]; snprintf(b,sizeof(b),"Zaidejas %d\nPoz %d\nBaitu %d\nJail %d",id,pos,money,jail); addorupdatem(950,b,(SDL_Color){255,255,255,255}); }
void show_memory_error_basic(void){ addorupdatem(90,"Klaida: nepakanka atminties",(SDL_Color){255,0,0,255}); }
void show_memory_error_save_fail(void){ addorupdatem(92,"Klaida: nepavyko issaugoti",(SDL_Color){255,0,0,255}); }
void show_file_read_error(void){ addorupdatem(93,"Klaida: nepavyko perskaityti",(SDL_Color){255,0,0,255}); }*/

#include "render.h"
#include "colors.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL_ttf.h>

#define maxm 40
#define maxmilg 256

typedef struct {
    int id;
    char text[maxmilg];
    SDL_Color color;
} info_t;

static info_t m[maxm];
static int kiekm = 0;

// du fontai: vienas sidebar'ui, vienas langeliams (mažesnis)
static TTF_Font *font_ui = NULL;
static TTF_Font *font_cell = NULL;

int render_init(const char *fontpath, int fontsize)
{
    if (TTF_Init() != 0) {
        printf("TTF_Init Error: %s\n", TTF_GetError());
        return 1;
    }

    const char *fallback[] = { "/System/Library/Fonts/Helvetica.ttc", NULL };

    const char *usepath = fontpath ? fontpath : fallback[0];

    font_ui = TTF_OpenFont(usepath, fontsize);
    if (!font_ui && !fontpath) {
        // bandyk kitus fallback (jei pridėsi)
        font_ui = NULL;
    }
    if (!font_ui) {
        printf("TTF_OpenFont(UI) Error: %s\n", TTF_GetError());
        return 2;
    }

    int cell_size = (fontsize >= 16) ? (fontsize - 4) : fontsize;
    font_cell = TTF_OpenFont(usepath, cell_size);
    if (!font_cell) font_cell = font_ui; // jei nepavyko – naudok tą patį

    return 0;
}

void render_quit(void)
{
    if (font_cell && font_cell != font_ui) {
        TTF_CloseFont(font_cell);
        font_cell = NULL;
    }
    if (font_ui) {
        TTF_CloseFont(font_ui);
        font_ui = NULL;
    }
    TTF_Quit();
}

static void addorupdatem(int id, const char *text, SDL_Color color)
{
    for (int i = 0; i < kiekm; i++) {
        if (m[i].id == id) {
            strncpy(m[i].text, text, maxmilg-1);
            m[i].text[maxmilg-1] = '\0';
            m[i].color = color;
            return;
        }
    }
    if (kiekm < maxm) {
        m[kiekm].id = id;
        strncpy(m[kiekm].text, text, maxmilg-1);
        m[kiekm].text[maxmilg-1] = '\0';
        m[kiekm].color = color;
        kiekm++;
        return;
    }
    for (int i = 1; i < maxm; i++) m[i-1] = m[i];
    m[maxm-1].id = id;
    strncpy(m[maxm-1].text, text, maxmilg-1);
    m[maxm-1].text[maxmilg-1] = '\0';
    m[maxm-1].color = color;
}

void clear_m(void) { kiekm = 0; }

static void draw_text_font(SDL_Renderer *r, TTF_Font *f, const char *txt, int x, int y, SDL_Color c)
{
    if (!txt || !*txt || !f) return;

    SDL_Surface *s = TTF_RenderUTF8_Solid(f, txt, c);
    if (!s) return;

    SDL_Texture *t = SDL_CreateTextureFromSurface(r, s);
    if (!t) { SDL_FreeSurface(s); return; }

    SDL_Rect dst = {x, y, s->w, s->h};
    SDL_RenderCopy(r, t, NULL, &dst);

    SDL_DestroyTexture(t);
    SDL_FreeSurface(s);
}

// tekstas centruotas + optional rotation
static void draw_text_center(SDL_Renderer *r, TTF_Font *f, const char *txt,
                             int cx, int cy, SDL_Color c, double angle_deg)
{
    if (!txt || !*txt || !f) return;

    SDL_Surface *s = TTF_RenderUTF8_Solid(f, txt, c);
    if (!s) return;

    SDL_Texture *t = SDL_CreateTextureFromSurface(r, s);
    if (!t) { SDL_FreeSurface(s); return; }

    SDL_Rect dst = { cx - s->w/2, cy - s->h/2, s->w, s->h };
    SDL_Point center = { s->w/2, s->h/2 };

    if (angle_deg == 0.0) {
        SDL_RenderCopy(r, t, NULL, &dst);
    } else {
        SDL_RenderCopyEx(r, t, NULL, &dst, angle_deg, &center, SDL_FLIP_NONE);
    }

    SDL_DestroyTexture(t);
    SDL_FreeSurface(s);
}

// labai paprastas wrap į 2 eilutes (pagal _ arba . arba tarpo vidurį)
static void split2(const char *in, char *a, size_t asz, char *b, size_t bsz)
{
    a[0] = b[0] = '\0';
    if (!in) return;

    size_t n = strlen(in);
    if (n < 10) { strncpy(a, in, asz-1); a[asz-1]='\0'; return; }

    // ieškom gero skirtuko netoli vidurio
    size_t mid = n/2;
    size_t best = (size_t)-1;

    for (size_t d = 0; d < n; d++) {
        size_t i1 = (mid >= d) ? (mid - d) : 0;
        size_t i2 = (mid + d < n) ? (mid + d) : (n-1);

        char c1 = in[i1];
        char c2 = in[i2];

        if (c1=='_' || c1=='.' || c1==' ' ) { best = i1; break; }
        if (c2=='_' || c2=='.' || c2==' ' ) { best = i2; break; }
        if (i1==0 && i2==n-1) break;
    }

    if (best == (size_t)-1 || best < 2 || best > n-2) {
        // fallback: tiesiog nukerpam
        strncpy(a, in, asz-1); a[asz-1]='\0';
        return;
    }

    // a = [0..best-1], b = [best+1..]
    size_t alen = best;
    if (alen >= asz) alen = asz-1;
    memcpy(a, in, alen);
    a[alen] = '\0';

    const char *rest = in + best + 1;
    strncpy(b, rest, bsz-1);
    b[bsz-1] = '\0';
}

static void drawinfo(SDL_Renderer *renderer, SDL_Rect sidebarRect)
{
    int y = sidebarRect.y + 10;
    for (int i = 0; i < kiekm; i++) {
        // multi-line per \n
        char *copy = strdup(m[i].text);
        if (!copy) continue;

        char *line = strtok(copy, "\n");
        int yy = y;
        while (line) {
            draw_text_font(renderer, font_ui, line, sidebarRect.x + 10, yy, m[i].color);
            yy += 18;
            line = strtok(NULL, "\n");
        }
        free(copy);

        y += 60;
        if (y > sidebarRect.y + sidebarRect.h - 60) break;
    }
}

static void render_cell(SDL_Renderer *renderer, cell *c, int kiekplayer)
{
    SDL_Rect cell_rect = {(int)c->rect.x, (int)c->rect.y, (int)c->rect.w, (int)c->rect.h};

    SDL_Rect top = {cell_rect.x, cell_rect.y, cell_rect.w, cell_rect.h / 3};
    SDL_Rect bot = {cell_rect.x, cell_rect.y + cell_rect.h / 3, cell_rect.w, cell_rect.h - top.h};

    SDL_SetRenderDrawColor(renderer, c->tcolor.r, c->tcolor.g, c->tcolor.b, 255);
    SDL_RenderFillRect(renderer, &top);

    SDL_SetRenderDrawColor(renderer, c->bcolor.r, c->bcolor.g, c->bcolor.b, 255);
    SDL_RenderFillRect(renderer, &bot);

    // owner bar
    if (c->owner > 0 && P_bought) {
        int pid = c->owner - 1;
        if (pid >= 0 && pid < kiekplayer) {
            SDL_Color col = P_bought[pid];
            SDL_SetRenderDrawColor(renderer, col.r, col.g, col.b, 255);
            SDL_Rect bar = {cell_rect.x, cell_rect.y, cell_rect.w, cell_rect.h / 12};
            SDL_RenderFillRect(renderer, &bar);
        }
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &cell_rect);

    // --- TITLE + PRICE (kaip tavo paveikslėly)
    char line1[64], line2[64];
    split2(c->btext[0] ? c->btext : c->ttext, line1, sizeof(line1), line2, sizeof(line2));

    char pr[32] = "";
    if (c->price > 0) snprintf(pr, sizeof(pr), "%d", c->price);

    // left/right langeliai: w==CORNER_SIZE && h==CELL_SIZE => rotuojam 90°
    int is_side_lr = (cell_rect.w == CORNER_SIZE && cell_rect.h == CELL_SIZE);

    int cx_top = top.x + top.w/2;
    int cy_top = top.y + top.h/2;

    if (!is_side_lr) {
        // top/bottom: normaliai, centre
        draw_text_center(renderer, font_cell, line1, cx_top, cy_top - 6, (SDL_Color){0,0,0,255}, 0.0);
        if (line2[0]) draw_text_center(renderer, font_cell, line2, cx_top, cy_top + 8, (SDL_Color){0,0,0,255}, 0.0);

        if (pr[0]) {
            int cx_bot = bot.x + bot.w/2;
            int cy_bot = bot.y + bot.h - 10;
            draw_text_center(renderer, font_cell, pr, cx_bot, cy_bot, (SDL_Color){0,0,0,255}, 0.0);
        }
    } else {
        // left/right: pasukam tekstą 90°
        // centruojam top juostos vidury, bet su rotacija
        draw_text_center(renderer, font_cell, line1, cx_top, cy_top, (SDL_Color){0,0,0,255}, 90.0);

        if (pr[0]) {
            int cx_bot = bot.x + bot.w/2;
            int cy_bot = bot.y + bot.h/2;
            draw_text_center(renderer, font_cell, pr, cx_bot, cy_bot, (SDL_Color){0,0,0,255}, 90.0);
        }
    }
}

static void draw_player_token(SDL_Renderer *renderer, cell *c, int player_index)
{
    if (!P_bought) return;

    SDL_Rect cell_rect = {(int)c->rect.x, (int)c->rect.y, (int)c->rect.w, (int)c->rect.h};

    int token_size = 12;
    int col = player_index % 3;
    int row = player_index / 3;
    int x = cell_rect.x + 5 + col*(token_size+4);
    int y = cell_rect.y + cell_rect.h - token_size - 5 - row*(token_size+4);

    SDL_Color token = P_bought[player_index < 6 ? player_index : (player_index%6)];
    SDL_SetRenderDrawColor(renderer, token.r, token.g, token.b, 255);
    SDL_Rect tr = {x, y, token_size, token_size};
    SDL_RenderFillRect(renderer, &tr);
    SDL_SetRenderDrawColor(renderer, 0,0,0,255);
    SDL_RenderDrawRect(renderer, &tr);
}

static void render_center_cards(SDL_Renderer *renderer, SDL_Rect centerRect)
{
    int rectW = centerRect.w / 3;
    int rectH = centerRect.h / 6;

    int cx1 = centerRect.x + centerRect.w / 3;
    int cy1 = centerRect.y + centerRect.h / 2;
    int cx2 = centerRect.x + 2 * centerRect.w / 3;
    int cy2 = centerRect.y + centerRect.h / 2;

    SDL_Rect r1 = {cx1 - rectW/2, cy1 - rectH/2, rectW, rectH};
    SDL_Rect r2 = {cx2 - rectW/2, cy2 - rectH/2, rectW, rectH};

    SDL_SetRenderDrawColor(renderer, rand_color.r, rand_color.g, rand_color.b, 255);
    SDL_RenderFillRect(renderer, &r1);
    SDL_SetRenderDrawColor(renderer, 0,0,0,255);
    SDL_RenderDrawRect(renderer, &r1);
    draw_text_center(renderer, font_ui, "RAND()", r1.x + r1.w/2, r1.y + r1.h/2, (SDL_Color){0,0,0,255}, -20.0);

    SDL_SetRenderDrawColor(renderer, system_logs.r, system_logs.g, system_logs.b, 255);
    SDL_RenderFillRect(renderer, &r2);
    SDL_SetRenderDrawColor(renderer, 0,0,0,255);
    SDL_RenderDrawRect(renderer, &r2);
    draw_text_center(renderer, font_ui, "SYSTEM LOGS", r2.x + r2.w/2, r2.y + r2.h/2, (SDL_Color){0,0,0,255}, -20.0);
}

void render_board(SDL_Renderer *renderer, cell cells[], player players[], int kiekplayer,
                  SDL_FRect centerRectF, SDL_FRect sidebarRectF)
{
    SDL_Rect centerRect  = {(int)centerRectF.x,  (int)centerRectF.y,  (int)centerRectF.w,  (int)centerRectF.h};
    SDL_Rect sidebarRect = {(int)sidebarRectF.x, (int)sidebarRectF.y, (int)sidebarRectF.w, (int)sidebarRectF.h};

    for (int i = 0; i < 44; i++) {
        render_cell(renderer, &cells[i], kiekplayer);
        for (int p = 0; p < kiekplayer; p++)
            if (players[p].poz_l == i)
                draw_player_token(renderer, &cells[i], p);
    }

    SDL_SetRenderDrawColor(renderer, middle.r, middle.g, middle.b, 255);
    SDL_RenderFillRect(renderer, &centerRect);
    render_center_cards(renderer, centerRect);

    SDL_SetRenderDrawColor(renderer, 40,40,40,255);
    SDL_RenderFillRect(renderer, &sidebarRect);
    SDL_SetRenderDrawColor(renderer, 100,100,100,255);
    SDL_RenderDrawRect(renderer, &sidebarRect);

    drawinfo(renderer, sidebarRect);
}

// --- UI messages (tavo) ---
void show_start_screen(void){ addorupdatem(1,"1 - testi\n2 - naujas",(SDL_Color){255,255,255,255}); }
void show_ask_player_count(void){ addorupdatem(2,"Pasirinkite zaideju skaiciu 2-6",(SDL_Color){255,255,255,255}); }
void show_game_instructions(void){
    addorupdatem(3,"Valdymas:\nm - mesti\nEnter - baigti ejima\nn - pirkti nama\ns - remontas\nr - praleisti\np - instrukcijos\ne - iseiti\nPele - pirkti laukeli",(SDL_Color){255,255,255,255});
}
void show_press_enter_prompt(void){ addorupdatem(4,"Spausk Enter",(SDL_Color){200,200,200,255}); }
void show_player_turn(int idx, int id, int bytes, int uf, int pr){ (void)idx;(void)uf;(void)pr; char b[128]; snprintf(b,sizeof(b),"ZAIDEJAS %d\nBaitu: %d\nm - mesti",id,bytes); addorupdatem(10+idx,b,(SDL_Color){255,255,255,255}); }
void show_roll_result(int v){ char b[64]; snprintf(b,sizeof(b),"Isridenote: %d",v); addorupdatem(51,b,(SDL_Color){255,255,255,255}); }
void show_cell_bought(int a){ char b[80]; snprintf(b,sizeof(b),"Nupirkta! Liko: %d",a); addorupdatem(100,b,(SDL_Color){0,255,0,255}); }
void show_cell_already_bought(void){ addorupdatem(99,"Jau nupirkta!",(SDL_Color){255,0,0,255}); }
void show_wrong_cell_press(void){ addorupdatem(101,"Nestovite ant sito laukelio",(SDL_Color){255,0,0,255}); }
void show_card_prompt(void){ addorupdatem(200,"Spausk ant kortu kalades centre",(SDL_Color){255,255,255,255}); }
void show_wrong_deck_press(void){ addorupdatem(201,"Netinkama kalade",(SDL_Color){255,0,0,255}); }
void show_card_drawn(const char *t){ char b[256]; snprintf(b,sizeof(b),"Kortele:\n%s",t?t:""); addorupdatem(202,b,(SDL_Color){255,255,255,255}); }
void show_code_ai_penalty(int p, int after){ char b[128]; snprintf(b,sizeof(b),"Bauda: -%d\nLiko: %d",p,after); addorupdatem(300,b,(SDL_Color){255,255,255,255}); }
void show_memory_bonus(int bonus, int after){ char b[128]; snprintf(b,sizeof(b),"Bonus: +%d\nLiko: %d",bonus,after); addorupdatem(310,b,(SDL_Color){255,255,255,255}); }
void show_error_cell_offer(int c){ char b[128]; snprintf(b,sizeof(b),"ERROR!\ns - moketi %d\nr - praleisti",c); addorupdatem(500,b,(SDL_Color){255,255,255,255}); }
void show_roll_again_message(int r){ char b[80]; snprintf(b,sizeof(b),"Mesk dar karta! pries tai %d",r); addorupdatem(600,b,(SDL_Color){255,255,255,255}); }
void show_buy_house_prompt(int cost){ char b[128]; snprintf(b,sizeof(b),"Pirkti nama? kaina %d\nn - pirkti",cost); addorupdatem(700,b,(SDL_Color){255,255,0,255}); }
void show_hotel_bought(void){ addorupdatem(702,"Turite 4 namus (projektas)!",(SDL_Color){0,255,0,255}); }
void show_house_limit_error(void){ addorupdatem(703,"Negalima 2 namu per ejima",(SDL_Color){255,0,0,255}); }
void show_bankruptcy_message(int id){ char b[64]; snprintf(b,sizeof(b),"Zaidejas %d BANKRUTAVO",id); addorupdatem(800,b,(SDL_Color){255,0,0,255}); }
void show_game_over_message(int id){ char b[64]; snprintf(b,sizeof(b),"LAIMEJO %d",id); addorupdatem(900,b,(SDL_Color){0,255,0,255}); }
void show_current_player_info(int id, int pos, int money, int jail){ char b[128]; snprintf(b,sizeof(b),"Zaidejas %d\nPoz %d\nBaitu %d\nJail %d",id,pos,money,jail); addorupdatem(950,b,(SDL_Color){255,255,255,255}); }
void show_memory_error_basic(void){ addorupdatem(90,"Klaida: nepakanka atminties",(SDL_Color){255,0,0,255}); }
void show_memory_error_save_fail(void){ addorupdatem(92,"Klaida: nepavyko issaugoti",(SDL_Color){255,0,0,255}); }
void show_file_read_error(void){ addorupdatem(93,"Klaida: nepavyko perskaityti",(SDL_Color){255,0,0,255}); }

// papildomos, kad zaidimo_ciklas.c nesprogtų
void show_jail_message(int player_id){ char b[80]; snprintf(b,sizeof(b),"Zaidejas %d -> KALEJIMAS",player_id); addorupdatem(520,b,(SDL_Color){255,255,0,255}); }
void show_passed_start(int player_id,int money){ char b[128]; snprintf(b,sizeof(b),"Zaidejas %d perejo START\nBaitu: %d",player_id,money); addorupdatem(521,b,(SDL_Color){0,255,0,255}); }
void show_buy_option(int player_id,int pos,int price){ char b[160]; snprintf(b,sizeof(b),"Zaidejas %d gali pirkti\nPoz %d\nKaina: %d\nPele - pirkti",player_id,pos,price); addorupdatem(522,b,(SDL_Color){255,255,0,255}); }
void show_tax_message(int player_id,int taxAmount,int money){ char b[160]; snprintf(b,sizeof(b),"Mokestis zaidejui %d: -%d\nLiko: %d",player_id,taxAmount,money); addorupdatem(523,b,(SDL_Color){255,255,255,255}); }
void show_card_draw_message(const char *which){ char b[128]; snprintf(b,sizeof(b),"Trauk kortele: %s",which?which:""); addorupdatem(524,b,(SDL_Color){255,255,255,255}); }
void show_insufficient_money(int player_id,int needed){ char b[128]; snprintf(b,sizeof(b),"Zaidejas %d: nepakanka baitu\nReikia: %d",player_id,needed); addorupdatem(525,b,(SDL_Color){255,0,0,255}); }
void show_property_bought(int player_id,int pos,int money){ char b[160]; snprintf(b,sizeof(b),"Zaidejas %d nupirko laukeli\nPoz %d\nLiko: %d",player_id,pos,money); addorupdatem(526,b,(SDL_Color){0,255,0,255}); }
void show_house_bought(int player_id,int pos,int houses){ char b[160]; snprintf(b,sizeof(b),"Zaidejas %d nupirko nama\nPoz %d\nNamai: %d",player_id,pos,houses); addorupdatem(701,b,(SDL_Color){0,255,0,255}); }