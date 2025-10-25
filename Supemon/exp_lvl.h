#ifndef EXP_LVL_H
#define EXP_LVL_H

#include "supemon.h"
#include "formule.h"

#define EXP_BASE 500
#define EXP_INCREMENT 1000


void exp_vic(Supemon *supemon, unsigned short exp_rand, unsigned short enemy_lvl);

unsigned short exp_for_next_lvl(const Supemon *supemon);

void lvl_up(Supemon *supemon);

#endif // EXP_LVL_H
