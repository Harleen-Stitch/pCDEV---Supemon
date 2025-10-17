//backup
#include "save.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <math.h>
#include <stdbool.h>

int save_game(const Player *p, const char *filename) {
FILE *f = fopen(filename, "w");
if (!f) { perror("fopen"); return 0; }


fprintf(f, "VERSION 1\n"); // pour l'évolutivité future
fprintf(f, "NAME %s\n", p->name);
fprintf(f, "COINS %d\n", p->supcoins);
fprintf(f, "SELECTED %d\n", p->selectedIndex);


// Items
fprintf(f, "ITEMS %d\n", p->itemsCount);
for (int i = 0; i < p->itemsCount; ++i) {
fprintf(f, "ITEM %d %d\n", (int)p->items[i].type, p->items[i].qty);
}


// Team
fprintf(f, "TEAM %d\n", p->teamCount);
for (int i = 0; i < p->teamCount; ++i) {
const Supemon *s = &p->team[i];
fprintf(f, "S %s\n", s->species);
fprintf(f, "L %d\n", s->level);
fprintf(f, "E %d\n", s->exp);
fprintf(f, "HP %d %d\n", s->hp, s->maxHp);
fprintf(f, "ATK %d %d\n", s->attack, s->baseAttack);
fprintf(f, "DEF %d %d\n", s->defense, s->baseDefense);
fprintf(f, "EVA %d %d\n", s->evasion, s->baseEvasion);
fprintf(f, "ACC %d %d\n", s->accuracy, s->baseAccuracy);
fprintf(f, "SPD %d\n", s->speed);
fprintf(f, "MOV %d\n", s->movesCount);
for (int m = 0; m < s->movesCount; ++m) {
Move mv = s->moves[m];
fprintf(f, "M %d %d %d %s\n", (int)mv.type, mv.baseDamage, mv.statAmount, mv.name);
}
fprintf(f, "END\n");
}


fclose(f);
return 1;
}


int load_game(Player *p, const char *filename) {
FILE *f = fopen(filename, "r");
if (!f) return 0;
memset(p, 0, sizeof(*p));


char key[64];
int version = 0;

while (fscanf(f, "%63s", key) == 1) {
if (strcmp(key, "VERSION") == 0) {
fscanf(f, "%d", &version);
} else if (strcmp(key, "NAME") == 0) {
fscanf(f, " %31[^\n]", p->name);
} else if (strcmp(key, "COINS") == 0) {
fscanf(f, "%d", &p->supcoins);
} else if (strcmp(key, "SELECTED") == 0) {
fscanf(f, "%d", &p->selectedIndex);
} else if (strcmp(key, "ITEMS") == 0) {
fscanf(f, "%d", &p->itemsCount);
for (int i = 0; i < p->itemsCount; ++i) {
int t, q; fscanf(f, "%63s", key);
if (strcmp(key, "ITEM") != 0) { fclose(f); return 0; }
fscanf(f, "%d %d", &t, &q);
p->items[i].type = (ItemType)t;
p->items[i].qty = q;
}
} else if (strcmp(key, "TEAM") == 0) {
fscanf(f, "%d", &p->teamCount);
for (int i = 0; i < p->teamCount; ++i) {
Supemon s; memset(&s, 0, sizeof(s));
while (fscanf(f, "%63s", key) == 1) {
if (strcmp(key, "S") == 0) {
fscanf(f, " %23[^\n]", s.species);
} else if (strcmp(key, "L") == 0) {
fscanf(f, "%d", &s.level);
} else if (strcmp(key, "E") == 0) {
fscanf(f, "%d", &s.exp);
} else if (strcmp(key, "HP") == 0) {
fscanf(f, "%d %d", &s.hp, &s.maxHp);
} else if (strcmp(key, "ATK") == 0) {
fscanf(f, "%d %d", &s.attack, &s.baseAttack);
} else if (strcmp(key, "DEF") == 0) {
fscanf(f, "%d %d", &s.defense, &s.baseDefense);
} else if (strcmp(key, "EVA") == 0) {
fscanf(f, "%d %d", &s.evasion, &s.baseEvasion);
} else if (strcmp(key, "ACC") == 0) {
fscanf(f, "%d %d", &s.accuracy, &s.baseAccuracy);
} else if (strcmp(key, "SPD") == 0) {
fscanf(f, "%d", &s.speed);
} else if (strcmp(key, "MOV") == 0) {
fscanf(f, "%d", &s.movesCount);
} else if (strcmp(key, "M") == 0) {
int t, bd, sa; char nm[24];
fscanf(f, "%d %d %d %23[^\n]", &t, &bd, &sa, nm);
Move mv; memset(&mv, 0, sizeof(mv));
mv.type = (MoveType)t; mv.baseDamage = bd; mv.statAmount = sa;
strncpy(mv.name, nm, sizeof(mv.name)-1);
s.moves[s.movesCount++] = mv;
} else if (strcmp(key, "END") == 0) {
break;
}
}
p->team[i] = s;
}
}
}
fclose(f);
if (p->selectedIndex < 0 || p->selectedIndex >= p->teamCount) p->selectedIndex = 0;
(void)version; // Version pas encore utilisée mais prête pour évoluer
return 1;
}
