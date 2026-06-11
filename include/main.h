#ifndef MAIN_H
#define MAIN_H

#include <switch.h>

#define S_SLOT    0
#define S_MENU    1
#define S_ITEMS   2
#define S_STATS   3
#define S_QUESTS  4
#define S_EDITOR  5
#define S_ERROR  -1

#define MENU_ITEMS   1
#define MENU_STATS   2
#define MENU_QUESTS  3
#define MENU_SAVE    4

extern int g_state;
extern int g_sel;
extern int g_page;
extern int g_maxpage;
extern int g_slot;
extern int g_editVal;
extern int g_editStep;
extern int g_editTarget;
extern int g_editorType;
extern PadState g_pad;

#endif
