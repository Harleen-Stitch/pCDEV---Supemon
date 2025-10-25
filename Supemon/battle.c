//battle.c

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <locale.h>
#ifdef _WIN32
#include <windows.h>
#endif

#include "battle.h"
#include "exp_lvl.h"
#include "supemon.h"
#include "player.h"
#include "items.h"
#include "formule.h"   // degats, precision, initiative, capture

// ---------------------
// Helpers d'entree (securises) pour battle
// ---------------------
static void flush_input_b(void) {
    int c; while ((c = getchar()) != '\n' && c != EOF) {}
}

static unsigned char read_choice_b(void) {
    unsigned int tmp = 0;
    if (scanf("%u", &tmp) != 1) { flush_input_b(); return 255; }
    flush_input_b();
    if (tmp > 255) return 255;
    return (unsigned char)tmp;
}

//==================================================
// ========== GESTION DES COMBATS ==========
//==================================================

//========== Changer de Supemon pendant le combat
// Retourne 1 si un changement est possible, 0 sinon (retour/annulation)
static int battle_switch_supemon(Player *player) {
    if (player->supemonsCount <= 1) {
        puts("\n\tTu n'as pas d'autre Supemon !");
        return 0;
    }

    puts("\n\t===== CHANGER DE SUPEMON =====");
    for (unsigned short i = 0; i < player->supemonsCount; i++) {
        printf("\t%hu) %-12s | NIV %-3hu | HP %-3hu/%3hu%s\n",
               (unsigned short)(i + 1),
               player->supemons[i].species,
               player->supemons[i].lvl,
               player->supemons[i].hp,
               player->supemons[i].maxHp,
               (player->selectedSupemon == &player->supemons[i]) ? "  (actif)" : "");
    }
    puts("\t0) Retour");
    printf("\tVotre choix : ");

    unsigned char ch = read_choice_b();
    if (ch == 0) return 0;  // annuler

    if (ch < 1 || ch > player->supemonsCount) {
        puts("\tChoix invalide !");
        return 0;
    }

    unsigned short idx = (unsigned short)(ch - 1);

    if (&player->supemons[idx] == player->selectedSupemon) {
        puts("\tCe Supemon est dejà actif !");
        return 0;
    }
    if (player->supemons[idx].hp == 0) {
        puts("\tCe Supemon est K.O. ! Choisis-en un autre.");
        return 0;
    }

    player->selectedSupemon = &player->supemons[idx];
    player->selectedIndex   = idx;

    printf("\tAllez, %s !\n", player->selectedSupemon->species);
    return 1;
}

/*
 * Ouvre le sac pendant le combat.
 * Retourne 1 si un objet a ete utilise (tour consomme),
 * 0 si aucun objet utilise (retour/annulation).
 */
static int battle_use_item(Player *player) {
    if (player->itemsCount == 0) {
        puts("\n\tTon sac est vide !");
        return 0;
    }
    if (player->selectedSupemon == NULL) {
        puts("\n\tAucun Supemon actif. Impossible d'utiliser un objet.");
        return 0;
    }

    unsigned char ch = 0;
    do {
        puts("\n\t========== SAC D OBJETS ==========");
        printf("\tSupemon actif : %s (NIV %hu | HP %-3hu/%3hu)\n",
               player->selectedSupemon->species,
               player->selectedSupemon->lvl,
               player->selectedSupemon->hp,
               player->selectedSupemon->maxHp);

        for (unsigned short i = 0; i < player->itemsCount; i++) {
            printf("\t%hu) %-15s | Qte: %hu\n",
                   (unsigned short)(i + 1),
                   player->inventory[i].item.item_name,
                   player->inventory[i].quantity);
        }
        puts("\t0) Retour");
        printf("\tChoix (numero de l'objet) : ");

        ch = read_choice_b();

        if (ch == 0) return 0; // annuler/retour

        if (ch >= 1 && ch <= player->itemsCount) {
            unsigned short idx = (unsigned short)(ch - 1);

// si qte == 0, mais normalement, si qte == 0, l objet n est pas dans l inventaire
            if (player->inventory[idx].quantity == 0) {
                puts("\tTu n'en as plus...");
                continue;
            }

            // utilise l'objet (defini dans player.c)
            use_item(player, idx);

            // Si quantite = 0 après usage, on compacte l'inventaire
            if (player->inventory[idx].quantity == 0) {
                for (unsigned short j = idx; j < player->itemsCount - 1; j++) {
                    player->inventory[j] = player->inventory[j + 1];
                }
                player->itemsCount--;
            }

            // Affiche l'etat après utilisation
            printf("\n\tApres utilisation : %s | HP %-3hu/%3hu | NIV %hu\n",
                   player->selectedSupemon->species,
                   player->selectedSupemon->hp,
                   player->selectedSupemon->maxHp,
                   player->selectedSupemon->lvl);

            return 1; // objet utilise -> tour consomme
        } else {
            puts("\tChoix invalide !");
        }

    } while (1);
}

