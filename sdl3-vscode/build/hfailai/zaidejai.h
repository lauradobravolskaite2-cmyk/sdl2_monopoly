#ifndef ZAIDEJAI_H
#define ZAIDEJAI_H

typedef struct
{
    int id;
    int money;
    int poz_l;
    int own[44][2];
    int kalejimo_laikas;
    int turi_iseiti_is_kalejimo_korta;
    int kartai_ismestas_dublikatas;
} player;

void zaideju_duomenys(int ats, int sk);

#endif
