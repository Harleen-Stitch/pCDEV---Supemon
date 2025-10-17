#include <locale.h>
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <math.h>
#include <stdbool.h>
int clampi(int v, int lo, int hi) {
if (v < lo) return lo;
if (v > hi) return hi;
return v;
}


int rand_range_int(int a, int b) {
if (b < a) { int t = a; a = b; b = t; }
return a + rand() % (b - a + 1);
}


int need_exp_for_next_level(int level) {
// Niveau 1 -> 2 : 500; chaque niveau suivant +1000
if (level < 1) level = 1;
if (level == 1) return 500;
return 500 + (level - 1) * 1000;
}


void press_enter_to_continue(void) {
printf("\n(Appuyez sur Entree pour continuer)\n");
int c; while ((c = getchar()) != '\n' && c != EOF) {}
}


int stochastic_round(double x) {
double r = floor(x);
if (fabs(x - r) < 1e-9) return (int)r; // déjà entier
return (rand() % 2 == 0) ? (int)floor(x) : (int)ceil(x);
}


// ---------- Entrées utilisateur robustes ----------


void read_line(const char *prompt, char *out, size_t outsz) {
if (prompt) printf("%s", prompt);
if (!fgets(out, (int)outsz, stdin)) {
// EOF: mettre chaîne vide
if (outsz) out[0] = '\0';
return;
}
size_t n = strlen(out);
if (n && out[n-1] == '\n') out[n-1] = '\0';
}


int read_int(const char *prompt, int minv, int maxv) {
char buf[64];
int value;
for (;;) {
if (prompt) printf("%s", prompt);
if (!fgets(buf, sizeof(buf), stdin)) return minv; // fallback
if (sscanf(buf, "%d", &value) == 1) {
value = clampi(value, minv, maxv);
return value;
}
printf("Entrée invalide. Réessaie.\n");
}
}


char read_yesno(const char *prompt) {
char buf[8];
for (;;) {
if (prompt) printf("%s", prompt);
if (!fgets(buf, sizeof(buf), stdin)) return 'n';
if (buf[0]=='o'||buf[0]=='O') return 'o';
if (buf[0]=='n'||buf[0]=='N') return 'n';
printf("Réponds par o/n.\n");
}
}
