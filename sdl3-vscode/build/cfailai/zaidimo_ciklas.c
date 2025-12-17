/*#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "zaidimo_ciklas.h"
#include "lenta.h"
#include "render.h"
#include "colors.h"
#include "input.h"
#include "veiksmai.h"
#include "korteles.h"
#include "save_open.h"

static int is_special_tile(int pos)
{
    return (pos == 0 || pos == 4 || pos == 9 || pos == 10 || pos == 11 || pos == 17 || pos == 20 || pos == 22 ||
            pos == 24 || pos == 28 || pos == 33 || pos == 37 || pos == 40 || pos == 42);
}

void game_cycle(player *players, int sk)
{
    if (sk < 2)
        return;

    srand((unsigned)time(NULL));

    // data init
    nuskaityti_bibliotekas();
    nuskaityti_rand_korteles();
    nuskaityti_system_logs_korteles();

    // SDL2 inicijavimas
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL initialization failed: %s\n", SDL_GetError());
        return;
    }
    
    if (TTF_Init() < 0) {
        printf("TTF initialization failed: %s\n", TTF_GetError());
        SDL_Quit();
        return;
    }

    SDL_Window *window = SDL_CreateWindow("Monopoly (C/SDL2)",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          SCREEN_WIDTH, SCREEN_HEIGHT,
                                          0);
    if (!window) {
        printf("Window creation failed: %s\n", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Renderer creation failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return;
    }

    player_colors(sk);

    board_layout_t layout = compute_board_layout();
    cell cells[44];
    buildCells(cells);

    int mas[44] = {0};
    for (int i = 0; i < 44; i++)
        if (is_special_tile(i))
            mas[i] = 2;

    int currentPlayer = 0;
    int awaitingRoll = 1;
    int awaitingBuy = 0;
    int awaitingRepairChoice = 0;
    int awaitingCardDraw = 0;
    int cardTypeToDraw = 0;

    int bought_property_this_turn = 0;
    int house_bought_this_turn = 0;

    show_game_instructions();
    show_player_turn(currentPlayer, players[currentPlayer].id, players[currentPlayer].money, 0, 0);

    int running = 1;
    SDL_Event e;
    int action = -1;
    
    while (running) {
        action = -1;

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = 0;
                break;
            }

            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                case SDLK_m: action = INPUT_ROLL; break;
                case SDLK_RETURN: action = INPUT_NEXT_PLAYER; break;
                case SDLK_n: action = INPUT_BUY_HOUSE; break;
                case SDLK_s: action = INPUT_REPAIR; break;
                case SDLK_r: action = INPUT_SKIP_TURN; break;
                case SDLK_p: action = INPUT_PAUSE; break;
                case SDLK_e: action = INPUT_EXIT; break;
                case SDLK_k: action = 999; break;
                default: break;
                }
            }

            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
                action = INPUT_MOUSE_CLICK;
        }

        show_current_player_info(players[currentPlayer].id,
                                 players[currentPlayer].poz_l,
                                 players[currentPlayer].money,
                                 players[currentPlayer].kalejimo_laikas);

        if (action == INPUT_EXIT)
            running = 0;

        else if (action == INPUT_PAUSE)
            show_game_instructions();

        else if (action == 999)
            save_game(players, sk);

        else if (action == INPUT_NEXT_PLAYER) {
            if (!awaitingBuy && !awaitingRepairChoice && !awaitingCardDraw) {
                currentPlayer = (currentPlayer + 1) % sk;
                awaitingRoll = 1;
                bought_property_this_turn = 0;
                house_bought_this_turn = 0;
                show_player_turn(currentPlayer, players[currentPlayer].id, players[currentPlayer].money, 0, 0);
            }
        }

        else if (action == INPUT_ROLL) {
            if (awaitingRoll && players[currentPlayer].kalejimo_laikas == 0) {
                int dice1 = rand() % 6 + 1;
                int dice2 = rand() % 6 + 1;
                
                show_player_turn(currentPlayer, players[currentPlayer].id, players[currentPlayer].money, dice1, dice2);
                
                int total = dice1 + dice2;
                
                if (dice1 == dice2)
                    players[currentPlayer].kartai_ismestas_dublikatas++;
                else
                    players[currentPlayer].kartai_ismestas_dublikatas = 0;
                
                if (players[currentPlayer].kartai_ismestas_dublikatas == 3) {
                    players[currentPlayer].kalejimo_laikas = 3;
                    players[currentPlayer].poz_l = 10;
                    players[currentPlayer].kartai_ismestas_dublikatas = 0;
                    // show_jail_message(players[currentPlayer].id);
                    char b[128];
                    snprintf(b, sizeof(b), "Zaidejas %d i kalejima!", players[currentPlayer].id);
                    addorupdatem(801, b, (SDL_Color){255,0,0,255});
                } else {
                    int newPos = players[currentPlayer].poz_l + total;
                    if (newPos >= 44) {
                        newPos -= 44;
                        players[currentPlayer].money += 400;
                        // show_passed_start(players[currentPlayer].id, players[currentPlayer].money);
                        char b[128];
                        snprintf(b, sizeof(b), "Zaidejas %d praejo pradzia\n+400 baitu\nDabar: %d", 
                                players[currentPlayer].id, players[currentPlayer].money);
                        addorupdatem(802, b, (SDL_Color){0,255,0,255});
                    }
                    players[currentPlayer].poz_l = newPos;
                    awaitingRoll = 0;
                    
                    // Check if player landed on a property
                    if (cells[newPos].owner == -1 && mas[newPos] != 2) {
                        awaitingBuy = 1;
                        // show_buy_option(players[currentPlayer].id, newPos, cells[newPos].price);
                        char b[128];
                        snprintf(b, sizeof(b), "Zaidejas %d gali pirkti\nPoz %d\nKaina: %d\nPele - pirkti", 
                                players[currentPlayer].id, newPos, cells[newPos].price);
                        addorupdatem(803, b, (SDL_Color){255,255,0,255});
                    } else if (mas[newPos] == 2) {
                        // Special tile logic
                        if (newPos == 4 || newPos == 38) {
                            // Tax tiles
                            int taxAmount = (newPos == 4) ? 200 : 100;
                            players[currentPlayer].money -= taxAmount;
                            // show_tax_message(players[currentPlayer].id, taxAmount, players[currentPlayer].money);
                            char b[128];
                            snprintf(b, sizeof(b), "Zaidejas %d moka mokescius\n-%d\nLiko: %d", 
                                    players[currentPlayer].id, taxAmount, players[currentPlayer].money);
                            addorupdatem(804, b, (SDL_Color){255,100,0,255});
                        } else if (newPos == 20 || newPos == 30) {
                            // Free parking / Go to jail
                            if (newPos == 30) {
                                players[currentPlayer].poz_l = 10;
                                players[currentPlayer].kalejimo_laikas = 3;
                                // show_jail_message(players[currentPlayer].id);
                                char b[128];
                                snprintf(b, sizeof(b), "Zaidejas %d i kalejima!", players[currentPlayer].id);
                                addorupdatem(805, b, (SDL_Color){255,0,0,255});
                            }
                        } else if (newPos == 7 || newPos == 22 || newPos == 36) {
                            // Chance tiles
                            awaitingCardDraw = 1;
                            cardTypeToDraw = 0; // Chance
                            // show_card_draw_message("CHANCE");
                            addorupdatem(806, "Traukite CHANCE kortele", (SDL_Color){255,255,255,255});
                        } else if (newPos == 2 || newPos == 17 || newPos == 33) {
                            // Community chest tiles
                            awaitingCardDraw = 1;
                            cardTypeToDraw = 1; // Community chest
                            // show_card_draw_message("COMMUNITY CHEST");
                            addorupdatem(807, "Traukite COMMUNITY CHEST kortele", (SDL_Color){255,255,255,255});
                        }
                    }
                }
            }
        }
        else if (action == INPUT_BUY_HOUSE) {
            if (!house_bought_this_turn) {
                // House buying logic
                int pos = players[currentPlayer].poz_l;
                if (cells[pos].owner == players[currentPlayer].id && cells[pos].type == 0) {
                    if (players[currentPlayer].money >= cells[pos].house_price) {
                        players[currentPlayer].money -= cells[pos].house_price;
                        cells[pos].houses++;
                        house_bought_this_turn = 1;
                        // show_house_bought(players[currentPlayer].id, pos, cells[pos].houses);
                        char b[128];
                        snprintf(b, sizeof(b), "Zaidejas %d nupirko nama\nPoz %d\nNamų: %d\nKaina: -%d\nLiko: %d", 
                                players[currentPlayer].id, pos, cells[pos].houses, 
                                cells[pos].house_price, players[currentPlayer].money);
                        addorupdatem(808, b, (SDL_Color){0,200,0,255});
                    } else {
                        // show_insufficient_money(players[currentPlayer].id, cells[pos].house_price);
                        char b[128];
                        snprintf(b, sizeof(b), "Zaidejas %d\nNepakanka pinigu namui!\nReikia: %d", 
                                players[currentPlayer].id, cells[pos].house_price);
                        addorupdatem(809, b, (SDL_Color){255,0,0,255});
                    }
                }
            }
        }
        else if (action == INPUT_MOUSE_CLICK) {
            if (awaitingBuy) {
                int pos = players[currentPlayer].poz_l;
                if (players[currentPlayer].money >= cells[pos].price) {
                    players[currentPlayer].money -= cells[pos].price;
                    cells[pos].owner = players[currentPlayer].id;
                    bought_property_this_turn = 1;
                    awaitingBuy = 0;
                    // show_property_bought(players[currentPlayer].id, pos, players[currentPlayer].money);
                    char b[128];
                    snprintf(b, sizeof(b), "Zaidejas %d nupirko laukeli\nPoz %d\nLiko: %d", 
                            players[currentPlayer].id, pos, players[currentPlayer].money);
                    addorupdatem(810, b, (SDL_Color){0,255,0,255});
                } else {
                    // show_insufficient_money(players[currentPlayer].id, cells[pos].price);
                    char b[128];
                    snprintf(b, sizeof(b), "Zaidejas %d\nNepakanka pinigu laukeliui!\nReikia: %d", 
                            players[currentPlayer].id, cells[pos].price);
                    addorupdatem(811, b, (SDL_Color){255,0,0,255});
                    awaitingBuy = 0;
                }
            }
        }
        else if (action == INPUT_REPAIR && awaitingRepairChoice) {
            // Repair logic
            awaitingRepairChoice = 0;
        }
        else if (action == INPUT_SKIP_TURN && awaitingRepairChoice) {
            awaitingRepairChoice = 0;
        }

        // Render
        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);
        // Pataisyta: SDL_Rect, ne SDL_FRect
        SDL_Rect centerRect = {(int)layout.centerRect.x, (int)layout.centerRect.y, 
                               (int)layout.centerRect.w, (int)layout.centerRect.h};
        SDL_Rect sidebarRect = {(int)layout.sidebarRect.x, (int)layout.sidebarRect.y,
                                (int)layout.sidebarRect.w, (int)layout.sidebarRect.h};
        render_board(renderer, cells, players, sk, centerRect, sidebarRect);
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    free_player_colors();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}*/

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "zaidimo_ciklas.h"
#include "lenta.h"
#include "render.h"
#include "colors.h"
#include "input.h"
#include "veiksmai.h"
#include "korteles.h"
#include "save_open.h"

