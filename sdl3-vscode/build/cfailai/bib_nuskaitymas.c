#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "veiksmai.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

int zaideju_kiekis = 0;
Biblioteka *bibliotekos[44] = {NULL};

void nuskaityti_bibliotekas(void)
{
    FILE *f = fopen("bibliotekos.txt", "r");
    if (!f) return;

    for (int i = 0; i < 44; i++) {
        free(bibliotekos[i]);
        bibliotekos[i] = NULL;
    }

    int bib_poz[] = {1,2,3,5,7,8,12,14,16,18,19,21,23,25,27,30,31,32,34,35,38,41,43};
    char line[512];
    int idx = 0;

    while (idx < 23 && fgets(line, sizeof(line), f))
    {
        int pos = bib_poz[idx];
        bibliotekos[pos] = (Biblioteka*)calloc(1, sizeof(Biblioteka));
        if (!bibliotekos[pos]) {
            fclose(f);
            return;
        }

        char *t = strtok(line, ",");
        if (t)
            strncpy(bibliotekos[pos]->pavadinimas, t,
                    sizeof(bibliotekos[pos]->pavadinimas) - 1);

        t = strtok(NULL, ","); bibliotekos[pos]->kaina = t ? atoi(t) : 0;
        t = strtok(NULL, ","); bibliotekos[pos]->nuoma_be_namo = t ? atoi(t) : 0;
        t = strtok(NULL, ","); bibliotekos[pos]->nuoma_su_1_namu = t ? atoi(t) : 0;
        t = strtok(NULL, ","); bibliotekos[pos]->nuoma_su_2_namais = t ? atoi(t) : 0;
        t = strtok(NULL, ","); bibliotekos[pos]->nuoma_su_3_namais = t ? atoi(t) : 0;
        t = strtok(NULL, ","); bibliotekos[pos]->nuoma_su_viesbuciu = t ? atoi(t) : 0;
        t = strtok(NULL, "\n"); bibliotekos[pos]->namo_kaina = t ? atoi(t) : 0;

        bibliotekos[pos]->savininko_id = -1;
        bibliotekos[pos]->namu_skaicius = 0;

        idx++;
    }

    fclose(f);
}

Biblioteka *gauti_biblioteka_pagal_pozicija(int pozicija)
{
    if (pozicija < 0 || pozicija >= 44)
        return NULL;

    return bibliotekos[pozicija];
}

