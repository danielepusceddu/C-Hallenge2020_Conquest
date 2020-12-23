#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include "utility.h"
#include "conquest.h"

/*
"Welcome to Conquest! You're a pirate in a very far island, a map has brought you here and now you're on a quest to find a lost treasure in the dangerous dungeon of Kermit."
"You wake up in a cold jail room. The door is open."
"You enter a cold jail room which is familiar to you."
"You enter in a shiny room full of gold, there is a big chest with diamonds and rubies."
*/

void init_room(Room* room, Room* north, Room* south, Room* west, Room* east, RoomType type)
{
    room->type = type;
    room->visited = false;
    if (north != NULL)  // already null thanks to memset
    {
        room->north = north;
        north->south = room;
    }
    if (south != NULL)
    {
        room->south = south;
        south->north = room;
    }
    if (west != NULL)
    {
        room->west = west;
        west->east = room;
    }
    if (east != NULL)
    {
        room->east = east;
        east->west = room;
    }
}


/*
                                          room_giant3
                                             |
    TREASURE              room_giant1    HEAL - room_skeleton2 - room_skeleton3 
       |                      |               |          |
room_giant2 - machete1 -  room_skeleton1 - EMPTY - room_machete2   WEAPONS2
                |            |                                 
                HEAL        EMPTY - WEAPONS1                 
                             |
                            JAIL
*/

Room jail, treasure, empty1, room_skeleton1, room_giant1, room_machete1, room_giant2, weapon1, heal1, empty2, room_machete2, room_skeleton2, heal2, room_giant3, room_skeleton3, weapon2;
Enemy skeleton, skeleton_machete, giant;

