// supemon.c
#include "species.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <math.h>
#include <stdbool.h>
// NE REDÉFINIS PAS Supemon ici !
// Pas de typedef struct Supemon {...};  ← à supprimer

Supemon make_species_supmander(void) {
    Supemon s; memset(&s, 0, sizeof(s));
    strcpy(s.species, "Supmander");
    s.level = 1; s.exp = 0;
    s.maxHp = 10; s.hp = s.maxHp;
    s.attack = s.baseAttack = 1;
    s.defense = s.baseDefense = 1;
    s.evasion = s.baseEvasion = 1;
    s.accuracy = s.baseAccuracy = 2;
    s.speed = 1;

    s.movesCount = 0;
    Move m1 = {"Scratch", MOVE_DAMAGE, 3, 0};
    s.moves[s.movesCount++] = m1;
    Move m2 = {"Grawl", MOVE_BUFF_SELF_ATK, 0, 1};
    s.moves[s.movesCount++] = m2;

    return s;
}

Supemon make_species_supasaure(void) {
    Supemon s; memset(&s, 0, sizeof(s));
    strcpy(s.species, "Supasaure");
    s.level = 1; s.exp = 0;
    s.maxHp = 9; s.hp = s.maxHp;
    s.attack = s.baseAttack = 1;
    s.defense = s.baseDefense = 1;
    s.evasion = s.baseEvasion = 3;
    s.accuracy = s.baseAccuracy = 2;
    s.speed = 2;

    s.movesCount = 0;
    Move m1 = {"Pound", MOVE_DAMAGE, 2, 0};
    s.moves[s.movesCount++] = m1;
    Move m2 = {"Foliage", MOVE_BUFF_SELF_EVA, 0, 1};
    s.moves[s.movesCount++] = m2;

    return s;
}

Supemon make_species_supirtle(void) {
    Supemon s; memset(&s, 0, sizeof(s));
    strcpy(s.species, "Supirtle");
    s.level = 1; s.exp = 0;
    s.maxHp = 11; s.hp = s.maxHp;
    s.attack = s.baseAttack = 1;
    s.defense = s.baseDefense = 2;
    s.evasion = s.baseEvasion = 2;
    s.accuracy = s.baseAccuracy = 1;
    s.speed = 2;

    s.movesCount = 0;
    Move m1 = {"Pound", MOVE_DAMAGE, 2, 0};
    s.moves[s.movesCount++] = m1;
    Move m2 = {"Shell", MOVE_BUFF_SELF_DEF, 0, 1};
    s.moves[s.movesCount++] = m2;

    return s;
}

void level_up_apply(Supemon *s) {
    double nhp  = s->maxHp   * 1.3;
    double natk = s->attack  * 1.3;
    double ndef = s->defense * 1.3;
    double neva = s->evasion * 1.3;
    double nacc = s->accuracy* 1.3;
    double nspd = s->speed   * 1.3;

    s->maxHp    = stochastic_round(nhp);
    s->attack   = s->baseAttack   = stochastic_round(natk);
    s->defense  = s->baseDefense  = stochastic_round(ndef);
    s->evasion  = s->baseEvasion  = stochastic_round(neva);
    s->accuracy = s->baseAccuracy = stochastic_round(nacc);
    s->speed    = stochastic_round(nspd);
    s->hp = s->maxHp;
}

void reset_combat_stats(Supemon *s) {
    s->attack   = s->baseAttack;
    s->defense  = s->baseDefense;
    s->evasion  = s->baseEvasion;
    s->accuracy = s->baseAccuracy;
}

Supemon species_to_level(Supemon base, int level) {
    Supemon s = base;
    s.level = 1; s.exp = 0; s.hp = s.maxHp;
    for (int i = 2; i <= level; ++i) {
        level_up_apply(&s);
        s.level = i;
    }
    s.hp = s.maxHp;
    return s;
}

Supemon random_enemy_same_level(int level) {
    int r = rand_range_int(1, 3);
    Supemon base = (r==1)? make_species_supmander()
                   : (r==2)? make_species_supasaure()
                           : make_species_supirtle();
    return species_to_level(base, level);
}
