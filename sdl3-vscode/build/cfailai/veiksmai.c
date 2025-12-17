#include <stdlib.h>
#include <string.h>
#include "veiksmai.h"
#include "render.h"
#include <SDL_ttf.h>

int kauliukas(void)
{
    return (rand() % 6 + 1) + (rand() % 6 + 1);
}

// standard tile: rent payment (bankrupt -> -1)
int atlik_ejima(player *players, int currentPlayer, int mas[], int sk) // CHANGED
{
    (void)mas;
    int r = moketi_nuoma(players, currentPlayer, sk);
    if (r == -1) return -1;
    return 0;
}