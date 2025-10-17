#include "battle.h"
#include "species.h"
#include "utils.h"
#include "items.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <math.h>
#include <stdbool.h>

#define MAX_SUPEMONS 12
#define COINS_REWARD_MIN 100
#define COINS_REWARD_MAX 500
#define EXP_REWARD_MIN   100
#define EXP_REWARD_MAX   500

static void print_supemon_brief(const Supemon *s) {
printf("%s (Niv %d) HP %d/%d | ATQ %d DEF %d EVA %d PREC %d VIT %d\n",
s->species, s->level, s->hp, s->maxHp, s->attack, s->defense, s->evasion, s->accuracy, s->speed);
}


int attempt_flee(Supemon *player, Supemon *enemy) {
double chance = (double)player->speed / (double)(player->speed + enemy->speed);
if (chance < 0) chance = 0; if (chance > 1) chance = 1;
double r = (double)rand() / (double)RAND_MAX;
return (r <= chance);
}


int attempt_capture(Supemon *enemy) {
double chance = 0.0;
if (enemy->maxHp > 0) {
chance = ((double)(enemy->maxHp - enemy->hp) / (double)enemy->maxHp) - 0.5;
}
if (chance < 0) chance = 0; if (chance > 1) chance = 1;
double r = (double)rand() / (double)RAND_MAX;
return (r <= chance);
}


int move_hits(Supemon *launcher, Supemon *target) {
double acc = (double)launcher->accuracy;
double eva = (double)target->evasion;
double hit = (acc + eva <= 0) ? 0.1 : acc / (acc + eva) + 0.1;
if (hit < 0) hit = 0; if (hit > 1) hit = 1;
double r = (double)rand() / (double)RAND_MAX;
return (r <= hit);
}


int compute_damage(Supemon *launcher, Supemon *target, int baseDamage) {
int def = target->defense; if (def <= 0) def = 1;
double raw = ((double)launcher->attack * (double)baseDamage) / (double)def;
int dmg = stochastic_round(raw); if (dmg < 0) dmg = 0; return dmg;
}

void apply_move(Supemon *launcher, Supemon *target, Move mv) {
switch (mv.type) {
case MOVE_DAMAGE: {
if (move_hits(launcher, target)) {
int dmg = compute_damage(launcher, target, mv.baseDamage);
target->hp -= dmg; if (target->hp < 0) target->hp = 0;
printf("%s utilise %s et inflige %d degats. (PV ennemi: %d/%d)\n",
launcher->species, mv.name, dmg, target->hp, target->maxHp);
} else {
printf("%s utilise %s... mais rate !\n", launcher->species, mv.name);
}
} break;
case MOVE_BUFF_SELF_ATK: {
launcher->attack += mv.statAmount;
printf("%s utilise %s ! ATQ augmente de %d (ATQ=%d)\n",
launcher->species, mv.name, mv.statAmount, launcher->attack);
} break;
case MOVE_BUFF_SELF_EVA: {
launcher->evasion += mv.statAmount;
printf("%s utilise %s ! EVA augmente de %d (EVA=%d)\n",
launcher->species, mv.name, mv.statAmount, launcher->evasion);
} break;
case MOVE_BUFF_SELF_DEF: {
launcher->defense += mv.statAmount;
printf("%s utilise %s ! DEF augmente de %d (DEF=%d)\n",
launcher->species, mv.name, mv.statAmount, launcher->defense);
} break;
default: break;
}
}


static void grant_battle_rewards(Player *p, Supemon *active, int enemyLevel) {
int coins = rand_range_int(COINS_REWARD_MIN, COINS_REWARD_MAX);
int baseExp = rand_range_int(EXP_REWARD_MIN, EXP_REWARD_MAX);
int expGain = baseExp * enemyLevel;
p->supcoins += coins;
printf("\nVictoire ! Vous gagnez %d Supcoins et %d XP pour %s.\n", coins, expGain, active->species);


active->exp += expGain;
while (active->exp >= need_exp_for_next_level(active->level)) {
active->exp -= need_exp_for_next_level(active->level);
active->level += 1;
level_up_apply(active);
printf("%s monte au niveau %d !\n", active->species, active->level);
}
}

