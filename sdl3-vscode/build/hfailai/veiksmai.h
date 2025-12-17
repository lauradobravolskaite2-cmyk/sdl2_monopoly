#ifndef VEIKSMAI_H
#define VEIKSMAI_H

#include "zaidejai.h"

typedef struct {
    char pavadinimas[100];
    int kaina;
    int nuoma_be_namo;
    int nuoma_su_1_namu;
    int nuoma_su_2_namais;
    int nuoma_su_3_namais;
    int nuoma_su_viesbuciu;
    int namo_kaina;
    int savininko_id;
    int namu_skaicius;
} Biblioteka;

typedef struct {
    char tekstas[250];
    int baitu_pokytis;
    int pozicijos_pokytis;
    int eiti_i_kalejima;
    int iseiti_is_kalejimo;
} Kortele;

extern Biblioteka *bibliotekos[44];   // CHANGED: global shared
extern int zaideju_kiekis;

void nuskaityti_bibliotekas(void);
Biblioteka *gauti_biblioteka_pagal_pozicija(int pozicija);

int kauliukas(void);
int atlik_ejima(player *players, int currentPlayer, int mas[], int sk); // CHANGED

int moketi_nuoma(player *players, int currentPlayer, int zaideju_kiekis);
int pirkti_nama(player *players, int currentPlayer, int pirkti);

#endif