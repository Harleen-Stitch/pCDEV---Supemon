// player.c
#include <stdio.h>      // printf
#include <string.h>     // memset
#include "types.h"      // définitions de Player/Supemon/constantes
#include "player.h"
#include "species.h"    // prototypes make_species_*
#include "utils.h"      // read_line, read_int, read_yesno
#include "items.h"      // give_starter_items
#include <stddef.h>
#include <math.h>
#include <stdbool.h>
// ⚠️ Ne re-définis PAS les constantes ici : elles sont déjà dans types.h
// Supprime les #define MAX_SUPEMONS ... etc. de ce fichier.

static void print_supemon_brief(const Supemon *s) {
    printf("%s (Niv %d) HP %d/%d | ATQ %d DEF %d EVA %d PREC %d VIT %d\n",
           s->species, s->level, s->hp, s->maxHp, s->attack, s->defense,
           s->evasion, s->accuracy, s->speed);
}

void new_game(Player *p) {
    memset(p, 0, sizeof(*p));
    read_line("Entrez votre nom : ", p->name, sizeof(p->name));

    // Déclarations AVANT instructions (compatible C90)
    Supemon s1 = make_species_supmander();
    Supemon s2 = make_species_supasaure();
    Supemon s3 = make_species_supirtle();

    printf("\nChoisissez votre Supemon de départ :\n");
    printf("1) "); print_supemon_brief(&s1);
    printf("2) "); print_supemon_brief(&s2);
    printf("3) "); print_supemon_brief(&s3);

    int ch = read_int("> ", 1, 3);
    if (ch == 1) p->team[p->teamCount++] = s1;
    else if (ch == 2) p->team[p->teamCount++] = s2;
    else              p->team[p->teamCount++] = s3;

    p->selectedIndex = 0;
    p->supcoins = 500; // budget de départ
    give_starter_items(p);

    printf("\nBienvenue %s ! Votre aventure Supemon commence !\n", p->name);
}

void pokemon_center(Player *p) {
    printf("\nBienvenue au Centre Supemon !\n");
    if (p->teamCount == 0) { printf("Vous n'avez aucun Supemon...\n"); return; }
    for (int i = 0; i < p->teamCount; ++i) {
        Supemon *s = &p->team[i];
        printf(" - %s (niv %d) %d/%d PV\n", s->species, s->level, s->hp, s->maxHp);
    }
    if (read_yesno("Voulez-vous soigner tous vos Supemons ? (o/n) ")=='o') {
        for (int i = 0; i < p->teamCount; ++i) p->team[i].hp = p->team[i].maxHp;
        printf("Tous vos Supemons sont soignes !\n");
    }
}

void list_team(const Player *p) {
    printf("\nVotre equipe (%d/%d) :\n", p->teamCount, MAX_SUPEMONS);
    for (int i = 0; i < p->teamCount; ++i) {
        const Supemon *s = &p->team[i];
        printf("%d) %s (Niv %d) HP %d/%d | ATQ %d DEF %d EVA %d PREC %d VIT %d\n",
               i+1, s->species, s->level, s->hp, s->maxHp, s->attack, s->defense,
               s->evasion, s->accuracy, s->speed);
    }
    printf("Actif : #%d\n", p->selectedIndex+1);
}