// =====================
// TOUR DE COMBAT — une seule definition
// =====================
void battle_turn(Supemon *attacker, Supemon *defender, int isPlayer) {
    unsigned char choice = 0;
    Cap *cap = NULL;

    // ==== Selection de la capacite ====
    if (isPlayer) {
        printf("\n\t=== CAPACITES DE %s ===\n", attacker->species);
        for (unsigned short i = 0; i < attacker->capCount; i++) {
            printf("\t%hu) %-12s - %s\n",
                   i + 1,
                   attacker->cap[i].cap_name,
                   attacker->cap[i].cap_desc);
        }

        // Choix verifie (lecture sure)
        do {
            printf("\tVotre choix : ");
            choice = read_choice_b();
            if (choice == 0 || choice > attacker->capCount) {
                puts("\tChoix invalide !");
                choice = 0;
            }
        } while (choice == 0);

        cap = &attacker->cap[choice - 1];
    } else {
        // L'ennemi choisit une attaque au hasard
        unsigned short randomCap = rand() % attacker->capCount;
        cap = &attacker->cap[randomCap];
    }

    printf("\n\t%s utilise %s !\n", attacker->species, cap->cap_name);

    // ========== Capacite offensive ==========
    if (cap->power > 0) {
        //========== Precision / esquive via formule.h
        float hitChance = calc_hit_success(attacker->acc, defender->eva);
        //retourne uen valeur entre 0 et 1 car rand nb aleatoire entre 0 et rand_max
        float roll = (float)rand() / RAND_MAX;
        if (roll > hitChance) {
            printf("\t%s esquive l attaque ! (%.0f%% de chance)\n",
                   defender->species, (1 - hitChance) * 100);
            return;
        }

        //========== Degats via formule.h ---
        unsigned short finalDamage = calc_damage(attacker->att, cap->power, defender->def);

        // Garde mini 1 si on a une attaque non nulle (optionnel)
        if (finalDamage == 0 && cap->power > 0) finalDamage = 1;

        if (finalDamage >= defender->hp)
            defender->hp = 0;
        else
            defender->hp -= finalDamage;

        printf("\t%s inflige %hu points de degats a %s !\n",
               attacker->species, finalDamage, defender->species);
        printf("\t(HP restants : %-3hu/%3hu)\n", defender->hp, defender->maxHp);
    }

    // ==== Capacite de soutien ====
    else {
        if (cap->targetSelf) {
            attacker->att += cap->attChg;
            attacker->def += cap->defChg;
            attacker->eva += cap->evaChg;
            attacker->acc += cap->accChg;
            printf("\tLes statistiques de %s augmentent !\n", attacker->species);
        } else {
            defender->att -= cap->attChg;
            defender->def -= cap->defChg;
            defender->eva -= cap->evaChg;
            defender->acc -= cap->accChg;
            printf("\tLes statistiques de %s diminuent !\n", defender->species);
        }
    }
}

