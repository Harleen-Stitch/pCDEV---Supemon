#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>   // pour size_t
#include "types.h"


int clampi(int v, int lo, int hi);
int rand_range_int(int a, int b); // inclusif
int need_exp_for_next_level(int level);
void press_enter_to_continue(void);
int stochastic_round(double x);


// Entrées utilisateur robustes
int read_int(const char *prompt, int minv, int maxv);
char read_yesno(const char *prompt); // retourne 'o' ou 'n'
void read_line(const char *prompt, char *out, size_t outsz);


#endif // UTILS_H
