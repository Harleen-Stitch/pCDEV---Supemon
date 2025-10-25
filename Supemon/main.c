// main.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "backup.h"
#include "main.h"
#include "player.h"
#include "ooc.h"

int main(void)
{
// active UTF-8 partout => ne fonctionne pas sur les autres fichiers visiblement

    srand((unsigned int)time(NULL));
    //initialise tous les objets au lancement, ne faire qu'une fois
    init_all_items();

    //ensuite, le reste du jeu
    //memset = cette fonction permet de remplir une zone memoire, identifiee par son adresse et sa taille, avec une valeur précise (pointeur, valeur, taille)
    Player player;
    memset(&player, 0, sizeof(player));

    puts("\n\t|========= SUPEMON =========|");
    puts("\t| 1) Nouvelle partie        |");
    puts("\t| 2) Charger une partie     |");
    puts("\t| 3) Quitter                |");
    puts("\t|___________________________|\n");
    puts("\tQue voulez-vous faire ? (Saisissez 1, 2 ou 3)");

    unsigned char ch;
    printf("\n\tJe choisis :  ");
    scanf("%hhu", &ch);

    if (ch == 2) {
        if (load_game(&player, SAVE_FILENAME)) {
            printf("\tBon retour parmis nous, %s.\n", player.name);
        } else {
            printf("\tOups ! Aucune sauvegarde trouvee. Nouvelle partie.\n");
            new_game(&player);
        }
    } else if (ch == 1) {
        new_game(&player);
    } else if (ch == 3){
        puts("\n\tA bientot, dresseur !");
        return 0;
    }

//========================================
//========== MENU PRINCIPAL ==========
//========================================

    puts("\n\tLancement du menu principal...");
    main_menu(&player);

    return 0;
}
