#include "menu.h"
#include "battle.h"
#include "items.h"
#include "player.h"
#include "save.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <math.h>
#include <stdbool.h>
#define SAVE_FILENAME "save_supemon.txt"

void main_menu(Player *p) {
for (;;) {
printf("\n===== MENU PRINCIPAL =====\n");
printf("Joueur : %s | Supcoins : %d\n", p->name, p->supcoins);
if (p->teamCount>0) {
const Supemon *a = &p->team[p->selectedIndex];
printf("Supemon actif : %s (Niv %d) %d/%d PV\n", a->species, a->level, a->hp, a->maxHp);
}
printf("--------------------------\n");
printf("1) Aller dans la nature (combat)\n");
printf("2) Boutique\n");
printf("3) Centre Supemon (soin)\n");
printf("4) Inventaire\n");
printf("5) Equipe / Changer d'actif\n");
printf("6) Sauvegarder\n");
printf("0) Quitter\n");


int ch = read_int("> ", 0, 6);
switch (ch) {
case 1: battle(p); press_enter_to_continue(); break;
case 2: shop(p); break;
case 3: pokemon_center(p); press_enter_to_continue(); break;
case 4: show_items(p); press_enter_to_continue(); break;
case 5: list_team(p); {
if (p->teamCount>0) {
int idx = read_int("Choisir l'actif (1..n) : ", 1, p->teamCount);
p->selectedIndex = idx-1;
}
} break;
case 6: if (save_game(p, SAVE_FILENAME)) printf("Sauvegarde réussie.\n"); else printf("Echec de la sauvegarde.\n"); break;
case 0: {
if (read_yesno("Voulez-vous sauvegarder avant de quitter ? (o/n) ")=='o') {
if (save_game(p, SAVE_FILENAME)) printf("Sauvegarde reussie.\n"); else printf("Echec de la sauvegarde.\n");
}
printf("\nMerci d'avoir joue a Supemon !\n");
return;
}
default: printf("Choix invalide.\n"); break;
}
}
}
