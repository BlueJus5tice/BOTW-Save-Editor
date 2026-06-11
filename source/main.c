#include "main.h"

void startSDLServices(){
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    Arial = TTF_OpenFont("romfs:/arial.ttf", 36);
    Arial_M = TTF_OpenFont("romfs:/arial.ttf", 28);
    Arial_S = TTF_OpenFont("romfs:/arial.ttf", 22);

    SDL_CreateWindowAndRenderer(1280, 720, 0, &window, &renderer);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    if(mountSaveData() == 0){
        currentState = -1;
        errorScreen();
    } else {
        maxSlot = 7;
        selectSlotMenu(slot);
    }
}

void initServices(){
    buttons = 0;
    slot = 0;
    currentPage = 1;
    currentState = 0;
    currentItem = 0;
    romfsInit();
    startSDLServices();
}

void setPages(){
    maxPage = numberOfItems / 5;
    if(numberOfItems % 5 != 0)
        maxPage++;
    if(maxPage == 0) maxPage = 1;
}

void MenuButtonsSlot(int x){
    switch(x){
        case 0:
            slot--;
            if(slot < 0) slot = maxSlot;
            break;
        case 1:
            slot++;
            if(slot > maxSlot) slot = 0;
            break;
    }
    selectSlotMenu(slot);
}

void MenuButtons(int x){
    switch(x){
        case 0:
            buttons--;
            if(buttons < 0){
                currentPage--;
                if(currentPage < 1) currentPage = maxPage;
                buttons = 4;
            }
            break;
        case 1:
            buttons++;
            if(buttons > 4){
                currentPage++;
                if(currentPage > maxPage) currentPage = 1;
                buttons = 0;
            }
            break;
        case 2:
            currentPage++;
            if(currentPage > maxPage) currentPage = 1;
            break;
        case 3:
            currentPage--;
            if(currentPage < 1) currentPage = maxPage;
            break;
    }
    currentItem = (5 * (currentPage - 1) + buttons);
    mainUI(buttons, currentPage, maxPage);
}

void adjustValue(int dir, int step){
    if(currentState == 2){
        newQuantItems[currentItem] += dir * step;
        if(newQuantItems[currentItem] < 0) newQuantItems[currentItem] = 0;
        if(newQuantItems[currentItem] > 9999) newQuantItems[currentItem] = 9999;
        showEditValue(currentItem);
        SDL_RenderPresent(renderer);
    } else if(currentState == 3){
        rupeeValue += dir * step;
        if(rupeeValue < 0) rupeeValue = 0;
        if(rupeeValue > 999999) rupeeValue = 999999;
        showRupeeEdit();
        SDL_RenderPresent(renderer);
    }
}

void ConfirmButton(){
    switch(currentState){
        case 0:
            if(setFile(slot)){
                setPages();
                currentState = 1;
                mainUI(buttons, currentPage, maxPage);
            } else {
                currentState = -1;
                errorScreen();
            }
            break;
        case 1:
            currentState = 2;
            showEditValue(currentItem);
            SDL_RenderPresent(renderer);
            break;
    }
}

void doSave(){
    if(fp){
        writeFile();
        fclose(fp);
        fp = NULL;
    }
}

void buttonLogic(int x){
    switch(currentState){
        case 0:
            MenuButtonsSlot(x);
            break;
        case 1:
            MenuButtons(x);
            break;
        case 2:
        case 3:
            if(x >= 0 && x <= 3){
                int step = (currentState == 2) ? 1 : 10;
                if(x == 0) adjustValue(-1, step);
                if(x == 1) adjustValue(1, step);
                if(x == 2) adjustValue(1, step * 10);
                if(x == 3) adjustValue(-1, step * 10);
            }
            break;
    }
}

int main(int argc, char **argv){
    initServices();

    padConfigureInput(1, HidNpadStyleSet_NpadStandard);
    padInitializeDefault(&pad);

    while(appletMainLoop()){
        padUpdate(&pad);
        u64 kDown = padGetButtonsDown(&pad);

        if(kDown & HidNpadButton_Up) buttonLogic(0);
        if(kDown & HidNpadButton_Down) buttonLogic(1);
        if(kDown & HidNpadButton_Right) buttonLogic(2);
        if(kDown & HidNpadButton_Left) buttonLogic(3);

        if(kDown & HidNpadButton_A) ConfirmButton();

        if(kDown & HidNpadButton_B){
            if(currentState == 1){
                currentState = 0;
                selectSlotMenu(slot);
            } else if(currentState == 2){
                currentState = 1;
                mainUI(buttons, currentPage, maxPage);
            } else if(currentState == 3){
                currentState = 1;
                mainUI(buttons, currentPage, maxPage);
            }
        }

        if(kDown & HidNpadButton_Y){
            if(currentState == 1){
                currentState = 3;
                showRupeeEdit();
                SDL_RenderPresent(renderer);
            }
        }

        if(kDown & HidNpadButton_Plus){
            if(currentState == 1 || currentState == 2 || currentState == 3)
                doSave();
            break;
        }
    }

    closeServices();
    return 0;
}

void closeServices(){
    if(fp) { fclose(fp); fp = NULL; }
    unmountSaveData();
    TTF_CloseFont(Arial);
    TTF_CloseFont(Arial_S);
    TTF_CloseFont(Arial_M);
    TTF_Quit();
    SDL_Quit();
    romfsExit();
}