static void choose_active_supemon(Player *p) {
if (p->teamCount == 0) return;
printf("\nChoisissez votre Supemon actif :\n");
for (int i = 0; i < p->teamCount; ++i) {
printf("%d) ", i+1); print_supemon_brief(&p->team[i]);
}
int ch = read_int("> ", 1, p->teamCount);
p->selectedIndex = ch-1;
}


static void switch_supemon(Player *p) {
if (p->teamCount <= 1) { printf("Vous n'avez pas d'autre Supemon.\n"); return; }
choose_active_supemon(p);
}


void battle(Player *p) {
if (p->teamCount == 0) { printf("Vous n'avez aucun Supemon pour combattre.\n"); return; }
Supemon *active = &p->team[p->selectedIndex];
if (active->hp <= 0) { printf("Votre Supemon actif est K.O., changez-en d'abord.\n"); return; }


Supemon enemy = random_enemy_same_level(active->level);
printf("\nUn %s sauvage apparait !\n", enemy.species);


reset_combat_stats(active);
reset_combat_stats(&enemy);


int itemsUsed = 0;

while (active->hp > 0 && enemy.hp > 0) {
printf("\n--- Etat ---\n");
printf("Allie : "); print_supemon_brief(active);
printf("Ennemi : "); print_supemon_brief(&enemy);


printf("\nVos actions :\n");
printf("1) Attaquer / Move\n2) Changer de Supemon\n3) Utiliser un objet (%d/%d utilises)\n4) Fuir\n5) Capturer\n", itemsUsed, MAX_ITEMS_PER_BATTLE);
int ch = read_int("> ", 1, 5);


int playerFirst = 0;
if (active->speed > enemy.speed) playerFirst = 1;
else if (active->speed < enemy.speed) playerFirst = 0;
else playerFirst = (rand()%2);


int playerSkipped = 0;


for (int step = 0; step < 2; ++step) {
int isPlayerTurn = (step == 0) ? playerFirst : !playerFirst;
if (isPlayerTurn) {
if (ch == 1) {
printf("\nChoisissez un move :\n");
for (int i = 0; i < active->movesCount; ++i) {
Move mv = active->moves[i];
if (mv.type == MOVE_DAMAGE) printf("%d) %s (Degats base %d)\n", i+1, mv.name, mv.baseDamage);
else printf("%d) %s (+%d stat)\n", i+1, mv.name, mv.statAmount);
}
int m = read_int("> ", 1, active->movesCount);
apply_move(active, &enemy, active->moves[m-1]);
} else if (ch == 2) {
switch_supemon(p);
active = &p->team[p->selectedIndex];
playerSkipped = 1;
} else if (ch == 3) {
use_item_in_battle(p, active, &itemsUsed);
} else if (ch == 4) {
if (attempt_flee(active, &enemy)) { printf("Vous prenez la fuite !\n"); return; }
else printf("La fuite echoue !\n");
} else if (ch == 5) {
if (attempt_capture(&enemy)) {
printf("Vous capturez %s !\n", enemy.species);
if (p->teamCount < MAX_SUPEMONS) { p->team[p->teamCount++] = enemy; }
else printf("Votre equipe est pleine, capture perdue…\n");
return;
} else { printf("Echec de la capture !\n"); }
}
} else {
if (enemy.hp > 0 && active->hp > 0) {
int m = rand_range_int(1, enemy.movesCount) - 1;
apply_move(&enemy, active, enemy.moves[m]);
}
}
if (active->hp <= 0 || enemy.hp <= 0) break;
(void)playerSkipped; // ici: on laisse l'ennemi jouer quand même
}
}


if (active->hp <= 0) { printf("\nVotre %s est K.O. ! Defaite…\n", active->species); return; }
else if (enemy.hp <= 0) { grant_battle_rewards(p, active, enemy.level); }
}
