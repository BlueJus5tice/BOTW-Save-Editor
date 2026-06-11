#include <switch.h>
#include "main.h"
#include "mount.h"
#include "save_io.h"
#include "ui.h"



/* ---- helpers ---- */
static int maxp(int n, int per) {
    int p = n / per;
    if (n % per) p++;
    return p > 0 ? p : 1;
}

static void go_slot() {
    g_state = S_SLOT; g_slot = 0;
    screen_slot(g_slot);
}

/* ---- editor ---- */
static void open_editor(int val, int step) {
    g_state = S_EDITOR;
    g_editVal = val;
    g_editStep = step;
    screen_editor(g_editVal, g_editStep);
}

static void editor_adj(int dir) {
    g_editVal += dir * g_editStep;
    if (g_editVal < 0) g_editVal = 0;
    screen_editor(g_editVal, g_editStep);
}

/* ---- nav helpers ---- */
static void slot_nav(int dir) {
    g_slot += dir;
    if (g_slot < 0) g_slot = 7;
    if (g_slot > 7) g_slot = 0;
    screen_slot(g_slot);
}

static void menu_nav(int dir) {
    g_sel += dir;
    if (g_sel < 0) g_sel = 3;
    if (g_sel > 3) g_sel = 0;
    screen_mainmenu(g_sel);
}

static void list_nav(int dir, int n, int per) {
    g_sel += dir;
    if (g_sel < 0) { g_sel = per - 1; g_page--; }
    if (g_sel >= per) { g_sel = 0; g_page++; }
    if (g_page < 1) g_page = g_maxpage;
    if (g_page > g_maxpage) g_page = 1;
    // clamp sel to actual items on last page
    int start = (g_page - 1) * per;
    while (start + g_sel >= n && g_sel > 0) g_sel--;
}

