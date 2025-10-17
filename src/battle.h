#ifndef BATTLE_H
#define BATTLE_H


#include "types.h"


int attempt_flee(Supemon *player, Supemon *enemy);
int attempt_capture(Supemon *enemy);
int move_hits(Supemon *launcher, Supemon *target);
int compute_damage(Supemon *launcher, Supemon *target, int baseDamage);
void apply_move(Supemon *launcher, Supemon *target, Move mv);
void battle(Player *p);


#endif // BATTLE_H