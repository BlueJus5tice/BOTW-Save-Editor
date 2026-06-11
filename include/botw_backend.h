#ifndef _BOTW_BACKEND_H_
#define _BOTW_BACKEND_H_

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

extern char itemName[200][30];
extern long int quantItems[200];
extern long int newQuantItems[200];
extern int numberOfItems;

extern long int rupeeValue;
extern long int rupees;

extern int numberOfWeapons;
extern int numberOfBows;
extern int numberOfShields;

extern long int quantMod[200];
extern long int new_quantMod[200];
extern long int modNames[200];
extern long int new_modNames[200];

extern int version;
extern FILE *fp;

int setFile(int intSlot);
void getData();
void writeFile();

#endif
