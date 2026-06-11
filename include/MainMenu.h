#ifndef _MAINMENU_H_
#define _MAINMENU_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "translations.h"
#include "botw_backend.h"

extern SDL_Window* window;
extern SDL_Surface* screenSurface;
extern SDL_Renderer* renderer;
extern TTF_Font *Arial, *Arial_S, *Arial_M;

void SDL_ClearScreen(SDL_Renderer* renderer, SDL_Color colour);
void SDL_DrawRect(SDL_Renderer* renderer, int x, int y, int w, int h, SDL_Color colour);
void SDL_DrawText(TTF_Font *font, int x, int y, SDL_Color colour, const char *text);
void setMenuItems(int active, int page);
void setPageBar(int page, int mPage);
void mainUI(int x, int currentPage, int maxPage);
void showItemSDL(int currentItem);
void selectSlotMenu(int slotchar);
void errorScreen();
void showCurrentRup();

#endif
