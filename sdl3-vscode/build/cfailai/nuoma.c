#include "veiksmai.h"
#include <SDL_ttf.h>


int moketi_nuoma(player *players, int currentPlayer, int kiek)
{
    if (players[currentPlayer].kalejimo_laikas > 0) return 0;

    Biblioteka *bib = gauti_biblioteka_pagal_pozicija(players[currentPlayer].poz_l);
    if (!bib) return 0;
    if (bib->savininko_id == -1 || bib->savininko_id == players[currentPlayer].id) return 0;

    int nuoma;
    switch (bib->namu_skaicius) {
        case 0: nuoma = bib->nuoma_be_namo; break;
        case 1: nuoma = bib->nuoma_su_1_namu; break;
        case 2: nuoma = bib->nuoma_su_2_namais; break;
        case 3: nuoma = bib->nuoma_su_3_namais; break;
        case 4: nuoma = bib->nuoma_su_viesbuciu; break;
        default: nuoma = bib->nuoma_be_namo; break;
    }

    if (players[currentPlayer].money < nuoma) return -1;
    players[currentPlayer].money -= nuoma;

    for (int i = 0; i < kiek; i++)
        if (players[i].id == bib->savininko_id)
            players[i].money += nuoma;

    return 1;
}