#ifndef FORMULE_H
#define FORMULE_H

//========== ARRONDI ==========
// arrondit un float avec 50% de chance vers le haut
unsigned short rand_round(float val);

//========== DEGATS ==========
// Calcule les degets infliges (attaque, puissance du move, defense adverse)
unsigned short calc_damage(unsigned short attack,
                           unsigned short power,
                           unsigned short defense);

//========== PRECISION / ESQUIVE ==========
// Renvoie la probabilite de succes d'une attaque
float calc_hit_success(unsigned short acc, unsigned short eva);

//========== ORDRE DE TOUR ==========
// Determine qui attaque en premier :
//  renvoie 1 si le joueur commence, 0 si l'ennemi commence
int calc_first_turn(unsigned short player_speed,
                    unsigned short enemy_speed);

//========== FUITE ==========
// Renvoie la probabilite de reussir e fuir
float calc_escape_rate(unsigned short player_speed,
                       unsigned short enemy_speed);

//========== CAPTURE ==========
// Renvoie la probabilite de capturer un Supemon selon sa vie restante
float calc_capture_rate(unsigned short enemy_hp,
                        unsigned short enemy_maxHp);

#endif // FORMULE_H
