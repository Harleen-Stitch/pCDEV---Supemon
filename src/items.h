#ifndef ITEMS_H
#define ITEMS_H


#include "types.h"


const char* item_name(ItemType t);
int item_price(ItemType t);
int add_item(Player *p, ItemType t, int qty);
int remove_item(Player *p, ItemType t, int qty);
void show_items(const Player *p);
void give_starter_items(Player *p);
void use_item_in_battle(Player *p, Supemon *target, int *itemsUsedThisBattle);
void shop(Player *p);


#endif // ITEMS_H