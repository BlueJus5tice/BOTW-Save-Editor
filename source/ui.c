#include "ui.h"
#include "save_io.h"

SDL_Window   *g_win;
SDL_Renderer *g_renderer;
TTF_Font     *g_fontL, *g_fontM, *g_fontS;

static SDL_Color clr(int r, int g, int b) {
    SDL_Color c = {r,g,b}; return c;
}
#define C_BG    clr(35,35,35)
#define C_BAR   clr(50,50,50)
#define C_WHITE clr(235,235,235)
#define C_BLUE  clr(0,220,180)
#define C_RED   clr(255,80,80)
#define C_GREEN clr(60,220,60)
#define C_PANEL clr(45,45,45)

void ui_init() {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    g_fontL = TTF_OpenFont("romfs:/arial.ttf", 36);
    g_fontM = TTF_OpenFont("romfs:/arial.ttf", 26);
    g_fontS = TTF_OpenFont("romfs:/arial.ttf", 20);
    SDL_CreateWindowAndRenderer(1280, 720, 0, &g_win, &g_renderer);
    SDL_SetRenderDrawBlendMode(g_renderer, SDL_BLENDMODE_BLEND);
}

void ui_quit() {
    TTF_CloseFont(g_fontL); TTF_CloseFont(g_fontM); TTF_CloseFont(g_fontS);
    TTF_Quit(); SDL_Quit();
}

void draw_clear(SDL_Color c) {
    SDL_SetRenderDrawColor(g_renderer, c.r, c.g, c.b, 255);
    SDL_RenderClear(g_renderer);
}

void draw_rect(int x, int y, int w, int h, SDL_Color c) {
    SDL_Rect r = {x,y,w,h};
    SDL_SetRenderDrawColor(g_renderer, c.r, c.g, c.b, 255);
    SDL_RenderFillRect(g_renderer, &r);
}

void draw_text(TTF_Font *f, int x, int y, SDL_Color c, const char *t) {
    if (!f || !t) return;
    SDL_Surface *s = TTF_RenderText_Blended_Wrapped(f, t, c, 1280);
    if (!s) return;
    SDL_Texture *tx = SDL_CreateTextureFromSurface(g_renderer, s);
    SDL_FreeSurface(s);
    if (!tx) return;
    SDL_Rect dst = {x, y, 0, 0};
    SDL_QueryTexture(tx, NULL, NULL, &dst.w, &dst.h);
    SDL_RenderCopy(g_renderer, tx, NULL, &dst);
    SDL_DestroyTexture(tx);
}

static void draw_bar(const char *title, const char *hint) {
    draw_rect(0,0,1280,80, C_BAR);
    draw_rect(20,80,1240,2, C_WHITE);
    draw_rect(0,650,1280,70, C_BAR);
    draw_rect(20,650,1240,2, C_WHITE);
    draw_text(g_fontL, 80, 18, C_WHITE, title);
    if (hint) draw_text(g_fontS, 900, 670, C_WHITE, hint);
}

void screen_slot(int slot) {
    draw_clear(C_BG);
    draw_bar("Wild Save Editor", "A Select   + Exit");
    draw_rect(380,180,520,320, C_PANEL);
    draw_text(g_fontM, 530, 220, C_WHITE, "Select Save Slot");
    char buf[32]; snprintf(buf,sizeof buf,"Slot %d", slot);
    draw_text(g_fontL, 560, 280, C_BLUE, buf);
    SDL_RenderPresent(g_renderer);
}

void screen_error() {
    draw_clear(C_BG);
    draw_bar("Wild Save Editor", NULL);
    draw_rect(340,260,600,180, C_PANEL);
    draw_text(g_fontM, 420, 290, C_RED, "Mount error — launch BOTW first");
    draw_text(g_fontS, 480, 340, C_WHITE, "Press + to exit");
    SDL_RenderPresent(g_renderer);
}

void screen_mainmenu(int sel) {
    draw_clear(C_BG);
    draw_bar("Wild Save Editor", "A Select   + Save & Exit");
    draw_rect(340,130,600,490, C_PANEL);
    const char *items[] = {"Inventory (Wpn/Bow/Shld)",
                           "Player Stats",
                           "Main Quests",
                           "Save & Exit"};
    int n = sizeof items / sizeof items[0];
    for (int i = 0; i < n; i++) {
        int y = 160 + i * 90;
        if (i == sel) {
            draw_rect(360, y, 560, 70, C_BAR);
            draw_rect(360, y, 6, 70, C_BLUE);
            draw_text(g_fontM, 430, y+18, C_BLUE, items[i]);
        } else {
            draw_text(g_fontM, 430, y+18, C_WHITE, items[i]);
        }
    }
    char rup[32]; snprintf(rup,sizeof rup,"Rupees: %d", g_newRupees);
    draw_text(g_fontS, 50, 18, C_WHITE, rup);
    SDL_RenderPresent(g_renderer);
}

