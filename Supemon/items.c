// items.c — implémentation des fonctions liées aux objets

#include <stdio.h>      // pour printf
#include <string.h>     // pour strncpy
#include "items.h"


    // pour la structure Item et les constantes

// === déclaration des objets globaux ===
Item potion;
Item superPotion;
Item bonbonExtra;

// Initialise un objet
void init_item(Item *item,
               const unsigned char *item_name,
               const unsigned char *item_desc,
               unsigned short price_buy,
               unsigned short price_sell,
               unsigned short heal,
               unsigned char lvl_up) {

    // copie le nom de l'objet dans la structure
    strncpy(item->item_name, item_name, ITEM_NAME_MAX);
    // force le caractère de fin de chaîne
    item->item_name[ITEM_NAME_MAX - 1] = '\0';

    // copie la description dans la structure
    strncpy(item->item_desc, item_desc, ITEM_DESC_MAX);
    item->item_desc[ITEM_DESC_MAX - 1] = '\0';

    // initialise les valeurs numériques
    item->price_buy = price_buy;        // prix de l’objet
    item->price_sell = price_sell;
    item->heal = heal;                  // points de vie rendus
    item->lvl_up = lvl_up;              // 1 si objet donne un niveau, sinon 0
}

//initialise tous les objets du prog
void init_all_items(void){
    init_item(&potion, "Potion", "Rend 5 PV", 100, 50, 5, 0);
    init_item(&superPotion, "Super Potion", "Rend 10 PV", 300, 150, 10, 0);
    init_item(&bonbonExtra, "Bonbon Extra", "Monte d'un niveau", 700, 350, 0, 1);
}


// Affiche un objet
void display_item(const Item *item) {
    // Affiche les infos de façon claire et alignée
    printf("\tNom : %-20s | Prix : %-4hu | Soin : %-3hu | MonteNiv : %-1hhu\n %s\n",
           item->item_name,
           item->price_buy,
           item->price_sell,
           item->heal,
           item->lvl_up,
           item->item_desc);
}

