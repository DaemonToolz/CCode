#ifndef menu_type
#define menu_type
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
typedef struct {
    // variables
    char name[50];
    int  number;
    void (*function)();

    // display
    int x;
    int y;
    int length;
    int height;

    // Graphics
    SDL_Renderer *renderer;
    SDL_Texture  *text_texture;
    SDL_Rect      outter_rect;
    SDL_Rect      inner_rect;
    TTF_Font     *font;
    SDL_Surface  *text_surface;
    SDL_Surface  *outter_surface;
} menu_item_s;


void set_render_rectangle(menu_item_s* menu, SDL_Window* window){
    menu->outter_surface = SDL_GetWindowSurface(window);
    SDL_Rect rect;
    rect.x = menu->x;
    rect.y = menu->y;
    rect.w = menu->length;
    rect.h = menu->height;
    menu->outter_rect = rect;
}


void set_text_font(menu_item_s* menu){
    menu->font = TTF_OpenFont("fonts/FreeSans.ttf", 24);
}

void set_text_surface(menu_item_s* menu, int r, int g, int b, int a){
    SDL_Color textColor = {r, g, b, a};
    if(menu->text_surface != NULL){
        SDL_FreeSurface(menu->text_surface);
    }
    menu->text_surface = TTF_RenderText_Solid(menu->font, menu->name, textColor);
}

void set_text_texture(menu_item_s* menu) {
    int text_width;
    int text_height;

    if(menu->text_texture != NULL){
        SDL_DestroyTexture(menu->text_texture);
    }

    menu->text_texture = SDL_CreateTextureFromSurface(menu->renderer, menu->text_surface);
    text_width = menu->text_surface->w - 5;
    text_height = menu->text_surface->h - 5;
    menu->inner_rect.x = menu->x + 5;
    menu->inner_rect.y = menu->y + 5;
    menu->inner_rect.w = text_width;
    menu->inner_rect.h = text_height;
}

void free_all(menu_item_s* item){
    SDL_FreeSurface(item->text_surface);
    SDL_FreeSurface(item->outter_surface);
    SDL_DestroyTexture(item->text_texture);
}

void render_menu_item(menu_item_s* menu){
    SDL_FillRect(menu->outter_surface, &menu->outter_rect, SDL_MapRGBA( menu->outter_surface->format, 0, 0, 0, 0 ) );
    SDL_RenderFillRect(menu->renderer, &menu->outter_rect);
    SDL_RenderCopy(menu->renderer, menu->text_texture, NULL, &menu->inner_rect);
}

void set_renderer(menu_item_s* menu, SDL_Renderer* renderer){
    menu->renderer = renderer;
}

#endif