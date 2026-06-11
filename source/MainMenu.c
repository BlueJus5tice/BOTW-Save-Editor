#include "MainMenu.h"

SDL_Window* window;
SDL_Renderer* renderer;
TTF_Font *Arial, *Arial_S, *Arial_M;

static inline SDL_Color SDL_MakeColor(Uint8 r, Uint8 g, Uint8 b){
    SDL_Color colour = {r, g, b};
    return colour;
}

#define TOPBAR_COLOR SDL_MakeColor(45,45,45)
#define LEFTBAR_COLOR SDL_MakeColor(50,50,50)
#define BACKGROUND_COLOR SDL_MakeColor(40,40,40)
#define ITEM_COLOR SDL_MakeColor(47,47,47)
#define WHITE_TEXT_COLOR SDL_MakeColor(235,235,235)
#define WHITE_COLOR SDL_MakeColor(254,254,254)
#define BLUE_TEXT_COLOR SDL_MakeColor(1,254,203)
#define RED_TEXT_COLOR SDL_MakeColor(255,80,80)
#define GREEN_COLOR SDL_MakeColor(80,255,80)

void SDL_ClearScreen(SDL_Renderer* renderer, SDL_Color colour){
    SDL_SetRenderDrawColor(renderer, colour.r, colour.g, colour.b, 255);
    SDL_RenderClear(renderer);
}

void SDL_DrawRect(SDL_Renderer* renderer, int x, int y, int w, int h, SDL_Color colour){
    SDL_Rect rect = {x, y, w, h};
    SDL_SetRenderDrawColor(renderer, colour.r, colour.g, colour.b, 255);
    SDL_RenderFillRect(renderer, &rect);
}

void SDL_DrawText(TTF_Font *font, int x, int y, SDL_Color colour, const char *text){
    if(!font || !text) return;
    SDL_Surface *surface = TTF_RenderText_Blended_Wrapped(font, text, colour, 1280);
    if(!surface) return;
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if(!texture) return;
    SDL_Rect position = {x, y, 0, 0};
    SDL_QueryTexture(texture, NULL, NULL, &position.w, &position.h);
    SDL_RenderCopy(renderer, texture, NULL, &position);
    SDL_DestroyTexture(texture);
}

void selectSlotMenu(int slotchar){
    SDL_ClearScreen(renderer, BACKGROUND_COLOR);
    SDL_DrawRect(renderer,0,0,1280,88, TOPBAR_COLOR);
    SDL_DrawRect(renderer,30,87,1220,2, WHITE_COLOR);
    SDL_DrawRect(renderer,0,648,1280,72, TOPBAR_COLOR);
    SDL_DrawRect(renderer,30,648,1220,2, WHITE_COLOR);
    SDL_DrawText(Arial, 110, 27, WHITE_TEXT_COLOR, "BOTW Save Editor");
    SDL_DrawText(Arial_S, 900, 670, WHITE_TEXT_COLOR, "\x85:Select | +:Exit");

    SDL_DrawRect(renderer,380,148,517,400, LEFTBAR_COLOR);
    SDL_DrawText(Arial_S,554,200, WHITE_TEXT_COLOR, "Select File Slot:");

    char slotc[8];
    snprintf(slotc, sizeof slotc, "Slot %d", slotchar);
    SDL_DrawText(Arial_S, 632, 230, WHITE_TEXT_COLOR, slotc);

    SDL_RenderPresent(renderer);
}

void errorScreen(){
    SDL_ClearScreen(renderer, BACKGROUND_COLOR);
    SDL_DrawRect(renderer,0,0,1280,88, TOPBAR_COLOR);
    SDL_DrawRect(renderer,30,87,1220,2, WHITE_COLOR);
    SDL_DrawRect(renderer,0,648,1280,72, TOPBAR_COLOR);
    SDL_DrawRect(renderer,30,648,1220,2, WHITE_COLOR);
    SDL_DrawText(Arial, 110, 27, WHITE_TEXT_COLOR, "BOTW Save Editor");
    SDL_DrawRect(renderer,380,248,517,167, LEFTBAR_COLOR);
    SDL_DrawText(Arial_S,440,280, WHITE_TEXT_COLOR, "Error mounting/reading save!");
    SDL_DrawText(Arial_S,420,320, WHITE_TEXT_COLOR, "(Launch BOTW first, then retry)");
    SDL_RenderPresent(renderer);
}

void showCurrentRup(){
    char rupString[20];
    snprintf(rupString, sizeof rupString, "Rupees: %d", rupeeValue);
    SDL_DrawText(Arial_S, 1020, 25, WHITE_TEXT_COLOR, rupString);
}

void setPageBar(int page, int mPage){
    char pageText[20];
    snprintf(pageText, sizeof pageText, "Page %d/%d", page, mPage);
    SDL_DrawText(Arial_S, 30, 670, WHITE_TEXT_COLOR, pageText);
}

void setMenuItems(int active, int page){
    int offset = 100;
    for(int x = 0; x < 5; x++){
        int idx = (5 * (page - 1)) + x;
        if(idx < numberOfItems){
            if(x == active){
                SDL_DrawRect(renderer, 30, 25 + (offset * (x+1)), 350, 80, ITEM_COLOR);
                SDL_DrawRect(renderer, 30, 25 + (offset * (x+1)), 5, 80, BLUE_TEXT_COLOR);
                SDL_DrawText(Arial_S, 120, offset * (x+1) + 50, BLUE_TEXT_COLOR, translate(itemName[idx]));
            } else {
                SDL_DrawText(Arial_S, 120, offset * (x+1) + 50, WHITE_TEXT_COLOR, translate(itemName[idx]));
            }
        }
    }
}

