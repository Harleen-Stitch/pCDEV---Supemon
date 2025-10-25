// ooc.c
#include <stdio.h>

#include "backup.h"
#include "battle.h"
#include "center.h"
#include "items.h"
#include "player.h"
#include "supemon.h"
#include "shop.h"

// ---------- Helpers d'entree ----------
static void flush_input(void) {
    int c; while ((c = getchar()) != '\n' && c != EOF) {}
}

static unsigned char read_choice(void) {
    unsigned int tmp = 0;
    if (scanf("%u", &tmp) != 1) {
        flush_input();
        return 255;
    }
    flush_input();
    if (tmp > 255) return 255;
    return (unsigned char)tmp;
}

// ---------- Securise l'actif avant sauvegarde ----------
static void normalize_selected_for_save(Player *p) {
    if (p->selectedSupemon == NULL) {
        p->selectedIndex = NO_ACTIVE_SUPEMON;
        return;
    }
    for (unsigned short i = 0; i < p->supemonsCount; i++) {
        if (&p->supemons[i] == p->selectedSupemon) {
            p->selectedIndex = (unsigned char)i;
            return;
        }
    }
    // si le pointeur ne pointe plus dans le tableau, on neutralise
    p->selectedSupemon = NULL;
    p->selectedIndex   = NO_ACTIVE_SUPEMON;
}

// --- Choisir le Supemon actif depuis l'equipe ---
static void choose_active_supemon(Player *player) {
    if (player->supemonsCount == 0) {
        puts("\n\tTu n'as encore aucun Supemon.");
        return;
    }

    puts("\n\t========== TON EQUIPE ==========\n");
    for (unsigned short i = 0; i < player->supemonsCount; i++) {
        printf("\n\t%hu) %-12s | NIV %-3hu | HP %-3hu/%3hu%s\n",
               (unsigned short)(i + 1),
               player->supemons[i].species,
               player->supemons[i].lvl,
               player->supemons[i].hp,
               player->supemons[i].maxHp,
               (player->selectedSupemon == &player->supemons[i]) ? " (actif)" : "");
    }
    puts("\t0) Retour");
    printf("\n\tTon choix : ");

    unsigned int choice = 0;
    if (scanf("%u", &choice) != 1) {
        flush_input();
        puts("\n\tChoix invalide.");
        return;
    }
    flush_input();

    if (choice == 0 || choice > player->supemonsCount) {
        puts("\n\tAucun changement effectue.");
        return;
    }

    unsigned short idx = (unsigned short)(choice - 1);

    if (player->supemons[idx].hp == 0) {
        puts("\n\tCe Supemon est K.O. !");
        return;
    }

    player->selectedIndex   = (unsigned char)idx;
    player->selectedSupemon = &player->supemons[idx];

    printf("\n\t%s est maintenant votre Supemon actif !\n",
           player->selectedSupemon->species);
}

//========================================
// ========== Menu principal ==========
//========================================
void main_menu(Player *player) {
    unsigned char ch = 0;
    do {
        puts("\n\t=========== MENU PRINCIPAL ===========\n");
        display_player(player);
        puts("\n\n\tQue voulez-vous faire ?");
        puts("\n\t\t1) Aller au combat");
        puts("\t\t2) Visiter la boutique");
        puts("\t\t3) Se rendre au centre Supemon (soin)");
        puts("\t\t4) Ouvrir l'inventaire");
        puts("\t\t5) Voir votre equipe / Changer d'actif");
        puts("\t\t6) Sauvegarder");
        puts("\t\t0) Quitter");
        puts("\n\t=====================================\n");

        printf("\n\tJe choisis :  ");
        ch = read_choice();

        switch (ch) {
        case 1:
            puts("\n\tVous marchez tranquillement dans la nature quand soudain");
            healing_animation();
            puts("\n\tUn Supemon sauvage vous attaque !");
            battle_start(player);
            break;

        case 2:
            shop_menu(player);
            continue;

        case 3:
            center(player);
            continue;

        case 4:
            open_inventory(player);
            continue;

        case 5:
            choose_active_supemon(player);
            break;

        case 6: {
//garantit que l index correspond bien au pointeur
            normalize_selected_for_save(player);

            if (save_game(player, SAVE_FILENAME)) {
                puts("\tSauvegarde reussie !");
            } else {
                puts("\tErreur lors de la sauvegarde !");
            }
            break;
        }

        case 0:
            //je tente une modif pour enregistrer avant de quitter
            puts("\tVoulez-vous enregistrer avant de quitter ? (o/n)");
            char answer;
            if (scanf(" %c", &answer) != 1) {
                flush_input(); // purge si saisie invalide
                answer = 'n';
            }
            flush_input();  // <-- important : vide tout le reste de la ligne

            if (answer == 'o' || answer == 'O') {
                normalize_selected_for_save(player);

                if (save_game(player, SAVE_FILENAME)) {
                    puts("\tSauvegarde reussie !");
                } else {
                    puts("\tErreur lors de la sauvegarde !");
                }
            }
            puts("\n\tA bientot, dresseur !");
            break;

        default:
            puts("\tChoix invalide");
        }
    } while (ch != 0);
}