void screen_itemlist(int sel, int page, int maxpage) {
    draw_clear(C_BG);
    draw_bar("Inventory", "A Select   B Back   + Save");
    draw_rect(0,82,400,568, C_BAR);
    int start = (page - 1) * 5;
    for (int i = 0; i < 5; i++) {
        int idx = start + i;
        if (idx >= g_itemCount) break;
        int y = 100 + i * 100;
        if (i == sel) draw_rect(5, y, 390, 85, C_PANEL);
        draw_text(g_fontS, 20, y+10, C_WHITE, translate(g_itemName[idx]));
        char q[16]; snprintf(q,sizeof q,"x%d", g_newQuant[idx]);
        draw_text(g_fontS, 320, y+10, C_BLUE, q);
    }

    if (start + sel < g_itemCount) {
        int idx = start + sel;
        int rx = 430, ry = 110;
        draw_rect(rx, ry-10, 800, 4, C_PANEL);
        draw_text(g_fontM, rx, ry, C_WHITE, translate(g_itemName[idx]));
        draw_rect(rx, ry+50, 800, 2, C_PANEL);
        char q[32]; snprintf(q,sizeof q,"Quantity: %d", g_newQuant[idx]);
        draw_text(g_fontM, rx, ry+70, C_WHITE, q);
        draw_rect(rx, ry+120, 800, 2, C_PANEL);
        char m[32]; snprintf(m,sizeof m,"Modifier: %d", g_newMod[idx + MAX_ITEMS/2]);
        draw_text(g_fontM, rx, ry+140, C_WHITE, m);
        draw_rect(rx, ry+190, 800, 2, C_PANEL);
        draw_text(g_fontS, rx, ry+210, C_RED, "A edit  B back");
    }

    char pg[16]; snprintf(pg,sizeof pg,"Page %d/%d",page,maxpage);
    draw_text(g_fontS, 20, 665, C_WHITE, pg);
    SDL_RenderPresent(g_renderer);
}

void screen_stats() {
    draw_clear(C_BG);
    draw_bar("Player Stats", "A Select   B Back");
    draw_rect(340,130,600,400, C_PANEL);

    draw_text(g_fontM, 370, 160, C_WHITE, "Rupees");
    char r[32]; snprintf(r,sizeof r,"%d", g_newRupees);
    draw_text(g_fontM, 750, 160, C_BLUE, r);

    draw_text(g_fontS, 370, 220, C_WHITE, "+ = Save & Exit");
    SDL_RenderPresent(g_renderer);
}

void screen_quests(int sel, int page, int maxpage) {
    draw_clear(C_BG);
    draw_bar("Main Quests", "A Toggle   B Back   + Save");
    draw_rect(0,82,1280,568, C_BAR);
    int start = (page - 1) * 6;
    for (int i = 0; i < 6; i++) {
        int idx = start + i;
        if (idx >= g_questCount) break;
        int y = 100 + i * 80;
        if (i == sel) draw_rect(10, y, 1260, 70, C_PANEL);
        draw_text(g_fontM, 30, y+15, C_WHITE, g_quests[idx]);
        draw_text(g_fontM, 1050, y+15,
                  g_newQuestFlag[idx] ? C_GREEN : C_RED,
                  g_newQuestFlag[idx] ? "COMPLETE" : "INCOMPLETE");
    }
    char pg[16]; snprintf(pg,sizeof pg,"Page %d/%d",page,maxpage);
    draw_text(g_fontS, 20, 665, C_WHITE, pg);
    SDL_RenderPresent(g_renderer);
}

void screen_editor(int val, int step) {
    draw_rect(350, 260, 580, 200, C_PANEL);
    draw_text(g_fontM, 400, 290, C_WHITE, "Edit Value");
    char v[32]; snprintf(v,sizeof v,"%d", val);
    draw_text(g_fontL, 560, 320, C_BLUE, v);
    char hint[64]; snprintf(hint, sizeof hint, "Left -%d    Right +%d    A OK", step, step);
    draw_text(g_fontS, 400, 380, C_RED, hint);
    SDL_RenderPresent(g_renderer);
}
