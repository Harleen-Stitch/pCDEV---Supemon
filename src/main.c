// main.c
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "types.h"
#include "utils.h"
#include "player.h"
#include "menu.h"
#include "save.h"


int main(void)
{
    srand((unsigned int)time(NULL));

    Player player;
    memset(&player, 0, sizeof(player));

    puts("|========= SUPEMON =========|");
    puts("| 1) Nouvelle partie        |");
    puts("| 2) Charger une partie     |");
    puts("| 3) Quitter                |");
    puts("|___________________________|\n");
    puts("Que voulez-vous faire ? (Saisissez 1, 2 ou 3)");
    int ch = read_int("Je choisis :  ", 1, 2);

    if (ch == 2) {
        if (load_game(&player, SAVE_FILENAME)) {
            printf("Bon retour parmis nous, %s.\n", player.name);
        } else {
            printf("Oups ! Aucune sauvegarde trouvee. Nouvelle partie.\n");
            new_game(&player);
        }
    } else {
        new_game(&player);
    }

    main_menu(&player);
    return 0;
}
