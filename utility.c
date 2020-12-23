#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>
#include <math.h>
#include "utility.h"

void printOption(const char const* optText, char symbol);
MenuOption* findOption(int num, MenuOption* options, char choice);

bool normalize(int *value, int min, int max, int *carryDown, int *carryUp)
{
    if (*value > max)
    {
        if (carryUp)
        {
            *carryUp += *value-max;
        }
        *value = max;
    }
    else if (*value < min)
    {
        if (carryDown)
        {
            *carryDown += *value-min;
        }
        *value = min;
    }
}

void drawStatus(int len, StatusEntry status[])
{
    int width, numWidth, extraSpaces, numSymbols, totWidth = 1;
    for (int i = 0; i < len; i++)
    {
        width = 2 + status[i].maxSymbols * (strlen(status[i].symbol) + 1);
        totWidth += width;
        for (int j = 0; j < width; j++)
            printf("-");
    }
    printf("-\n");
    for (int i = 0; i < len; i++)
    {
        width = 1 + status[i].maxSymbols * (strlen(status[i].symbol) + 1);
        numWidth = (int) floor(log10(status[i].max) + 1);
        extraSpaces = width - strlen(status[i].name) - numWidth - 4;
        assert(extraSpaces >= 0);
        printf("| %s: %*d %*s", status[i].name, -numWidth, status[i].value, extraSpaces, "");
    }
    printf("|\n");
    for (int i = 0; i < len; i++)
    {
        numSymbols = status[i].value * status[i].maxSymbols / status[i].max;
        printf("| ");
        for (int j = 0; j < status[i].maxSymbols; j++)
        {
            if (j < numSymbols)
                printf("%s ", status[i].symbol);
            else
                printf("%*s ", strlen(status[i].symbol), "");
        }
    }
    printf("|\n");
    for (int i = 0; i < totWidth; i++)
        printf("-");
    printf("\n");
}

int randBetween(int min, int max)
{
  return min + rand()%(max-min);
}


long min(long x, long y){
    return (x < y) ? x : y;
}

char toLower(char c){
    return (c >= 'A' && c <= 'Z') ? c - ('A' - 'a') : c;
}

char toUpper(char c){
    return (c >= 'a' && c <= 'z') ? c - ('a' - 'A') : c;
}


// Toglie l'andata a capo e altri caratteri indesiderati nel buffer
void clearBuffer(){
    int result = 'a'; //getchar restituisce un intero
    while(result != EOF && result != '\n'){
        result = getchar();
    }
}

MenuOption* findOption(int num, MenuOption* options, char choice)
{
    MenuOption* result = NULL;
    
    for(int i = 0; i < num && !result; i++){
        if(toLower(options[i].symbol) == toLower(choice) && options[i].enabled){
            result = options + i;
        }
    }

    return result;
}

void printOption(const char const* optText, char symbol)
{
    char *pos1 = strchr(optText, toUpper(symbol)); // ptr to Upper symbol
    char *pos2 = strchr(optText, toLower(symbol)); // ptr to Lower symbol
    char *symbolPos; // ptr to first of pos1 pos2 symbols

    // Get first occurrance of character
    if (pos1 == NULL)
        symbolPos = pos2;
    else if (pos2 == NULL)
        symbolPos = pos1;
    else
        symbolPos = (char*)min((long)pos1, (long)pos2);

    if (symbolPos) // if symbol found
    {
        fwrite(optText, sizeof(char), symbolPos-optText, stdout); // print upto symbol
        printf("[%c]", *symbolPos); // print symbol
        printf("%s\n", symbolPos+1); // print last part of string
    }
    else // If symbol not found
    {
        printf("[%c]%s\n", symbol, optText);
    }
}

MenuOption* printMenu(int num, MenuOption* options)
{
    char choice;
    MenuOption* chosenOption;
    
    for(int i = 0; i < num; i++){
        if(options[i].enabled){
            printOption(options[i].optText, options[i].symbol);
        }
    }
    
    do
    {
        printf("> ");
        scanf("%c", &choice);
        clearBuffer();
        chosenOption = findOption(num, options, choice);
    }
    while(!chosenOption);
    
    return chosenOption;
}

void bufferedPrint(const char* format, ...)
{
    #define NEXT_MSG_SIZE 4096
    static char message[NEXT_MSG_SIZE];
    
    char newMessage[NEXT_MSG_SIZE];
    va_list valist;

    if (format)
    {
        va_start(valist, format);
        vsnprintf(newMessage, NEXT_MSG_SIZE, format, valist);
        strncat(message, newMessage, NEXT_MSG_SIZE);
    }
    else
    {
        printf("%s", message);
        message[0] = '\0';
    }
}
