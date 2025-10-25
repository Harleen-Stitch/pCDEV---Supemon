//center.c

#include <stdio.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#define slepp(x) Sleep(1000 * (x))
#else
#include <unistd.h>
#endif
#include "player.h"
#include "supemon.h"



// Fonction utilitaire pour vider le buffer clavier
static void flush_input(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

// Petite animation textuelle (affiche "..." lentement)
void healing_animation(void) {
    for (int i = 0; i < 3; i++) {
        printf(".");
        fflush(stdout); // force l affichage immediat du point
        sleep(1);       // pause d 1 seconde entre chaque point
    }
    puts(""); // retour a la ligne
}

//========= CENTRE SUPEMON =========
void center(Player *player){
    char answer;

    puts("\n\t====== BIENVENUE AU CENTRE SUPEMON ======");
    puts("\tAgent : Bonjour, dresseur !");
    puts("\tAgent : Confiez-nous vos Supemons, nous allons les soigner...");

// affiche la liste avant soin
    puts("\n\tEtat actuel de vos Supemons :");
    for (unsigned short i = 0; i < player->supemonsCount; i++) {
        display_supemon(&player->supemons[i]);
    }

    printf("\n\tSouhaitez-vous les soigner ? (o/n) : ");
    if (scanf(" %c", &answer) != 1) {
        flush_input();                 // purge si saisie invalide
        answer = 'n';
    }
    flush_input();                     // <-- important : vide tout le reste de la ligne

    if (answer == 'o' || answer == 'O') {
        puts("\n\tAgent : Tres bien, nous prenons soin de vos Supemons...");
        healing_animation();

// soigne tous les Supemons du joueur
        for (unsigned short i = 0; i < player->supemonsCount; i++) {
            player->supemons[i].hp = player->supemons[i].maxHp;
        }

        puts("\n\tAgent : Tout est en ordre !");
        puts("\tAgent : Vos Supemons sont en plein de vie !");
        puts("\tA bientot, et prenez soin d eux !");

// affiche la liste apres soin
        puts("\n\t=== Supemons apres soin ===");
        for (unsigned short i = 0; i < player->supemonsCount; i++) {
            display_supemon(&player->supemons[i]);
        }

    } else {
        puts("\n\tAgent : Tres bien, revenez quand vous le souhaitez !");
    }

    return;  // <-- revient directement au main_menu()
}



