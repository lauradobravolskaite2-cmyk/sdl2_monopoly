/*#include <stdio.h>
#include "save_open.h"
#include "render.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

int save_game(player *players, int sk) // CHANGED
{
    char name[64];

    for (int i = 0; i < sk; i++)
    {
        snprintf(name, sizeof(name), "data_player_%d.txt", i + 1);
        FILE *f = fopen(name, "w");
        if (!f) { show_memory_error_save_fail(); return -1; }

        fprintf(f, "%d\n%d\n%d\n", players[i].id, players[i].money, players[i].poz_l);

        for (int a = 0; a < 44; a++) {
            fprintf(f, "%d %d\n", players[i].own[a][0], players[i].own[a][1]);
        }
        fclose(f);
    }

    FILE *ff = fopen("p_number.txt", "w");
    if (!ff) { show_memory_error_save_fail(); return -1; }
    fprintf(ff, "%d", sk);
    fclose(ff);

    addorupdatem(951, "Zaidimas issaugotas (K)", (SDL_Color){0,255,0,255});
    return 0;
}

int open_number(void)
{
    int p_number = 0;
    FILE *ff = fopen("p_number.txt", "r");
    if (!ff) { show_memory_error_basic(); return -1; }
    if (fscanf(ff, "%d", &p_number) != 1) { fclose(ff); show_file_read_error(); return -1; }
    fclose(ff);
    return p_number;
}

int open_data(player *players, int p_number)
{
    char name[64];
    for (int i = 0; i < p_number; i++)
    {
        snprintf(name, sizeof(name), "data_player_%d.txt", i + 1);
        FILE *f = fopen(name, "r");
        if (!f) { show_memory_error_basic(); return -1; }

        if (fscanf(f, "%d%d%d", &players[i].id, &players[i].money, &players[i].poz_l) != 3)
        { fclose(f); show_file_read_error(); return -1; }

        for (int a = 0; a < 44; a++) {
            if (fscanf(f, "%d%d", &players[i].own[a][0], &players[i].own[a][1]) != 2)
            { fclose(f); show_file_read_error(); return -1; }
        }
        fclose(f);
    }
    return 0;
}*/

#include <stdio.h>
#include "save_open.h"
#include "render.h"   // kad galėtum rodyti pranešimus (show_game_saved, show_memory_error_...)

int save_game(player *players, int sk)
{
    char name[64];

    for (int i = 0; i < sk; i++)
    {
        snprintf(name, sizeof(name), "data_player_%d.txt", i + 1);
        FILE *f = fopen(name, "w");
        if (!f) { show_memory_error_save_fail(); return -1; }

        fprintf(f, "%d\n%d\n%d\n", players[i].id, players[i].money, players[i].poz_l);

        for (int a = 0; a < 44; a++) {
            fprintf(f, "%d %d\n", players[i].own[a][0], players[i].own[a][1]);
        }
        fclose(f);
    }

    FILE *ff = fopen("p_number.txt", "w");
    if (!ff) { show_memory_error_save_fail(); return -1; }
    fprintf(ff, "%d", sk);
    fclose(ff);

    show_game_saved();
    return 0;
}

int open_number(void)
{
    int p_number = 0;
    FILE *ff = fopen("p_number.txt", "r");
    if (!ff) { show_memory_error_basic(); return -1; }
    if (fscanf(ff, "%d", &p_number) != 1) { fclose(ff); show_file_read_error(); return -1; }
    fclose(ff);
    return p_number;
}

int open_data(player *players, int p_number)
{
    char name[64];

    for (int i = 0; i < p_number; i++)
    {
        snprintf(name, sizeof(name), "data_player_%d.txt", i + 1);
        FILE *f = fopen(name, "r");
        if (!f) { show_memory_error_basic(); return -1; }

        if (fscanf(f, "%d%d%d", &players[i].id, &players[i].money, &players[i].poz_l) != 3)
        { fclose(f); show_file_read_error(); return -1; }

        for (int a = 0; a < 44; a++) {
            if (fscanf(f, "%d%d", &players[i].own[a][0], &players[i].own[a][1]) != 2)
            { fclose(f); show_file_read_error(); return -1; }
        }

        fclose(f);
    }

    return 0;
}