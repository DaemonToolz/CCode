#ifndef save_type
#define save_type
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../meta/preprocessors.c"
#include "../meta/globals.c"
typedef struct {
    // variables
    char name[50];
    int  number;
    void (*function)(void*);

    // display
    int x;
    int y;
    int length;
    int height;

    int pX;
    int pY;

    // Colors
    int r;
    int g;
    int b;
    int a;

    // Graphics
    SDL_Vertex    outer_shape[4];
    SDL_Renderer *renderer;
    SDL_Texture  *text_texture;
    
    SDL_Rect      inner_rect;
    TTF_Font     *font;
    SDL_Surface  *text_surface;
    
    SDL_Vertex    shadow_shape[4];
    int _indices[6];
} save_item_s;

void set_save_item_polygon_coordiantes(save_item_s* save, SDL_Vertex* vertex, int x, int y, int r, int g, int b, int a){
    // Position
    vertex->position.x = x;
    vertex->position.y = y;
    // Colors
    vertex->color.r = r;
    vertex->color.g = g;
    vertex->color.b = b;
    vertex->color.a = a;
    // Text
    vertex->tex_coord.x = 1;
    vertex->tex_coord.y = 1;
}

void set_save_render_shape(save_item_s* save){
    // {SDL_FPoint, SDL_Color, SDL_FPoint}
    save->_indices[0] = 0;
    save->_indices[1] = 1;
    save->_indices[2] = 2;
    save->_indices[3] = 2;
    save->_indices[4] = 3;
    save->_indices[5] = 0;

    // 
    set_save_item_polygon_coordiantes(save, &save->outer_shape[0], save->x, save->y,                                          save->r,save->g,save->b,255);
    set_save_item_polygon_coordiantes(save, &save->outer_shape[1], save->x + save->length, save->y,                           save->r,save->g,save->b,255);
    set_save_item_polygon_coordiantes(save, &save->outer_shape[2], save->x + save->length, save->y + save->height,            save->r,save->g,save->b,255);
    set_save_item_polygon_coordiantes(save, &save->outer_shape[3], save->x, save->y + save->height,                           save->r,save->g,save->b,255);

    //
    set_save_item_polygon_coordiantes(save, &save->shadow_shape[0], save->x, save->y,                                         50,50,50,50);
    set_save_item_polygon_coordiantes(save, &save->shadow_shape[1], save->x + save->length + 7, save->y,                      50,50,50,50);
    set_save_item_polygon_coordiantes(save, &save->shadow_shape[2], save->x + save->length + 7, save->y + save->height + 7,   50,50,50,50);
    set_save_item_polygon_coordiantes(save, &save->shadow_shape[3], save->x, save->y + save->height + 7,                      50,50,50,50);
}

void set_save_text_font(save_item_s* save){
    save->font = TTF_OpenFont("fonts/FreeSans.ttf", 24);
}

void set_save_text_surface(save_item_s* save, int r, int g, int b, int a){
    SDL_Color textColor = {r, g, b, a};
    if(save->text_surface != NULL){
        SDL_FreeSurface(save->text_surface);
    }
    save->text_surface = TTF_RenderText_Solid(save->font, save->name, textColor);
}

void set_save_text_texture(save_item_s* save) {
    int text_width;
    int text_height;

    if(save->text_texture != NULL){
        SDL_DestroyTexture(save->text_texture);
    }

    save->text_texture = SDL_CreateTextureFromSurface(save->renderer, save->text_surface);
    text_width = save->text_surface->w - 5;
    text_height = save->text_surface->h - 5;
    save->inner_rect.x = save->x + 5;
    save->inner_rect.y = save->y + 5;
    save->inner_rect.w = text_width;
    save->inner_rect.h = text_height;
}

void save_free(save_item_s* item){
    SDL_FreeSurface(item->text_surface);
    SDL_DestroyTexture(item->text_texture);
}


void render_save_item(save_item_s* save){
    SDL_RenderGeometry( save->renderer, 
        NULL, 
        save->shadow_shape, 
        (int)COMPUTE_ARRAY_SIZE(save->shadow_shape), 
        save->_indices, 
        (int)COMPUTE_ARRAY_SIZE(save->_indices)
    );

    SDL_RenderGeometry( save->renderer, 
        NULL, 
        save->outer_shape, 
        (int)COMPUTE_ARRAY_SIZE(save->outer_shape), 
        save->_indices, 
        (int)COMPUTE_ARRAY_SIZE(save->_indices)
    );

    SDL_RenderCopy(save->renderer, 
        save->text_texture,
        NULL, 
        &save->inner_rect
    );
}

void set_save_renderer(save_item_s* save, SDL_Renderer* renderer){
    save->renderer = renderer;
}

#endif