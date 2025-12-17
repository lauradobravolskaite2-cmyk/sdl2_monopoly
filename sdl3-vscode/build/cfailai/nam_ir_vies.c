#include "veiksmai.h"
#include "render.h"
#include <SDL_ttf.h>


int pirkti_nama(player *players, int currentPlayer, int pirkti)
{
    if (players[currentPlayer].kalejimo_laikas > 0) return 0;

    Biblioteka *bib = gauti_biblioteka_pagal_pozicija(players[currentPlayer].poz_l);
    if (!bib) return 0;
    if (bib->savininko_id != players[currentPlayer].id) return 0;
    if (pirkti != 1) return 0;
    if (bib->namu_skaicius >= 4) return 0;
    if (players[currentPlayer].money < bib->namo_kaina) return 0;

    players[currentPlayer].money -= bib->namo_kaina;
    bib->namu_skaicius++;

    show_house_bought(bib->namo_kaina, players[currentPlayer].money);
    players[currentPlayer].own[players[currentPlayer].poz_l][1] = bib->namu_skaicius;

    if (bib->namu_skaicius == 4) show_hotel_bought();
    return 1;
}