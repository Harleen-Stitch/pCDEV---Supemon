// SHOP;H
#ifndef SHOP_H
#define SHOP_H

#include "items.h"
#include "player.h"

//appelle le menu boutique
void shop_menu(Player *player);
//appelle le menu achat
void shop_buy(Player *player);
//appelle le menu vente
void shop_sell(Player *player);

#endif
