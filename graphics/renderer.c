#ifndef RENDERERS
#define RENDERERS
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include "../classes/menu.c"

void render_element(SDL_Window* window, SDL_Renderer* renderer, menu_item_s* item){
    SDL_Surface* surface = SDL_GetWindowSurface(window);
    SDL_Rect rect;
    rect.x = item->x;
    rect.y = item->y;
    rect.w = item->length;
    rect.h = item->height;
    SDL_FillRect(surface, &rect, SDL_MapRGBA( surface->format, 255, 0, 0, 255 ) );
    //SDL_UpdateWindowSurface(window);
    SDL_RenderFillRect(renderer, &rect);
}

void display_text(SDL_Renderer *renderer, int x, int y, char *text, TTF_Font *font, SDL_Texture **texture, SDL_Rect *rect) {
    int text_width;
    int text_height;
    SDL_Surface *surface;
    SDL_Color textColor = {0, 0, 0, 255};

    surface = TTF_RenderText_Solid(font, text, textColor);
    *texture = SDL_CreateTextureFromSurface(renderer, surface);

    text_width = surface->w;
    text_height = surface->h;
    SDL_FreeSurface(surface);
    rect->x = x;
    rect->y = y;
    rect->w = text_width;
    rect->h = text_height;
}
#endif