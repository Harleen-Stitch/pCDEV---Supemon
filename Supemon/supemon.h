//supemon.h
#ifndef SUPEMON_H
#define SUPEMON_H

#include "cap.h"

#define SPECIES_NAME_MAX 12
#define MAX_CAP 5

//==================================================
//========== STRUCTURE D UN SUPEMON ==========
//==================================================

//ici je ne fais que declarer les types variable

typedef struct{
    char species[SPECIES_NAME_MAX];
    unsigned short lvl;
    unsigned short exp;
    unsigned short hp, maxHp;
    unsigned short att, def;
    unsigned short eva, acc, spe;
    Cap cap[MAX_CAP]; //liste des attaques
    unsigned short capCount; //nb de capacites apprises
} Supemon;

//def de base d'un supemon
void init_supemon(Supemon *supemon, const char *species,
                   unsigned short lvl,
                   unsigned short hp, unsigned short maxHp,
                   unsigned short att, unsigned short def,
                   unsigned short eva, unsigned short acc, unsigned short spe
                   );

//========================================
//========== AFFICE UN SUPEMON ==========
//========================================
void display_supemon(const Supemon *supemon);

Supemon create_supmander(void);
Supemon create_supasaur(void);
Supemon create_supirtle(void);

#endif // SUPEMON_H_INCLUDED
