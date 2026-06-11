#ifndef _main_h_
#define _main_h_

#include <switch.h>
#include <stdio.h>
#include <stdlib.h>
#include "MainMenu.h"
#include "botw_backend.h"
#include "mount.h"

int buttons;
int currentPage;
int maxPage;
int currentState;
int slot;
int currentItem;
int maxSlot;

PadState pad;

int main(int argc, char **argv);
void startSDLServices();
void initServices();
void buttonLogic(int x);
void closeServices();
void MenuButtons(int x);
void ConfirmButton();
void setPages();
void MenuButtonsSlot(int x);

#endif
