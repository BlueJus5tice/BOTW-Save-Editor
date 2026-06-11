#include "botw_backend.h"
#include "MainMenu.h"
#include "main.h"

char itemName[200][30];
long int quantItems[200];
long int newQuantItems[200];
int numberOfItems;

long int rupeeValue;
long int rupees;

int numberOfWeapons;
int numberOfBows;
int numberOfShields;

long int quantMod[200];
long int new_quantMod[200];
long int modNames[200];
long int new_modNames[200];

int version;
FILE *fp;

int buttons;
int currentPage;
int maxPage;
int currentState;
int slot;
int currentItem;
int maxSlot;

PadState pad;
