//supemon.c
#include <stdio.h>
#include <string.h>
#include "supemon.h"


//je definis les val de base d'un supemon d'apres sa structure
//dans .h je declare le type de variable

void init_supemon(Supemon *supemon, const char *species,
                  unsigned short lvl,
                  unsigned short hp, unsigned short maxHp,
                  unsigned short att, unsigned short def,
                  unsigned short eva, unsigned short acc, unsigned short spe){
    // strncopy(a,b,n) copie n char de b dans a donc b rempl a
    //copie simplement le nom
    strncpy(supemon->species, species, SPECIES_NAME_MAX);
    supemon->species[SPECIES_NAME_MAX-1] = '\0'; //defini le dernier caractere pr faire une liste meme si le nom est trop long
    //definit les stat
    supemon->lvl = lvl;
    supemon->exp = 0 ;
    supemon->hp = hp;
    supemon->maxHp = maxHp;
    supemon->att = att;
    supemon->def = def;
    supemon->eva = eva;
    supemon->acc = acc;
    supemon->spe = spe;
    supemon->capCount = 0;
}

//========================================
//========== AFFICHE LE SUPEMON ==========
//========================================
void display_supemon(const Supemon *supemon){
    puts("");
    printf("\t%-12s | NIV %-3hu | HP %-3hu/%-3hu | ATT %-3hu | DEF %-3hu | EVA %-3hu | PRE %-3hu | VIT %-3hu\n",
           supemon->species,
           supemon->lvl,
           supemon->hp,
           supemon->maxHp,
           supemon->att,
           supemon->def,
           supemon->eva,
           supemon->acc,
           supemon->spe);

}

//========== SUPMANDER ==========
Supemon create_supmander(void){
    Supemon supemon;
    init_supemon(&supemon, "Supmander", 1, 10, 10, 1, 1, 1, 2, 1);

    //lui affecte ses capacites
    init_cap(&supemon.cap[0], "Griffe", 3, 0, 0, 0, 0, 0,
              "Inflige 3 de degats"
              );
    init_cap(&supemon.cap[1], "Rugissement", 0, 1, 0, 0, 0, 1,
              "Augmente ATT de 1"
              );
    supemon.capCount = 2;

    return supemon;
}

//========== SUPASAUR ==========
Supemon create_supasaur(void){
    Supemon supemon;
    init_supemon(&supemon, "Supasaur", 1, 9, 9, 1, 1, 3, 2, 2);

    //lui affecte ses capacites
    init_cap(&supemon.cap[0], "Ecrasement", 2, 0, 0, 0, 0, 0,
              "Inflige 2 de degats"
              );
    init_cap(&supemon.cap[1], "Feuillage", 0, 0, 0, 1, 0, 1,
              "Augmente EVA de 1"
              );
    supemon.capCount = 2;

    return supemon;
}
//========== SUPIRTLE ==========
Supemon create_supirtle(void){
    Supemon supemon;
    init_supemon(&supemon, "Supirtle", 1, 11, 11, 1, 2, 2, 1, 2);

    //lui affecte ses capacites
    init_cap(&supemon.cap[0], "Ecrasement", 2, 0, 0, 0, 0, 0,
              "Inflige 2 de degats"
              );
    init_cap(&supemon.cap[1], "Carapace", 0, 0, 1, 0, 0, 1,
             "Augmente DEF de 1"
             );
    supemon.capCount = 2;

    return supemon;
}
//==================================================
//========== SUPEMON PREND UN OBJET ==========
// ==================================================

void heal_supemon(Supemon *supemon, unsigned short heal) {
    supemon->hp += heal;
    if (supemon->hp > supemon->maxHp)
        supemon->hp = supemon->maxHp;
    printf("%s recupere %hu HP (HP : %-3hu/%3hu)\n", supemon->species,
            heal, supemon->hp, supemon->maxHp
            );
}
