#include <stdlib.h>
#include "zaidejai.h"
#include "render.h"
#include "save_open.h"
#include "zaidimo_ciklas.h"
#include <SDL_ttf.h>

void zaideju_duomenys(int ats, int sk)
{
    if (ats == 1)
    {
        sk = open_number();
        if (sk <= 0) return;
    }

    player *players = (player*)malloc(sk * sizeof(player));
    if (!players) { show_memory_error_basic(); return; }

    for (int i = 0; i < sk; i++)
    {
        players[i].id = i + 1;
        players[i].money = (ats != 1) ? 2527267 : 0;
        players[i].poz_l = 0;
        players[i].kalejimo_laikas = 0;
        players[i].turi_iseiti_is_kalejimo_korta = 0;
        for (int a = 0; a < 44; a++)
            for (int b = 0; b < 2; b++)
                players[i].own[a][b] = 0;
    }

    if (ats == 1)
    {
        if (open_data(players, sk) != 0) { free(players); return; }
    }

    game_cycle(players, sk);

    free(players);
}
