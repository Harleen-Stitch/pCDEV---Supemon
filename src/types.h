#ifndef TYPES_H
#define TYPES_H
#define EXP_REWARD_MAX 500


// Objets (prix d'achat; revente = prix/2)
#define PRICE_POTION 100
#define PRICE_SUPERPOTION 300
#define PRICE_RARE_CANDY 700
#define MAX_NAME_LEN 32
#define MAX_SUPEMONS 12
#define MAX_MOVES     4
#define MAX_ITEMS    32
#define SAVE_FILENAME "save_supemon.txt"


// Limite d'objets utilisables par combat
#define MAX_ITEMS_PER_BATTLE 4


// ---------- Types et énumérations ----------
typedef enum {
MOVE_DAMAGE, // inflige des dégâts
MOVE_BUFF_SELF_ATK, // + ATQ sur soi
MOVE_BUFF_SELF_EVA, // + EVA sur soi
MOVE_BUFF_SELF_DEF // + DEF sur soi
} MoveType;


typedef struct {
char name[24];
MoveType type;
int baseDamage; // utilisé seulement si MOVE_DAMAGE
int statAmount; // +1, +2, etc. pour les buffs
} Move;


typedef struct {
char species[24];


int level; // >=1
int exp; // >=0


int hp; // PV actuels (persistent entre combats)
int maxHp; // PV max


int attack; // ATQ actuelle
int baseAttack; // ATQ au début du combat (pour reset)


int defense; // DEF actuelle
int baseDefense; // DEF au début du combat


int evasion; // EVA actuelle
int baseEvasion; // EVA au début du combat


int accuracy; // PREC actuelle
int baseAccuracy; // PREC au début du combat


int speed; // VITESSE (constante)


Move moves[MAX_MOVES];
int movesCount;
} Supemon;


typedef enum {
ITEM_POTION = 1,
ITEM_SUPER_POTION = 2,
ITEM_RARE_CANDY = 3
} ItemType;


typedef struct {
ItemType type;
int qty;
} ItemStack;


typedef struct {
char name[MAX_NAME_LEN];
Supemon team[MAX_SUPEMONS];
int teamCount;
int selectedIndex; // index du Supémon actif


int supcoins;


ItemStack items[MAX_ITEMS];
int itemsCount;
} Player;


#endif // TYPES_H
