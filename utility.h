#ifndef __UTILITY__H__
#define __UTILITY__H__

#define ARRAY_SIZEOF(x) sizeof(x)/sizeof(x[0])

typedef struct {
    char* name;
    char* symbol;
    int maxSymbols;
    int max;
    int value;
} StatusEntry;

typedef struct 
{
    const char const* optText;
    char symbol;
    bool enabled;
} MenuOption;

void bufferedPrint(const char* format, ...);
MenuOption* printMenu(int num, MenuOption* options);void clearBuffer();
char toLower(char c);
char toUpper(char c);
long min(long x, long y);
int randBetween(int min, int max);
bool normalize(int *value, int min, int max, int *carryDown, int *carryUp);
void drawStatus(int len, StatusEntry status[]);

#endif