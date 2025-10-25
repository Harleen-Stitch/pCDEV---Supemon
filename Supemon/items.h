#ifndef ITEMS_H
#define ITEMS_H

#define ITEM_NAME_MAX 20
#define ITEM_DESC_MAX 80

//==================================================
//========== STRUCTURE D UN OBJET ==========
//==================================================
typedef struct {
    char item_name[ITEM_NAME_MAX];
    char item_desc[ITEM_DESC_MAX];
    unsigned short price_buy;
    unsigned short price_sell;
    unsigned short heal; //mega potion pourrait donner 300 Hp donc j'ai besoin d'un short
    unsigned char lvl_up;
}Item;

//initialise tous les objets existants du jeu
void init_all_items(void);

//========== un exemplaire ==========
void init_item(Item *item,
               const unsigned char *item_name,
               const unsigned char *item_desc,
               unsigned short price_buy,
               unsigned short price_sell,
               unsigned short heal,
               unsigned char lvl_up
               );

//affiche les infos d un objet
void display_item(const Item *item);

/*declaration externes (objets globaux utilisables dans tout le prog)
ne les recree pas a chaque fois
Mieux dit : j'ai declare dans items.c les objets, ici, je fais appel a ca sans recreer des objets*/
extern Item potion;
extern Item superPotion;
extern Item bonbonExtra;

#endif // ITEMS_H
