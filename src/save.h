#ifndef SAVE_H
#define SAVE_H


#include "types.h"


int save_game(const Player *p, const char *filename);
int load_game(Player *p, const char *filename);


#endif // SAVE_H