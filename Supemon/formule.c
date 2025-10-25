//formule.c
#include <stdlib.h>
#include <stdio.h>
#include "formule.h"


// ========== ARRONDI ==========
// 50% chance de round up si valeur non entiere
unsigned short rand_round(float val) {
    float decimal = val - (unsigned short)val;
    if (decimal <= 0.0001f) {
        return (unsigned short)val;
    } else {
        if (rand() % 2 == 0)
            return (unsigned short)val;
        else
            return (unsigned short)(val + 1);
    }
}

// ========== DEGATS =============
// Damage = (Attack * Power) / Defense
// puis arrondi aleatoire selon les regles
unsigned short calc_damage(unsigned short attack,
                           unsigned short power,
                           unsigned short defense) {
    if (defense == 0) defense = 1; // securite anti-division par
    float raw = ((float)attack * power) / (float)defense;
    return rand_round(raw);
}

// ========== PRECISION / ESQUIVE =============
// Hit chance = Acc / (Acc + Eva) + 0.1
float calc_hit_success(unsigned short acc, unsigned short eva) {
    float hit = (float)acc / ((float)acc + (float)eva) + 0.1f;
    if (hit > 1.0f) hit = 1.0f;
    if (hit < 0.0f) hit = 0.0f;
    return hit;
}

// ========== ORDRE DU PREMIER JOUEUR =============
// Compare les vitesses ; tirage aleatoire si egalite
int calc_first_turn(unsigned short player_speed,
                    unsigned short enemy_speed) {
    if (player_speed > enemy_speed)
        return 1; // joueur commence
    else if (player_speed < enemy_speed)
        return 0; // ennemi commence
    else
        return rand() % 2; // egalite : tirage aleatoire
}

// ========== FUITE =============
// success = player_speed / (player_speed + enemy_speed)
float calc_escape_rate(unsigned short player_speed,
                       unsigned short enemy_speed) {
    float rate = (float)player_speed /
                 ((float)player_speed + (float)enemy_speed);
    if (rate > 1.0f) rate = 1.0f;
    if (rate < 0.0f) rate = 0.0f;
    return rate;
}

// ========== CAPTURE =============
// success = (Enemy_MaxHP - Enemy_HP) / Enemy_MaxHP - 0.5
float calc_capture_rate(unsigned short enemy_hp,
                        unsigned short enemy_maxHp) {
    if (enemy_maxHp == 0) return 0.0f;
    float rate = ((float)(enemy_maxHp - enemy_hp) / (float)enemy_maxHp) - 0.5f;
    if (rate < 0.0f) rate = 0.0f;
    if (rate > 1.0f) rate = 1.0f;
    return rate;
}