void showItemSDL(int currentItem){
    SDL_DrawRect(renderer, 470, 140, 750, 3, LEFTBAR_COLOR);
    SDL_DrawText(Arial, 480, 150, WHITE_TEXT_COLOR, translate(itemName[currentItem]));
    SDL_DrawRect(renderer, 470, 200, 750, 3, LEFTBAR_COLOR);

    char value[25];
    snprintf(value, sizeof value, "%d", newQuantItems[currentItem]);

    SDL_DrawRect(renderer, 470, 240, 750, 3, LEFTBAR_COLOR);
    SDL_DrawText(Arial_M, 480, 252, WHITE_TEXT_COLOR, "Quantity:");
    SDL_DrawText(Arial_M, 1100, 252, BLUE_TEXT_COLOR, value);
    SDL_DrawRect(renderer, 470, 300, 750, 3, LEFTBAR_COLOR);

    char othervalue[25];
    snprintf(othervalue, sizeof othervalue, "%d", new_quantMod[currentItem]);
    SDL_DrawRect(renderer, 470, 320, 750, 3, LEFTBAR_COLOR);
    SDL_DrawText(Arial_M, 480, 332, WHITE_TEXT_COLOR, "Modifier:");
    SDL_DrawText(Arial_M, 1100, 332, BLUE_TEXT_COLOR, othervalue);
    SDL_DrawRect(renderer, 470, 380, 750, 3, LEFTBAR_COLOR);
}

void showEditValue(int currentItem){
    SDL_DrawRect(renderer, 470, 140, 750, 3, LEFTBAR_COLOR);
    SDL_DrawText(Arial, 480, 150, WHITE_TEXT_COLOR, translate(itemName[currentItem]));
    SDL_DrawRect(renderer, 470, 200, 750, 3, LEFTBAR_COLOR);

    char value[25];
    snprintf(value, sizeof value, "%d", newQuantItems[currentItem]);

    SDL_DrawRect(renderer, 470, 240, 750, 3, LEFTBAR_COLOR);
    SDL_DrawText(Arial_M, 480, 252, WHITE_TEXT_COLOR, "Quantity:");
    SDL_DrawRect(renderer, 530, 300, 200, 60, GREEN_COLOR);
    SDL_DrawText(Arial, 550, 310, WHITE_TEXT_COLOR, value);
    SDL_DrawRect(renderer, 470, 380, 750, 3, LEFTBAR_COLOR);

    SDL_DrawText(Arial_S, 500, 420, RED_TEXT_COLOR, "\x84: -10   \x85: +10");
    SDL_DrawText(Arial_S, 500, 450, RED_TEXT_COLOR, "\x86: -1    \x87: +1");
}

void showRupeeEdit(){
    SDL_DrawRect(renderer, 470, 140, 750, 3, LEFTBAR_COLOR);
    SDL_DrawText(Arial, 480, 150, WHITE_TEXT_COLOR, "Edit Rupees");
    SDL_DrawRect(renderer, 470, 200, 750, 3, LEFTBAR_COLOR);

    char value[25];
    snprintf(value, sizeof value, "%d", rupeeValue);

    SDL_DrawRect(renderer, 470, 240, 750, 3, LEFTBAR_COLOR);
    SDL_DrawText(Arial_M, 480, 252, WHITE_TEXT_COLOR, "Rupees:");
    SDL_DrawRect(renderer, 530, 300, 200, 60, GREEN_COLOR);
    SDL_DrawText(Arial, 550, 310, WHITE_TEXT_COLOR, value);
    SDL_DrawRect(renderer, 470, 380, 750, 3, LEFTBAR_COLOR);

    SDL_DrawText(Arial_S, 500, 420, RED_TEXT_COLOR, "\x84: -100  \x85: +100");
    SDL_DrawText(Arial_S, 500, 450, RED_TEXT_COLOR, "\x86: -10   \x87: +10");
}

void mainUI(int x, int currentPage, int maxPage){
    SDL_ClearScreen(renderer, BACKGROUND_COLOR);
    SDL_DrawRect(renderer,0,0,1280,88, TOPBAR_COLOR);
    SDL_DrawRect(renderer,30,87,1220,2, WHITE_COLOR);
    SDL_DrawRect(renderer,0,89,410,560, LEFTBAR_COLOR);
    SDL_DrawRect(renderer,410,89,880,560, BACKGROUND_COLOR);
    SDL_DrawRect(renderer,0,648,1280,72, TOPBAR_COLOR);
    SDL_DrawRect(renderer,30,648,1220,2, WHITE_COLOR);
    SDL_DrawText(Arial, 110, 27, WHITE_TEXT_COLOR, "BOTW Save Editor");
    SDL_DrawText(Arial_S, 620, 670, WHITE_TEXT_COLOR, "\x85:Select  B:Back  Y:Rupees  A:Edit  +:Save&Exit");

    setMenuItems(x, currentPage);
    showCurrentRup();
    setPageBar(currentPage, maxPage);

    int selectedItem = (5 * (currentPage - 1) + x);
    if(selectedItem >= 0 && selectedItem < numberOfItems){
        showItemSDL(selectedItem);
    }

    SDL_RenderPresent(renderer);
}