void init(Player* player)
{
    srand(time(NULL));
    
    // Init player stats
    player->position = &jail;
    player->health = 100;
    player->attack = 1;  // Starts with fists
    
    // Init enemy
    skeleton.attack = 5;
    skeleton.health = 20;
    strcpy(skeleton.name, "Skeleton");

    skeleton_machete.attack = 10;
    skeleton_machete.health = 40;
    strcpy(skeleton_machete.name, "Skeleton with a machete");

    giant.attack = 20;
    giant.health = 100;
    strcpy(giant.name, "Giant");

    // Init rooms
    memset(&jail, 0, sizeof(jail));
    memset(&empty1, 0, sizeof(empty1));
    memset(&weapon1, 0, sizeof(weapon1));
    memset(&room_skeleton1, 0, sizeof(room_skeleton1));
    memset(&room_giant1, 0, sizeof(room_giant1));
    memset(&room_machete1, 0, sizeof(room_machete1));
    memset(&room_giant2, 0, sizeof(room_giant2));
    memset(&heal1, 0, sizeof(heal1));
    memset(&empty2, 0, sizeof(empty2));
    memset(&room_machete2, 0, sizeof(room_machete2));
    memset(&room_skeleton2, 0, sizeof(room_skeleton2));
    memset(&heal2, 0, sizeof(heal2));
    memset(&room_giant3, 0, sizeof(room_giant3));
    memset(&room_skeleton3, 0, sizeof(room_skeleton3));
    memset(&weapon2, 0, sizeof(weapon2));

    // Room messages
    strcpy(jail.firstVisit, "You wake up in a cold jail room. The door is open.");
    strcpy(jail.nextVisit, "You enter a cold jail room which is familiar to you.");
    
    strcpy(empty1.firstVisit, "You see an empty room.");
    strcpy(empty1.nextVisit, "You remember this empty room.");
    
    strcpy(weapon1.firstVisit, "You enter the room and see a table with a pointy knife on top of it.");
    strcpy(weapon1.nextVisit, "You see an empty table where the knife stood.");
    
    strcpy(room_skeleton1.firstVisit, "In the room there is a skeleton with a mace");
    strcpy(room_skeleton1.nextVisit, "You see skeleton bones all over the room.");

    strcpy(room_giant1.firstVisit, "You enter the room and you see a giant with a huge weapon.");
    strcpy(room_giant1.nextVisit, "You see the cold giant dead body of your enemy.");

    strcpy(room_machete1.firstVisit, "Standing in the room you find a skeleton, wielding a machete.");
    strcpy(room_machete1.nextVisit, "You see the the dead body of the skeleton. This time for real.");

    strcpy(room_giant2.firstVisit, "The room smells bad, an enormous man yells at you and waves his big mace.");
    strcpy(room_giant2.nextVisit, "The room floor is red with blood and there is a big corpse inside it.");

    strcpy(heal1.firstVisit, "You enter a room with a big fountain, its pure and clear water might cure your wounds.");
    strcpy(heal1.nextVisit, "You can see a big fountain in the room, you remember being here before.");

    strcpy(treasure.firstVisit, "You enter in a shiny room full of gold, there is a big chest with diamonds and rubies.");
    strcpy(treasure.nextVisit, "You see the treasure standing there. Just like you left it.");

    strcpy(empty2.firstVisit, "You see an empty room.");
    strcpy(empty2.nextVisit, "You remember this empty room.");

    strcpy(room_machete2.firstVisit, "The big cold room has a skeleton with a big machete on his hand");
    strcpy(room_machete2.nextVisit, "The cold room is silent, a big machete lays on the floor next to a pile of bones");

    strcpy(room_skeleton2.firstVisit, "As you enter the room you are greeted, so to say, by a skeleton");
    strcpy(room_skeleton2.nextVisit, "The corpse of the skeleton lays in the middle of the room");

    strcpy(room_skeleton3.firstVisit, "Standing in the room you find a skeleton, wielding a sword.");
    strcpy(room_skeleton3.nextVisit, "You see the the dead body of the skeleton. This time for real.");

    strcpy(heal2.firstVisit, "You see a green fountain.");
    strcpy(heal2.nextVisit, "You remember seeing this green fountain before.");

    strcpy(room_giant3.firstVisit, "You enter the room and you are shaded by the big silouhette of a man with a giant axe.");
    strcpy(room_giant3.nextVisit, "The rooms smells like blood, you can see the dead body of your enemy on the wall.");
    
    strcpy(room_skeleton3.firstVisit, "Standing in the room you find a skeleton, wielding a sword.");
    strcpy(room_skeleton3.nextVisit, "You see the the dead body of the skeleton. This time for real.");

    strcpy(weapon2.firstVisit, "You enter what seems to be an armory and you see a large chest in the corner of this room.");
    strcpy(weapon2.nextVisit, "You remember this armory.");

    // Room directions
    init_room(&jail, &empty1, NULL, NULL, NULL, JAIL_ROOM);
    init_room(&empty1, &room_skeleton1, NULL, NULL, &weapon1, EMPTY_ROOM); 
    init_room(&weapon1, NULL, NULL, NULL, NULL, WEAPON_ROOM); 
    init_room(&room_skeleton1, &room_giant1, NULL, &room_machete1, &empty2, COMBAT_ROOM); 
    init_room(&room_giant1, NULL, NULL, NULL, NULL, COMBAT_ROOM); 
    init_room(&room_machete1, NULL, &heal1, &room_giant2, NULL, COMBAT_ROOM); 
    init_room(&heal1, NULL, NULL, NULL, NULL, HEAL_ROOM);
    init_room(&room_giant2, &treasure, NULL, NULL, NULL, COMBAT_ROOM);
    init_room(&treasure, NULL, NULL, NULL, NULL, TREASURE_ROOM); 
    init_room(&empty2, NULL, NULL, NULL, &room_machete2, EMPTY_ROOM);
    init_room(&room_machete2, &room_skeleton2, NULL, NULL, NULL, COMBAT_ROOM);
    init_room(&room_skeleton2, &room_giant3, NULL, &heal2, &room_skeleton3, COMBAT_ROOM);
    init_room(&heal2, NULL, NULL, NULL, NULL, HEAL_ROOM);
    init_room(&room_giant3, NULL, NULL, NULL, NULL, COMBAT_ROOM);
    init_room(&room_skeleton3, NULL, &weapon2, NULL, NULL, COMBAT_ROOM);
    init_room(&weapon2, NULL, NULL, NULL, NULL, WEAPON_ROOM); 
 
    // Init loot and enemi
    weapon1.loot = SWORD;
    weapon2.loot = AXE;

    heal1.loot = HEAL;
    heal2.loot = HEAL;

    room_skeleton1.enemy = skeleton;
    room_skeleton2.enemy = skeleton;
    room_skeleton3.enemy = skeleton;
        
    room_giant1.enemy = giant;
    room_giant2.enemy = giant;
    room_giant3.enemy = giant;
    
    room_machete1.enemy = skeleton_machete;
    room_machete2.enemy = skeleton_machete;
}

int main()
{
    Player player;
    bool running = true;
    bool win = false;
    char choice;
    init(&player);
    printf("Welcome to Conquest! You're a pirate in a very far island, a map has brought you here and now you're on a quest to find a lost treasure in the dangerous dungeon of Kermit.\n");
    visitRoom(&jail);
    while (running && !win && player.health>0)
    {
        printf("\n");
        printStatus(&player);
        bufferedPrint(NULL); //Print messages of the turn before
        printf("\n");
        
        choice = askAction(&player);
        switch (choice)
        {
            case 'N':
            case 'S':
            case 'W':
            case 'E':
                goTo(&player, choice);
                break;
            case 'C':
                combat(&player);
                break;
            case 'P':
            case 'H':
                loot(&player);
                break;
            case 'T':
                win = true;
                break;
            case 'Q':
                running = false;    
                break;         
        }
    }
    bufferedPrint(NULL);

    if (win)
    {
        printf("You open the treasure. You are rich!\n");
    }
    else if(!running)
    {
        printf("Goodbye adventurer.\n");
    } else if(player.health <= 0)
    {
        printf("You died, Game Over!\n");
    }

    printf("Your adventure has come to an end.\n");
    
    return 0;
}
