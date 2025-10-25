// shop.c
#include <stdio.h>
#include <string.h>
#include "player.h"
#include "items.h"


//extern utilise un objet deja cree ailleurs, a ne pas recreer de copie
extern Item potion;
extern Item superPotion;
extern Item bonbonExtra;

//nettoyage de la memoire
static void flush_input(void) {
    int c; while ((c = getchar()) != '\n' && c != EOF) {}
}


static unsigned char read_choice(void) {
    unsigned int tmp = 0;
    if (scanf("%u", &tmp) != 1) { flush_input(); return 255; }
    flush_input();
    if (tmp > 255) return 255;
    return (unsigned char)tmp;
}

// tableau global des objets disponibles dans la boutique
Item* shop_items[] = { &potion, &superPotion, &bonbonExtra };
const unsigned short SHOP_ITEM_COUNT =
    sizeof(shop_items) / sizeof(shop_items[0]);

//==================================================
// === AFFICHAGE PRINCIPAL DE LA BOUTIQUE ===
//==================================================
void shop_menu(Player *player) {
    unsigned char ch = 0;

    do {
        puts("\n\t=== BIENVENUE DANS LA BOUTIQUE ===");
        puts("\n\t1) Acheter");
        puts("\t2) Vendre");
        puts("\t0) Quitter");
        printf("\n\tQue choisissez-vous ? ");

        ch = read_choice();

        switch (ch) {
            case 1:
                shop_buy(player);
                break;
            case 2:
                shop_sell(player);
                break;
            case 0:
                puts("\tMerci de votre visite et a bientot...");
                break; // sort de la boucle do/while => retour au main_menu
            default:
                puts("\tChoix invalide !");
        }
    } while (ch != 0);
}
//========================================
//========== BOUTIQUE  : ACHETER  ========
//========================================
void shop_buy(Player *player) {
    unsigned char ch = 0;

    do {
        puts("\n\t==== Boutique : Acheter ====");
        printf("\n\tSupcoins disponibles : %hu\n\n", player->supcoins);

        for (unsigned short i = 0; i < SHOP_ITEM_COUNT; i++) {
            printf("\t%hu) %-15s | %-4hu supcoins | Qte inventaire : %hu | %s\n",
                   (unsigned short)(i + 1),
                   shop_items[i]->item_name,
                   shop_items[i]->price_buy,
                   player->inventory[i].quantity,
                   shop_items[i]->item_desc)
                   ;
        }
        puts("\t0) Retour");
        printf("\n\tVotre choix : ");

        ch = read_choice();

        if (ch == 0) return;

        if (ch > 0 && ch <= SHOP_ITEM_COUNT){
            Item *chosen = shop_items[ch - 1];

            if (player->supcoins < chosen->price_buy) {
                puts("\tPas assez de supcoins !");
                continue;
            }

            // retire le prix
            player->supcoins -= chosen->price_buy;

            // cherche si l objet existe deja dans l inventaire
            unsigned char found = 0;
            for (unsigned short i = 0; i < player->itemsCount; i++) {
                if (strcmp(player->inventory[i].item.item_name,
                           chosen->item_name) == 0) {
                    // augmente la quantite
                    player->inventory[i].quantity++;
                    found = 1;
                    break;
                }
            }

            // si l objet n etait pas encore dans l inventaire, on le cree
            if (!found && player->itemsCount < MAX_ITEMS) {
                unsigned short idx = player->itemsCount++;
                player->inventory[idx].item = *chosen;  // copie des infos
                player->inventory[idx].quantity = 1;
            }

            printf("\tVous avez achete 1 %s !\n", chosen->item_name);
        } else {
            puts("\tChoix invalide !");
        }

    } while (ch != 0);
}

//========================================
//========== BOUTIQUE : VENDRE ==========
//========================================
void shop_sell(Player *player) {
    unsigned char ch = 0;

    do {
        puts("\n\t==== Boutique : Vendre ====");
        if (player->itemsCount == 0) {
            puts("\n\tVous n'avez rien a vendre !");
            return;
        }

        printf("\n\tSupcoins : %hu\n\n", player->supcoins);

        for (unsigned short i = 0; i < player->itemsCount; i++) {
            printf("\t%hu) %-15s | Qte : %hu  | Prix de vente : %hu\n",
                   (unsigned short)(i + 1),
                   player->inventory[i].item.item_name,
                   player->inventory[i].quantity,
                   player->inventory[i].item.price_sell);
        }
        puts("\t0) Retour");
        printf("\n\tVotre choix : ");

        ch = read_choice();

        if (ch == 0) return;

        if (ch > 0 && ch <= player->itemsCount) {
            InventorySlot *slot = &player->inventory[ch - 1];

            // ajoute les supcoins au joueur
            player->supcoins += slot->item.price_sell;
            slot->quantity--;

            printf("\tVous avez vendu 1 %s !\n", slot->item.item_name);

            // si quantite = 0, on retire l objet de l inventaire
            if (slot->quantity == 0) {
                for (unsigned short j = ch - 1; j < player->itemsCount - 1; j++) {
                    player->inventory[j] = player->inventory[j + 1];
                }
                player->itemsCount--;
            }
        } else {
            puts("\tChoix invalide !");
        }

    } while (ch != 0);
}
