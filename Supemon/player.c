//player.c
#include <stdio.h>  //printf
#include <string.h> //%s
#include "player.h"



// --- Helpers locaux d'entree (securises) ---
static void flush_input_local(void) {
    int c; while ((c = getchar()) != '\n' && c != EOF) {}
}

static unsigned char read_choice_local(void) {
    unsigned int tmp = 0;
    if (scanf("%u", &tmp) != 1) { flush_input_local(); return 255; }
    flush_input_local();
    if (tmp > 255) return 255;
    return (unsigned char)tmp;
}

//==================================================
//========== CREATION D UNE PARTIE ==========
//========== CREATION DU JOUEUR ==========
//==================================================

//ne retourne rien mais prends les infos et les stockent
void new_game(Player *player) {
    puts("\n\tEntrez votre nom : ");
    scanf("%49s", player->name);
    player->supcoins = 200; //valeur arbitraire que je definis pour le debut de la partie
    player->supemonsCount = 0; //debute sans supemon
    player->itemsCount = 0;
    player->selectedIndex = NO_ACTIVE_SUPEMON; //aucun actif au depart
    player->selectedSupemon = NULL;

    //initialise l'inventaire
    for (unsigned short i = 0; i < MAX_ITEMS; i++){
        player->inventory[i].quantity = 0;      //accede a un champs donc .qty
    }
        printf("\n\tBienvenue a toi %s !\n",player->name);

 //==================================================
 //========== CHOIX DU SUPEMON DE DEPART ==========
 //==================================================
    puts("\n\tChoisis ton Supemon de depart !\n");
    puts("\t1) Supmander");
    puts("\t2) Supasaur");
    puts("\t3) Supirtle");

    unsigned char ch = 0;
    do {
        printf("\tTon choix (1-3) : ");
        scanf("%hhu", &ch);

        if (ch == 1) {
            player->supemons[0] = create_supmander();
        } else if (ch == 2) {
            player->supemons[0] = create_supasaur();
        } else if (ch == 3) {
            player->supemons[0] = create_supirtle();
        } else {
            puts("\tChoix invalide !");
            ch = 0; // force a redemander
        }
    } while (ch == 0);

    player->supemonsCount = 1;
    player->selectedIndex = 0;
    player->selectedSupemon = &player->supemons[0];

    printf("\nTu commences ton aventure avec %s !\n",
           player->selectedSupemon->species
           );
    display_player(player);
}

//========================================
//========== CREATION DU JOUEUR ==========
//========================================

//ne retourne rien mais affiche le joueur
//definit dans player.h
void display_player(const Player *player) {

    printf("\tNom : %s", player->name);
    printf("\tSupcoins : %hu\n", player->supcoins);

    if (player->supemonsCount == 0) {
        puts("\tAucun Supemon pour le moment...");
        return;
    }

    if (player->selectedSupemon != NULL) {
        printf("\tCompagnon : %-12s | NIV %-3hu | HP %-3hu/%3hu\n",
               player->selectedSupemon->species,
               player->selectedSupemon->lvl,
               player->selectedSupemon->hp,
               player->selectedSupemon->maxHp);
    } else {
        puts("\tAucun Supemon actif pour le moment !");
    }
}

//utilisation d'item qui change les stat player
void use_item(Player *player, unsigned char index) {
    InventorySlot *slot = &player->inventory[index];


    if (slot->quantity == 0) {
        puts("Tu n as plus cet objet !");
        return;
    }

    Item *item= &slot->item;
    //si c'est une potion, j'appelle la fct dans supemon
    if (item->heal >0 && player->selectedSupemon != NULL){
        heal_supemon(player->selectedSupemon, item->heal); //par de la fct
    }
    if (item->lvl_up && player->selectedSupemon != NULL){
        lvl_up(player->selectedSupemon);
    }

    slot->quantity--;
}

//========================================
//========== MENU INVENTAIRE ==========
//========================================
void open_inventory(Player *player) {
    if (player->itemsCount == 0) {
        puts("\n\tVotre inventaire est vide.");
        return; // retour au menu principal
    }

//comme tous les objets s appliquent qu au supemon c est ok
    if (player->selectedSupemon == NULL) {
        puts("\n\tAucun Supemon actif. Choisissez-en un avant d'utiliser un objet.");
        return;
    }

    unsigned char ch = 0;
    do {
        puts("\n\t====== INVENTAIRE ====== \n");
        printf("\tSupcoins : %hu\n", player->supcoins);
        printf("\tSupemon actif : %s (NIV %hu | HP %hu/%hu)\n\n",
               player->selectedSupemon->species,
               player->selectedSupemon->lvl,
               player->selectedSupemon->hp,
               player->selectedSupemon->maxHp);

// liste des objets
        for (unsigned short i = 0; i < player->itemsCount; i++) {
            printf("\t%hu) %-15s | Qte: %hu\n",
                   (unsigned short)(i + 1),
                   player->inventory[i].item.item_name,
                   player->inventory[i].quantity);
        }
        puts("\t0) Retour");
        printf("\tVotre choix (utiliser l'objet #) : ");

        ch = read_choice_local();

        if (ch == 0) return; // retour au menu principal

        if (ch >= 1 && ch <= player->itemsCount) {
            unsigned short idx = (unsigned short)(ch - 1);

// Utilise l'objet via ta fonction existante
            use_item(player, (unsigned char)idx);

// Si la quantite est tombee a 0, on supprime l entree de l inventaire (compactage)
            if (player->inventory[idx].quantity == 0) {
                for (unsigned short j = idx; j < player->itemsCount - 1; j++) {
                    player->inventory[j] = player->inventory[j + 1];
                }
                player->itemsCount--;
            }

            // Affiche l'etat du Supemon apres usage
            printf("\n\tApres utilisation : %s | HP %hu/%hu | NIV %hu\n",
                   player->selectedSupemon->species,
                   player->selectedSupemon->hp,
                   player->selectedSupemon->maxHp,
                   player->selectedSupemon->lvl);

        } else {
            puts("\tChoix invalide !");
        }

    } while (1);
}

