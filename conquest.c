#include "conquest.h"
#include "utility.h"
#include <stdlib.h>
#include <stdio.h>


void goTo(Player* player, char direction)
{
    switch(direction)
    {
        case 'N':
            player->position = player->position->north;
            break;
        case 'S':
            player->position = player->position->south;
            break;
        case 'W':
            player->position = player->position->west;
            break;
        case 'E':
            player->position = player->position->east;
            break;
    }
    visitRoom(player->position);
}

void visitRoom(Room* room)
{
    if(room->visited){ // Room was already visited
        bufferedPrint("%s\n", room->nextVisit);
    }
    else{   // Room was visited for the first time
        bufferedPrint("%s\n", room->firstVisit);
        room->visited = true;
    }
}

void printStatus(Player* player)
{
    StatusEntry status[] = {
        {"Health", "+", 10, 100, player->health},
        {"Attack", "<=", 4, 40, player->attack},
    };
    drawStatus(ARRAY_SIZEOF(status), status);
}

void loot(Player* player)
{
    int weapon = player->attack;

    switch(player->position->loot){
        case NONE:
        break;

        case SWORD:
            weapon = randBetween(5, 11);
            if (weapon <= 6)
                bufferedPrint("You find a sword, it's dull and rusty.\n");
            else if (weapon <= 8)
                bufferedPrint("You find a sword, it has seen some combat.\n");
            else if (weapon <= 10)
                bufferedPrint("You find a sword. It is brand new.\n");
        break;
        
        case AXE:     
        weapon = randBetween(30, 41);       
        if (weapon <= 33)
            bufferedPrint("You find an axe, it's dull and rusty.\n");
        else if (weapon <= 36)
            bufferedPrint("You find an axe, it has seen some combat.\n");
        else if (weapon <= 40)
            bufferedPrint("You find an axe. It is brand new.\n");
        break;
        
        case HEAL:
            player->health += randBetween(25,50);
            
            //health cannot go over 100
            normalize(&player->health, 0, 100, NULL, NULL);
            bufferedPrint("You feel better now. You have %d health.\n", player->health);
        
    }

    player->attack = weapon;
    player->position->looted = true;
}

void combat(Player* player)
{

    while(player->health > 0 && player->position->enemy.health > 0) 
    {
        int playerDamage = randBetween(player->attack / 2, player->attack * 2);
        int enemyDamage = randBetween(player->position->enemy.attack / 2, player->position->enemy.attack * 2);
        player->position->enemy.health -= playerDamage;
        bufferedPrint("You attack %s for %d damage.\n", player->position->enemy.name, playerDamage);
        if(player->position->enemy.health <= 0)
            break;

        bufferedPrint("%s attacks you for %d damage.\n", player->position->enemy.name, enemyDamage);
        player->health -= enemyDamage;
    }

    if(player->health <= 0) {
        bufferedPrint("The %s hits you and kills you. You lose.\n", player->position->enemy.name);
    } else {
        bufferedPrint("You hit the %s and he goes down to the floor, it is dead.\n", player->position->enemy.name);
    }
}

char askAction(Player* player) {
    bool stuck = false;
    if (player->position->enemy.health > 0)
    {
        stuck = true;
    }

    MenuOption options[] = {
        {"Go North", 'N', player->position->north != NULL && !stuck},
        {"Go South", 'S', player->position->south != NULL && !stuck},
        {"Go West",  'W', player->position->west != NULL && !stuck},
        {"Go East",  'E', player->position->east != NULL && !stuck},
        {"Take treasure", 'T', player->position->type == TREASURE_ROOM},
        {"Combat the enemy", 'C', player->position->type == COMBAT_ROOM && player->position->enemy.health > 0},
        {"Take weapon", 'P', player->position->type == WEAPON_ROOM && !player->position->looted},
        {"Heal", 'H', player->position->type == HEAL_ROOM && !player->position->looted},
        {"Quit", 'Q', true},
    };
    MenuOption* result = printMenu(ARRAY_SIZEOF(options), options);
    return result->symbol;
}
