#ifndef __CONQUEST__H__
#define __CONQUEST__H__

#include <stdbool.h>

typedef enum{JAIL_ROOM, EMPTY_ROOM, COMBAT_ROOM, TREASURE_ROOM, WEAPON_ROOM, HEAL_ROOM} RoomType;
typedef enum{NONE = 0, SWORD, AXE, HEAL} LootType;

// enemies can be found in rooms
typedef struct Enemy{
    char name[64];
    int health;
    int attack;
} Enemy;

//every room points to its neighbours and may contain an enemy or a loot
//When first visited it prints a message and for every next visit it prints another.
typedef struct Room{
    struct Room* north;
    struct Room* south;
    struct Room* west;
    struct Room* east;
    Enemy enemy;
    RoomType type;
    LootType loot;
    bool visited;
    bool looted;
    char firstVisit[256];
    char nextVisit[256];
} Room;

//The player is in a room position
typedef struct Player
{
    Room* position;
    //Inventory inventory;
    int health;
    int attack;
} Player;



void goTo(Player* player, char direction);
void visitRoom(Room* room);
void combat(Player* player);
void loot(Player* player);
char askAction(Player* player);
void printStatus(Player* player);

#endif
