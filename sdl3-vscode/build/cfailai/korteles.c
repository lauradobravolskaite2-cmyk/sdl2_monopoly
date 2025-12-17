#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "korteles.h"

Kortele *rand_korteles = NULL;
int rand_korteliu_kiekis = 0;
Kortele *system_logs_korteles = NULL;
int system_logs_korteliu_kiekis = 0;

static void shuffle(Kortele *k, int n)
{
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Kortele tmp = k[i]; k[i] = k[j]; k[j] = tmp;
    }
}

static void parse_line(Kortele *out, char *line)
{
    memset(out, 0, sizeof(*out));
    char *t = strtok(line, ",");
    if (t) strncpy(out->tekstas, t, sizeof(out->tekstas)-1);
    t = strtok(NULL, ","); out->baitu_pokytis = t ? atoi(t) : 0;
    t = strtok(NULL, ","); out->pozicijos_pokytis = t ? atoi(t) : 0;
    t = strtok(NULL, ","); out->eiti_i_kalejima = t ? (atoi(t)==1) : 0;
    t = strtok(NULL, "\n"); out->iseiti_is_kalejimo = t ? (atoi(t)==1) : 0;
}

void nuskaityti_rand_korteles(void)
{
    FILE *f = fopen("rand.txt", "r");
    if (!f) return;
    char line[512];
    Kortele *temp = NULL;
    int n = 0;

    while (fgets(line, sizeof(line), f)) {
        Kortele *tmp2 = realloc(temp, (n+1)*sizeof(Kortele));
        if (!tmp2) { free(temp); fclose(f); return; }
        temp = tmp2;
        parse_line(&temp[n], line);
        n++;
    }
    fclose(f);

    free(rand_korteles);
    rand_korteles = temp;
    rand_korteliu_kiekis = n;
    shuffle(rand_korteles, rand_korteliu_kiekis);
}

void nuskaityti_system_logs_korteles(void)
{
    FILE *f = fopen("systemlogs.txt", "r");
    if (!f) return;
    char line[512];
    Kortele *temp = NULL;
    int n = 0;

    while (fgets(line, sizeof(line), f)) {
        Kortele *tmp2 = realloc(temp, (n+1)*sizeof(Kortele));
        if (!tmp2) { free(temp); fclose(f); return; }
        temp = tmp2;
        parse_line(&temp[n], line);
        n++;
    }
    fclose(f);

    free(system_logs_korteles);
    system_logs_korteles = temp;
    system_logs_korteliu_kiekis = n;
    shuffle(system_logs_korteles, system_logs_korteliu_kiekis);
}

Kortele traukti_rand_kortele(void)
{
    if (rand_korteliu_kiekis <= 0) return (Kortele){0};
    Kortele k = rand_korteles[0];
    for (int i=0;i<rand_korteliu_kiekis-1;i++) rand_korteles[i]=rand_korteles[i+1];
    rand_korteles[rand_korteliu_kiekis-1]=k;
    return k;
}

Kortele traukti_system_logs_kortele(void)
{
    if (system_logs_korteliu_kiekis <= 0) return (Kortele){0};
    Kortele k = system_logs_korteles[0];
    for (int i=0;i<system_logs_korteliu_kiekis-1;i++) system_logs_korteles[i]=system_logs_korteles[i+1];
    system_logs_korteles[system_logs_korteliu_kiekis-1]=k;
    return k;
}

void apdoroti_kortele(player *players, int currentPlayer, int tipas)
{
    Kortele k = (tipas==RAND_KORTA) ? traukti_rand_kortele() : traukti_system_logs_kortele();
    players[currentPlayer].money += k.baitu_pokytis;
    players[currentPlayer].poz_l += k.pozicijos_pokytis;
    if (players[currentPlayer].poz_l > 43) { players[currentPlayer].poz_l %= 44; players[currentPlayer].money += 200; }
    if (players[currentPlayer].poz_l < 0) players[currentPlayer].poz_l = 0;
    if (k.iseiti_is_kalejimo) players[currentPlayer].turi_iseiti_is_kalejimo_korta++;
}
