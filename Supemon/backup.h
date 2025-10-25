#ifndef BACKUP_H
#define BACKUP_H

#include "player.h"

#define SAVE_FILENAME "./save.bin"   // <= IMPORTANT : une chaîne non vide !

int save_game(const Player *player, const char *filename);
int load_game(Player *player, const char *filename);

#endif

