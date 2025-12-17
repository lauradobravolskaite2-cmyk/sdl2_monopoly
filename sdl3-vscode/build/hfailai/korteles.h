#ifndef KORTELE_H
#define KORTELE_H

#include "veiksmai.h"

#define RAND_KORTA 1
#define SYSTEM_LOGS_KORTA 2

void nuskaityti_rand_korteles(void);
void nuskaityti_system_logs_korteles(void);
Kortele traukti_rand_kortele(void);
Kortele traukti_system_logs_kortele(void);
void apdoroti_kortele(player *players, int currentPlayer, int korteles_tipas);

#endif
