// backup.c
#include <stdio.h>      // fopen, fwrite, fread, fclose
#include <stdlib.h>
#include <string.h>
#include "backup.h"


// Sauvegarde la partie dans un fichier binaire
int save_game(const Player *player, const char *filename) {
    FILE *file = fopen(filename, "wb");  // "wb" = write binary
    if (file == NULL) {
        perror("Erreur d'ouverture du fichier de sauvegarde");
        return 0; // echec
    }

    // on ecrit toute la structure Player d'un coup
    size_t written = fwrite(player, sizeof(Player), 1, file);

    fclose(file);
    return (written == 1); // 1 = succes, 0 = echec
}

// Charge une partie depuis un fichier binaire
int load_game(Player *player, const char *filename) {
    FILE *file = fopen(filename, "rb");  // "rb" = read binary
    if (file == NULL) {
        perror("Erreur d'ouverture du fichier de chargement");
        return 0;
    }

    size_t read = fread(player, sizeof(Player), 1, file);

    fclose(file);
        if (read != 1) return 0;

    // reconstruire le pointeur depuis l index
    if (player->selectedIndex != NO_ACTIVE_SUPEMON &&
        player->selectedIndex < player->supemonsCount) {
        player->selectedSupemon = &player->supemons[player->selectedIndex];
    } else {
        player->selectedSupemon = NULL;
        player->selectedIndex = NO_ACTIVE_SUPEMON; // securite
    }

    return 1;
}
