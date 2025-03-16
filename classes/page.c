#ifndef menu_type
#define menu_type
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../meta/preprocessors.c"

typedef struct {
    // variables
    char name[50];
    int  number;
    void (*function)();

    // Graphics
    SDL_Renderer *renderer;

    SDL_Texture  *text_texture;
    SDL_Surface  *text_surface;
    SDL_Rect      inner_rect;
    TTF_Font     *font;
    
    SDL_Rect     image_rect;
    SDL_Texture* image_texture;
} page_item_s;

#endif