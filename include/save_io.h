#ifndef SAVE_IO_H
#define SAVE_IO_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_ITEMS 200
#define MAX_QUESTS 50
#define VERSIONS 7

extern int g_version;
extern FILE *g_fp;

extern char g_itemName[MAX_ITEMS][30];
extern int g_quant[MAX_ITEMS];
extern int g_newQuant[MAX_ITEMS];
extern int g_mod[MAX_ITEMS];
extern int g_newMod[MAX_ITEMS];
extern int g_itemCount;
extern int g_weaponCount;
extern int g_bowCount;
extern int g_shieldCount;

extern int g_rupees;
extern int g_newRupees;

extern char g_quests[MAX_QUESTS][64];
extern int g_questFlag[MAX_QUESTS];
extern int g_newQuestFlag[MAX_QUESTS];
extern int g_questCount;

int load_save(int slot);
void save_changes();
const char *translate(const char *raw);

#endif
