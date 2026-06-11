#include "botw_backend.h"
#include "MainMenu.h"
#include "main.h"

char itemName[200][30];
int quantItems[200];
int newQuantItems[200];
int numberOfItems;

int rupeeValue;
int rupees;

int numberOfWeapons;
int numberOfBows;
int numberOfShields;

int quantMod[200];
int new_quantMod[200];
int modNames[200];
int new_modNames[200];

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