// =====================
// Lancement du combat
// =====================
void battle_start(Player *player) {

    if (player->selectedSupemon == NULL) {
        puts("\n\tAucun Supemon actif ! Selectionnez-en un avant de combattre.");
        return;
    }

    // Creation du Supemon sauvage
    //creation de l objet wild
    Supemon wild;
    unsigned short random = rand() % 3;
    if (random == 0) wild = create_supmander();
    else if (random == 1) wild = create_supasaur();
    else wild = create_supirtle();

    Supemon *playerSupemon = player->selectedSupemon;
    Supemon *wildSupemon = &wild;
    //faut mettre le wild au niveau
    while (wildSupemon->lvl < playerSupemon->lvl){
        lvl_up(wildSupemon);
    }

    printf("\n\tUn %s sauvage apparait !\n", wildSupemon->species);
    printf("\tAllez, %s !\n", playerSupemon->species);

    // ---- Ordre du premier joueur via formule.h ----
    int playerFirst = calc_first_turn(playerSupemon->spe, wildSupemon->spe);
    Supemon *first  = playerFirst ? playerSupemon : wildSupemon;

    if (playerFirst) {
        printf("\n\t%s est plus rapide et attaque en premier !\n", playerSupemon->species);
    } else {
        printf("\n\t%s sauvage est plus rapide et attaque en premier !\n", wildSupemon->species);
    }

    // === Boucle principale du combat ===
    while (playerSupemon->hp > 0 && wildSupemon->hp > 0) {

        printf("\n\t===== ETAT DES SUPEMONS =====\n");
        display_supemon(playerSupemon);
        display_supemon(wildSupemon);

        printf("\n\t===== MENU DE COMBAT =====\n");
        puts("\t1) Attaquer");
        puts("\t2) Utiliser un objet");
        puts("\t3) Capturer le Supemon");
        puts("\t4) Changer de Supemon");
        puts("\t5) Fuir");

        unsigned char ch = 0;
        printf("\tVotre choix : ");
        ch = read_choice_b();    // lecture securisee avec purge

        switch (ch) {
            case 1: // Attaque
                if (first == playerSupemon) {
                    battle_turn(playerSupemon, wildSupemon, 1);
                    if (wildSupemon->hp == 0) break;
                    battle_turn(wildSupemon, playerSupemon, 0);
                } else {
                    battle_turn(wildSupemon, playerSupemon, 0);
                    if (playerSupemon->hp == 0) break;
                    battle_turn(playerSupemon, wildSupemon, 1);
                }
                break;

            case 2: // Utiliser un objet (tour consomme si utilise)
            {
                int used = battle_use_item(player);
                if (used) {
                    if (wildSupemon->hp > 0 && playerSupemon->hp > 0) {
                        battle_turn(wildSupemon, playerSupemon, 0);
                    }
                }
                break;
            }

            case 3: // === CAPTURE ===
            {
                if (player->supemonsCount >= MAX_SUPEMONS) {
                    puts("\tTon equipe est deja complete !");
                    break;
                }

                //
                float captureChance = calc_capture_rate(wildSupemon->hp, wildSupemon->maxHp);
                float roll = (float)rand() / RAND_MAX;

                printf("\n\tTentative de capture... (Chance : %.0f%%)\n", captureChance * 100);

                if (roll < captureChance) {
                    printf("\n\tSucces ! Tu as capture %s sauvage ! \n",
                           wildSupemon->species);
                    player->supemons[player->supemonsCount++] = *wildSupemon;

                    puts("\tLe combat est termine !");

                    unsigned short exp_gain = (rand() % 401) + 100;
                    exp_vic(playerSupemon, exp_gain, wildSupemon->lvl);

                    unsigned short coin_gain = (rand() % 401) + 100;
                    player->supcoins += coin_gain;

            printf("\tVous avez gagne %hu Supcoins ! (total : %hu)\n", coin_gain, player->supcoins);
                    return;
                } else {
                    printf("\n\techec ! %s s'echappe...\n", wildSupemon->species);
                }
                break;
            }

            case 4: // Changer de Supemon
            {
                int switched = battle_switch_supemon(player);
                if (switched) {
                    playerSupemon = player->selectedSupemon;
                    if (wildSupemon->hp > 0 && playerSupemon->hp > 0) {
                        battle_turn(wildSupemon, playerSupemon, 0);
                    }
                }
                break;
            }

            case 5:
                puts("\n\tVous prenez la fuite !");
                return;

            default:
                puts("\tChoix invalide !");
                break;
        }

        // Fin de combat
        if (wildSupemon->hp == 0) {
            printf("\n\t%s sauvage est K.O. !\n", wildSupemon->species);
            puts("\tVictoire !");

            unsigned short exp_gain = (rand() % 401) + 100;
            exp_vic(playerSupemon, exp_gain, wildSupemon->lvl);

            unsigned short coin_gain = (rand() % 401) + 100;
            player->supcoins += coin_gain;



            printf("\nVous avez gagne %hu Supcoins ! (total : %hu)\n", coin_gain, player->supcoins);
            return;
        }

        if (playerSupemon->hp == 0) {
            printf("\n\tOh non ! %s est K.O. !\n", playerSupemon->species);
            puts("\tVous avez perdu le combat...");
            return;
        }
    }
}