static int is_special_tile(int pos)
{
    // tavo special tile sąrašas (palieku kaip buvo)
    return (pos == 0 || pos == 4 || pos == 9 || pos == 10 || pos == 11 || pos == 17 || pos == 20 || pos == 22 ||
            pos == 24 || pos == 28 || pos == 33 || pos == 37 || pos == 40 || pos == 42);
}

void game_cycle(player *players, int sk)
{
    if (!players || sk < 2) return;

    srand((unsigned)time(NULL));

    // data init
    nuskaityti_bibliotekas();
    nuskaityti_rand_korteles();
    nuskaityti_system_logs_korteles();

    // SDL2 init
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL initialization failed: %s\n", SDL_GetError());
        return;
    }

    SDL_Window *window = SDL_CreateWindow(
        "Monopoly (C/SDL2)",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        0
    );

    if (!window) {
        printf("Window creation failed: %s\n", SDL_GetError());
        SDL_Quit();
        return;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Renderer creation failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }

    // TTF + font init per render modulį (NEBEDARYK TTF_Init() čia)
    if (render_init(NULL, 18) != 0) {
        // render_init turi pats išspausdinti klaidą, jei nepavyko
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }

    player_colors(sk);

    board_layout_t layout = compute_board_layout();

    cell cells[44];
    buildCells(cells);

    int mas[44] = {0};
    for (int i = 0; i < 44; i++) {
        if (is_special_tile(i)) mas[i] = 2;
    }

    int currentPlayer = 0;
    int awaitingRoll = 1;
    int awaitingBuy = 0;
    int awaitingRepairChoice = 0;
    int awaitingCardDraw = 0;
    int cardTypeToDraw = 0;

    int bought_property_this_turn = 0;
    int house_bought_this_turn = 0;

    show_game_instructions();
    show_player_turn(currentPlayer, players[currentPlayer].id, players[currentPlayer].money, 0, 0);

    int running = 1;
    SDL_Event e;
    int action = -1;

    while (running) {
        action = -1;

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = 0;
                break;
            }

            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_m:      action = INPUT_ROLL; break;
                    case SDLK_RETURN: action = INPUT_NEXT_PLAYER; break;
                    case SDLK_n:      action = INPUT_BUY_HOUSE; break;
                    case SDLK_s:      action = INPUT_REPAIR; break;
                    case SDLK_r:      action = INPUT_SKIP_TURN; break;
                    case SDLK_p:      action = INPUT_PAUSE; break;
                    case SDLK_e:      action = INPUT_EXIT; break;
                    case SDLK_k:      action = 999; break; // save
                    default: break;
                }
            }

            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                action = INPUT_MOUSE_CLICK;
            }
        }

        show_current_player_info(players[currentPlayer].id,
                                 players[currentPlayer].poz_l,
                                 players[currentPlayer].money,
                                 players[currentPlayer].kalejimo_laikas);

        if (action == INPUT_EXIT) {
            running = 0;
        }
        else if (action == INPUT_PAUSE) {
            show_game_instructions();
        }
        else if (action == 999) {
            save_game(players, sk); // save_open.c
        }
        else if (action == INPUT_NEXT_PLAYER) {
            if (!awaitingBuy && !awaitingRepairChoice && !awaitingCardDraw) {
                currentPlayer = (currentPlayer + 1) % sk;
                awaitingRoll = 1;
                bought_property_this_turn = 0;
                house_bought_this_turn = 0;
                show_player_turn(currentPlayer, players[currentPlayer].id, players[currentPlayer].money, 0, 0);
            }
        }
        else if (action == INPUT_ROLL) {
            if (awaitingRoll && players[currentPlayer].kalejimo_laikas == 0) {
                int dice1 = rand() % 6 + 1;
                int dice2 = rand() % 6 + 1;

                show_player_turn(currentPlayer, players[currentPlayer].id, players[currentPlayer].money, dice1, dice2);

                int total = dice1 + dice2;

                if (dice1 == dice2) players[currentPlayer].kartai_ismestas_dublikatas++;
                else                players[currentPlayer].kartai_ismestas_dublikatas = 0;

                if (players[currentPlayer].kartai_ismestas_dublikatas == 3) {
                    players[currentPlayer].kalejimo_laikas = 3;
                    players[currentPlayer].poz_l = 10;
                    players[currentPlayer].kartai_ismestas_dublikatas = 0;

                    char b[128];
                    snprintf(b, sizeof(b), "Zaidejas %d i kalejima!", players[currentPlayer].id);
                    show_debug_message(801, b, (SDL_Color){255,0,0,255});
                } else {
                    int newPos = players[currentPlayer].poz_l + total;

                    if (newPos >= 44) {
                        newPos -= 44;
                        players[currentPlayer].money += 400;

                        char b[160];
                        snprintf(b, sizeof(b),
                                 "Zaidejas %d praejo pradzia\n+400 baitu\nDabar: %d",
                                 players[currentPlayer].id, players[currentPlayer].money);
                        show_debug_message(802, b, (SDL_Color){0,255,0,255});
                    }

                    players[currentPlayer].poz_l = newPos;
                    awaitingRoll = 0;

                    // property pirkimas
                    if (cells[newPos].owner == -1 && mas[newPos] != 2) {
                        awaitingBuy = 1;
                        char b[160];
                        snprintf(b, sizeof(b),
                                 "Zaidejas %d gali pirkti\nPoz %d\nKaina: %d\nPele - pirkti",
                                 players[currentPlayer].id, newPos, cells[newPos].price);
                        show_debug_message(803, b, (SDL_Color){255,255,0,255});
                    }
                    else if (mas[newPos] == 2) {
                        // special tile logic (palieku kaip buvo)
                        if (newPos == 4 || newPos == 38) {
                            int taxAmount = (newPos == 4) ? 200 : 100;
                            players[currentPlayer].money -= taxAmount;

                            char b[160];
                            snprintf(b, sizeof(b),
                                     "Zaidejas %d moka mokescius\n-%d\nLiko: %d",
                                     players[currentPlayer].id, taxAmount, players[currentPlayer].money);
                            show_debug_message(804, b, (SDL_Color){255,100,0,255});
                        }
                        else if (newPos == 20 || newPos == 30) {
                            if (newPos == 30) {
                                players[currentPlayer].poz_l = 10;
                                players[currentPlayer].kalejimo_laikas = 3;

                                char b[128];
                                snprintf(b, sizeof(b), "Zaidejas %d i kalejima!", players[currentPlayer].id);
                                show_debug_message(805, b, (SDL_Color){255,0,0,255});
                            }
                        }
                        else if (newPos == 7 || newPos == 22 || newPos == 36) {
                            awaitingCardDraw = 1;
                            cardTypeToDraw = 0;
                            show_debug_message(806, "Traukite CHANCE kortele", (SDL_Color){255,255,255,255});
                        }
                        else if (newPos == 2 || newPos == 17 || newPos == 33) {
                            awaitingCardDraw = 1;
                            cardTypeToDraw = 1;
                            show_debug_message(807, "Traukite COMMUNITY CHEST kortele", (SDL_Color){255,255,255,255});
                        }
                    }
                }
            }
        }
        else if (action == INPUT_BUY_HOUSE) {
            if (!house_bought_this_turn) {
                int pos = players[currentPlayer].poz_l;

                if (cells[pos].owner == players[currentPlayer].id && cells[pos].type == 0) {
                    if (players[currentPlayer].money >= cells[pos].house_price) {
                        players[currentPlayer].money -= cells[pos].house_price;
                        cells[pos].houses++;
                        house_bought_this_turn = 1;

                        // jei nori “gražų” pranešimą — naudok show_house_bought()
                        show_house_bought(cells[pos].house_price, players[currentPlayer].money);

                        // papildomai detalus debug:
                        char b[200];
                        snprintf(b, sizeof(b),
                                 "Zaidejas %d nupirko nama\nPoz %d\nNamu: %d\nKaina: -%d\nLiko: %d",
                                 players[currentPlayer].id, pos, cells[pos].houses,
                                 cells[pos].house_price, players[currentPlayer].money);
                        show_debug_message(808, b, (SDL_Color){0,200,0,255});
                    } else {
                        char b[160];
                        snprintf(b, sizeof(b),
                                 "Zaidejas %d\nNepakanka pinigu namui!\nReikia: %d",
                                 players[currentPlayer].id, cells[pos].house_price);
                        show_debug_message(809, b, (SDL_Color){255,0,0,255});
                    }
                }
            }
        }
        else if (action == INPUT_MOUSE_CLICK) {
            if (awaitingBuy) {
                int pos = players[currentPlayer].poz_l;

                if (players[currentPlayer].money >= cells[pos].price) {
                    players[currentPlayer].money -= cells[pos].price;
                    cells[pos].owner = players[currentPlayer].id;

                    bought_property_this_turn = 1;
                    awaitingBuy = 0;

                    char b[160];
                    snprintf(b, sizeof(b),
                             "Zaidejas %d nupirko laukeli\nPoz %d\nLiko: %d",
                             players[currentPlayer].id, pos, players[currentPlayer].money);
                    show_debug_message(810, b, (SDL_Color){0,255,0,255});
                } else {
                    char b[160];
                    snprintf(b, sizeof(b),
                             "Zaidejas %d\nNepakanka pinigu laukeliui!\nReikia: %d",
                             players[currentPlayer].id, cells[pos].price);
                    show_debug_message(811, b, (SDL_Color){255,0,0,255});
                    awaitingBuy = 0;
                }
            }
        }
        else if (action == INPUT_REPAIR && awaitingRepairChoice) {
            awaitingRepairChoice = 0;
        }
        else if (action == INPUT_SKIP_TURN && awaitingRepairChoice) {
            awaitingRepairChoice = 0;
        }

        // kortų traukimas (jei nori įjungti realiai)
        if (action == INPUT_MOUSE_CLICK && awaitingCardDraw) {
            // čia pagal tavo sistemą: paspaudi centre ant kaladės
            // jei turi detect funkciją, ją naudok; dabar palieku paprastai:
            (void)cardTypeToDraw;
            awaitingCardDraw = 0;
            show_card_prompt();
        }

        // Render
        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);

        render_board(renderer, cells, players, sk, layout.centerRect, layout.sidebarRect);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    free_player_colors();
    render_quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}