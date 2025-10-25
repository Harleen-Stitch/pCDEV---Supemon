//exp_lvl.c
#include <stdio.h>
#include <stdlib.h> // pour rand()
#include "exp_lvl.h"
#include "supemon.h"
#include "formule.h"


//calcul du nb d'EXP necessaire pr passe 1 niv
unsigned short exp_for_next_lvl(const Supemon *supemon){
    unsigned short exp_goal = 0;

    //si lvl 1
    if (supemon->lvl == 1){
        exp_goal = EXP_BASE;
    } else {
    //niv suivant
    exp_goal = EXP_BASE + (supemon->lvl -1)* EXP_INCREMENT;
    }
    return exp_goal;
}

//affiche l'exp en fin de combat en cas de vicctoire
void exp_vic(Supemon *supemon, unsigned short exp_rand, unsigned short enemy_lvl){
    unsigned short gain = exp_rand * enemy_lvl;
    supemon->exp += gain;
    printf("\n%s gagne %hu d'EXP\nNiveau EXP actuel = %hu",
           supemon->species,
           exp_rand,
           supemon->exp);

    //verifie si je passe de niveau
    //boucle if si je considere qu'on peut passer 2niv d'un coup
    //boucle while pour evo possible du jeu (potion speciale)
    while (supemon->exp>= exp_for_next_lvl(supemon) ){
        supemon->exp -= exp_for_next_lvl(supemon);
        lvl_up(supemon);
        printf("\n\nWahou ! %s monte au niveau %hu !",
           supemon->species, supemon->lvl
           );
        display_supemon(supemon);
    }
}
//maj des stat en cas de lvl up
void lvl_up(Supemon *supemon) {
    supemon->lvl++; //j'increment de 1
    //consequence sur les stat
    supemon->maxHp = rand_round(supemon->maxHp * 1.3f);
    supemon->att = rand_round(supemon->att * 1.3f);
    supemon->def = rand_round(supemon->def * 1.3f);
    supemon->eva = rand_round(supemon->eva * 1.3f);
    supemon->acc = rand_round(supemon->acc * 1.3f);
    supemon->spe = rand_round(supemon->spe * 1.3f);
    supemon->hp = supemon->maxHp;
}
