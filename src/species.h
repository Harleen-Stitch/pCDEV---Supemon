#ifndef SPECIES_H
#define SPECIES_H
#include "types.h"


Supemon make_species_supmander(void);
Supemon make_species_supasaure(void);
Supemon make_species_supirtle(void);


void level_up_apply(Supemon *s);
void reset_combat_stats(Supemon *s);
Supemon species_to_level(Supemon base, int level);
Supemon random_enemy_same_level(int level);


#endif // SPECIES_H