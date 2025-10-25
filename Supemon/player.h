//player.h

// permet de définir une première fois le fichier et ne l'appeler qu'une seule fois dans un autre fichier.
#ifndef PLAYER_H
#define PLAYER_H

//j'appelle les fichiers dont j'ai besoin. Ils sont protégés des appels multiples dans leur fichier respectif
#include "supemon.h" // définit les supemons
#include "items.h" // définit les items

//je définis mes constantes
#define PLAYER_NAME_MAX 50
#define MAX_SUPEMONS 10
#define MAX_ITEMS 15
#define MAX_ITEM_QUANTITY 50 //pas utilise ici mais servira pour des limites
#define NO_ACTIVE_SUPEMON 255

//==================================================
//========== INVENTAIRE DES OBJETS ==========
//==================================================
//typedef permet de renommer un type
typedef struct{
    //Classe Item et objet item
    Item item;
    //sa quantite est un int
    unsigned short quantity;
//nom de la structure (ca devient un type comme int ou unsigned short)
}InventorySlot;        

//==================================================
//========== STRUCTURE DU JOUEUR ==========
//==================================================
    
typedef struct {
    char name[PLAYER_NAME_MAX];
    Supemon supemons[MAX_SUPEMONS];
    //memorisee combien de supemon sont dans ma liste. Count car compteur
    unsigned char supemonsCount;
    unsigned char selectedIndex; //on sauvegarde un index (0..count-1) plutot qu un pointeur
    Supemon *selectedSupemon;
    unsigned short supcoins;
    //ds ma structure InventorySlot, ma liste s'appelle inventory
    InventorySlot inventory[MAX_ITEMS];
    unsigned char itemsCount;
    } Player;
    //permet d'écrire "Supemon s;" au lieu de "struct Supemon s;"


//je déclare ma fonction "nouv partie" avec 1 paramètre pointant vers Player. Comme on utilise un pointeur, appeler la fonction permet directement de modifier la valeur 
//void comme type = fct ne renvoie rien
void new_game(Player *player);
//affiche le joueur
void display_player(const Player *player);
//utilise un item
void use_item(Player *player, unsigned char index);
//ouvir l inventaire
void open_inventory(Player *player);

#endif // PLAYER_H
