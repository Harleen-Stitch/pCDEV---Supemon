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

// ⚠️ Ne redéfinis pas MAX_ITEMS ou SAVE_FILENAME ici.
// Laisse-les dans types.h pour éviter les incohérences.

int main(void)
{
    // Pour mieux afficher les accents selon l’OS
    setlocale(LC_ALL, "");

    srand((unsigned int)time(NULL));

    Player player;
    memset(&player, 0, sizeof(player));

    printf("===== SUPEMON (CLI) =====\n");
    printf("1) Nouvelle partie\n2) Charger une partie\n");
    int ch = read_int("> ", 1, 2);

    if (ch == 2) {
        if (load_game(&player, SAVE_FILENAME)) {
            printf("Partie chargee ! Bienvenue a nouveau, %s.\n", player.name);
        } else {
            printf("Aucune sauvegarde trouvee. Nouvelle partie.\n");
            new_game(&player);
        }
    } else {
        new_game(&player);
    }

    main_menu(&player);
    return 0;
}
