#ifndef UI_H
#define UI_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

extern SDL_Window *g_win;
extern SDL_Renderer *g_renderer;
extern TTF_Font *g_fontL, *g_fontM, *g_fontS;

void ui_init();
void ui_quit();

void draw_clear(SDL_Color c);
void draw_rect(int x, int y, int w, int h, SDL_Color c);
void draw_text(TTF_Font *f, int x, int y, SDL_Color c, const char *t);
void draw_topbar(const char *title);
void draw_bottombar(const char *hint);

void screen_slot(int slot);
void screen_error();
void screen_mainmenu(int sel);
void screen_itemlist(int sel, int page, int maxpage);
void screen_stats();
void screen_quests(int sel, int page, int maxpage);
void screen_editor(int val, int step);

#endif
