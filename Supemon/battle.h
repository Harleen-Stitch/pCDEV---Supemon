#ifndef BATTLE_H
#define BATTLE_H

#include "player.h"
#include "supemon.h"
#include "exp_lvl.h"
#include "items.h"

// ------------------------------------------------------
// Lance un combat entre le Supemon actif du joueur
// et un Supemon sauvage genere aleatoirement
// ------------------------------------------------------
void battle_start(Player *player);

// ------------------------------------------------------
// Gere un tour de combat entre deux Supemons.
// - attacker : Supemon qui agit
// - defender : cible du tour
// - isPlayer : 1 si le lanceur est le joueur, 0 si c est un ennemi
// ------------------------------------------------------
void battle_turn(Supemon *attacker, Supemon *defender, int isPlayer);

#endif // BATTLE_H