/* ---- main ---- */
int main(int argc, char **argv) {
    romfsInit();
    ui_init();

    if (!mount_save()) {
        g_state = S_ERROR;
        screen_error();
    } else {
        go_slot();
    }

    padConfigureInput(1, HidNpadStyleSet_NpadStandard);
    padInitializeDefault(&g_pad);

    while (appletMainLoop()) {
        padUpdate(&g_pad);
        u64 k = padGetButtonsDown(&g_pad);

        if (k & HidNpadButton_Plus) {
            if (g_state == S_ITEMS || g_state == S_STATS || g_state == S_QUESTS) {
                save_changes();
            }
            break;
        }

        switch (g_state) {

        case S_SLOT:
            if (k & HidNpadButton_Up)   slot_nav(-1);
            if (k & HidNpadButton_Down) slot_nav(1);
            if (k & HidNpadButton_A) {
                if (load_save(g_slot)) {
                    g_state = S_MENU; g_sel = 0;
                    screen_mainmenu(g_sel);
                } else {
                    g_state = S_ERROR;
                    screen_error();
                }
            }
            break;

        case S_MENU:
            if (k & HidNpadButton_Up)    menu_nav(-1);
            if (k & HidNpadButton_Down)  menu_nav(1);
            if (k & HidNpadButton_A) {
                if (g_sel == 0) { // inventory
                    g_state = S_ITEMS; g_sel = 0; g_page = 1;
                    g_maxpage = maxp(g_itemCount, 5);
                    screen_itemlist(g_sel, g_page, g_maxpage);
                } else if (g_sel == 1) { // stats
                    g_state = S_STATS; g_sel = 0;
                    screen_stats();
                } else if (g_sel == 2) { // quests
                    g_state = S_QUESTS; g_sel = 0; g_page = 1;
                    g_maxpage = maxp(g_questCount, 6);
                    screen_quests(g_sel, g_page, g_maxpage);
                } else if (g_sel == 3) { // save & exit
                    save_changes();
                    goto done;
                }
            }
            if (k & HidNpadButton_B) { go_slot(); }
            break;

        case S_ITEMS:
            if (k & HidNpadButton_Up)   { list_nav(-1,g_itemCount,5); screen_itemlist(g_sel,g_page,g_maxpage); }
            if (k & HidNpadButton_Down) { list_nav(1,g_itemCount,5);  screen_itemlist(g_sel,g_page,g_maxpage); }
            if (k & HidNpadButton_Right){ g_page++; if(g_page>g_maxpage)g_page=1; g_sel=0; screen_itemlist(g_sel,g_page,g_maxpage); }
            if (k & HidNpadButton_Left) { g_page--; if(g_page<1)g_page=g_maxpage; g_sel=0; screen_itemlist(g_sel,g_page,g_maxpage); }
            if (k & HidNpadButton_A) {
                int idx = (g_page-1)*5 + g_sel;
                if (idx < g_itemCount) {
                    g_editorType = 0; // quantity
                    g_editTarget = idx;
                    open_editor(g_newQuant[idx], 1);
                }
            }
            if (k & HidNpadButton_B) { g_state = S_MENU; g_sel = 0; screen_mainmenu(g_sel); }
            break;

        case S_STATS:
            if (k & HidNpadButton_A) {
                g_editorType = 1; // rupee
                g_editTarget = 0;
                open_editor(g_newRupees, 10);
            }
            if (k & HidNpadButton_B) { g_state = S_MENU; g_sel = 1; screen_mainmenu(g_sel); }
            break;

        case S_QUESTS:
            if (k & HidNpadButton_Up)   { list_nav(-1,g_questCount,6); screen_quests(g_sel,g_page,g_maxpage); }
            if (k & HidNpadButton_Down) { list_nav(1,g_questCount,6);  screen_quests(g_sel,g_page,g_maxpage); }
            if (k & HidNpadButton_Right){ g_page++; if(g_page>g_maxpage)g_page=1; g_sel=0; screen_quests(g_sel,g_page,g_maxpage); }
            if (k & HidNpadButton_Left) { g_page--; if(g_page<1)g_page=g_maxpage; g_sel=0; screen_quests(g_sel,g_page,g_maxpage); }
            if (k & HidNpadButton_A) {
                int idx = (g_page-1)*6 + g_sel;
                if (idx < g_questCount) {
                    g_newQuestFlag[idx] = !g_newQuestFlag[idx];
                    screen_quests(g_sel, g_page, g_maxpage);
                }
            }
            if (k & HidNpadButton_B) { g_state = S_MENU; g_sel = 2; screen_mainmenu(g_sel); }
            break;

        case S_EDITOR:
            if (k & HidNpadButton_Up)    editor_adj(1);
            if (k & HidNpadButton_Down)  editor_adj(-1);
            if (k & HidNpadButton_Right) { int save = g_editStep; g_editStep*=10; editor_adj(1); g_editStep = save; }
            if (k & HidNpadButton_Left)  { int save = g_editStep; g_editStep*=10; editor_adj(-1); g_editStep = save; }
            if (k & HidNpadButton_A) {
                if (g_editorType == 0) { // item quantity
                    int idx = g_editTarget;
                    g_newQuant[idx] = g_editVal;
                    g_state = S_ITEMS;
                    screen_itemlist(g_sel, g_page, g_maxpage);
                } else if (g_editorType == 1) { // rupee
                    g_newRupees = g_editVal;
                    g_state = S_STATS;
                    screen_stats();
                }
            }
            if (k & HidNpadButton_B) {
                if (g_editorType == 0) {
                    g_state = S_ITEMS;
                    screen_itemlist(g_sel, g_page, g_maxpage);
                } else if (g_editorType == 1) {
                    g_state = S_STATS;
                    screen_stats();
                }
            }
            break;

        case S_ERROR:
            break;
        }
    }

done:
    if (g_fp) { fclose(g_fp); g_fp = NULL; }
    unmount_save();
    ui_quit();
    romfsExit();
    return 0;
}
