#ifndef _BOTW_BACKEND_H_
#define _BOTW_BACKEND_H_

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

extern char itemName[200][30];
extern int quantItems[200];
extern int newQuantItems[200];
extern int numberOfItems;

extern int rupeeValue;
extern int rupees;

extern int numberOfWeapons;
extern int numberOfBows;
extern int numberOfShields;

extern int quantMod[200];
extern int new_quantMod[200];
extern int modNames[200];
extern int new_modNames[200];

extern int version;
extern FILE *fp;

int setFile(int intSlot);
void getData();
void writeFile();

#endif
