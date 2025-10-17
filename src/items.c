#include "items.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <math.h>
#include <stdbool.h>
#define MAX_ITEMS    32


const char* item_name(ItemType t) {
switch (t) {
case ITEM_POTION: return "Potion (+5 PV)";
case ITEM_SUPER_POTION: return "Super potion (+10 PV)";
case ITEM_RARE_CANDY: return "Bonbon rare (+1 niveau)";
default: return "Inconnu";
}
}


int item_price(ItemType t) {
switch (t) {
case ITEM_POTION: return PRICE_POTION;
case ITEM_SUPER_POTION: return PRICE_SUPERPOTION;
case ITEM_RARE_CANDY: return PRICE_RARE_CANDY;
default: return 0;
}
}


int add_item(Player *p, ItemType t, int qty) {
if (qty <= 0) return 0;
for (int i = 0; i < p->itemsCount; ++i) {
if (p->items[i].type == t) { p->items[i].qty += qty; return 1; }
}
if (p->itemsCount >= MAX_ITEMS) return 0;
p->items[p->itemsCount].type = t;
p->items[p->itemsCount].qty = qty;
p->itemsCount++;
return 1;
}


int remove_item(Player *p, ItemType t, int qty) {
if (qty <= 0) return 0;
for (int i = 0; i < p->itemsCount; ++i) {
if (p->items[i].type == t) {
if (p->items[i].qty < qty) return 0;
p->items[i].qty -= qty;
if (p->items[i].qty == 0) {
for (int j = i; j < p->itemsCount-1; ++j) p->items[j] = p->items[j+1];
p->itemsCount--;
}
return 1;
}
}
return 0;
}

void show_items(const Player *p) {
printf("\nVos objets :\n");
if (p->itemsCount == 0) { printf(" (aucun)\n"); return; }
for (int i = 0; i < p->itemsCount; ++i) {
printf(" %d) %s x%d\n", i+1, item_name(p->items[i].type), p->items[i].qty);
}
}


void give_starter_items(Player *p) {
add_item(p, ITEM_POTION, 2);
add_item(p, ITEM_SUPER_POTION, 1);
}


void use_item_in_battle(Player *p, Supemon *target, int *itemsUsedThisBattle) {
if (*itemsUsedThisBattle >= MAX_ITEMS_PER_BATTLE) {
printf("Vous avez deja utilise %d objets dans ce combat.\n", MAX_ITEMS_PER_BATTLE);
return;
}
if (p->itemsCount == 0) { printf("Vous n'avez aucun objet.\n"); return; }


show_items(p);
int ch = read_int("Choisissez un objet à utiliser (0 pour annuler) : ", 0, p->itemsCount);
if (ch == 0) return;
ItemStack *st = &p->items[ch-1];
if (st->qty <= 0) return;


if (st->type == ITEM_POTION) {
int before = target->hp;
target->hp = clampi(target->hp + 5, 0, target->maxHp);
printf("Potion utilisee sur %s : +%d PV (%d/%d)\n", target->species, target->hp - before, target->hp, target->maxHp);
(*itemsUsedThisBattle)++;
remove_item(p, st->type, 1);
} else if (st->type == ITEM_SUPER_POTION) {
int before = target->hp;
target->hp = clampi(target->hp + 10, 0, target->maxHp);
printf("Super potion utilisee sur %s : +%d PV (%d/%d)\n", target->species, target->hp - before, target->hp, target->maxHp);
(*itemsUsedThisBattle)++;
remove_item(p, st->type, 1);
} else if (st->type == ITEM_RARE_CANDY) {
target->level += 1;
level_up_apply(target);
printf("Bonbon rare : %s passe niveau %d ! (PV max=%d, ATQ=%d, DEF=%d, EVA=%d, PREC=%d, VIT=%d)\n",
target->species, target->level, target->maxHp, target->attack, target->defense, target->evasion, target->accuracy, target->speed);
(*itemsUsedThisBattle)++;
remove_item(p, st->type, 1);
}
}

void shop(Player *p) {
for (;;) {
printf("\n--- Boutique ---\n");
printf("Supcoins : %d\n", p->supcoins);
printf("1) Acheter\n2) Vendre\n0) Quitter la boutique\n");
int ch = read_int("> ", 0, 2);
if (ch == 0) break;
else if (ch == 1) {
printf("\nArticles disponibles :\n");
printf("1) %s - %d Supcoins\n", item_name(ITEM_POTION), item_price(ITEM_POTION));
printf("2) %s - %d Supcoins\n", item_name(ITEM_SUPER_POTION), item_price(ITEM_SUPER_POTION));
printf("3) %s - %d Supcoins\n", item_name(ITEM_RARE_CANDY), item_price(ITEM_RARE_CANDY));
int a = read_int("0) Annuler\n> ", 0, 3);
if (a == 0) continue;
ItemType t = (a==1)?ITEM_POTION: (a==2)?ITEM_SUPER_POTION: ITEM_RARE_CANDY;
int q = read_int("Quantité : ", 1, 1000000);
int cost = item_price(t) * q;
if (p->supcoins < cost) printf("Pas assez de Supcoins.\n");
else { p->supcoins -= cost; add_item(p, t, q); printf("Achat réussi !\n"); }
} else if (ch == 2) {
if (p->itemsCount == 0) { printf("Vous n'avez rien a vendre.\n"); continue; }
show_items(p);
int i = read_int("Choisir l'objet à vendre (0 pour annuler) : ", 0, p->itemsCount);
if (i == 0) continue;
ItemStack st = p->items[i-1];
int q = read_int("Quantite : ", 1, st.qty);
int gain = (item_price(st.type) / 2) * q;
p->supcoins += gain;
remove_item(p, st.type, q);
printf("Vendu ! +%d Supcoins\n", gain);
}
}
}
