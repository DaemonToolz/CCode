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

    // display
    int x;
    int y;
    int length;
    int height;

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
} menu_item_s;

void set_menu_item_polygon_coordiantes(menu_item_s* menu, SDL_Vertex* vertex, int x, int y, int r, int g, int b, int a){
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

void set_menu_render_shape(menu_item_s* menu){
    // {SDL_FPoint, SDL_Color, SDL_FPoint}
    menu->_indices[0] = 0;
    menu->_indices[1] = 1;
    menu->_indices[2] = 2;
    menu->_indices[3] = 2;
    menu->_indices[4] = 3;
    menu->_indices[5] = 0;

    // 
    set_menu_item_polygon_coordiantes(menu, &menu->outer_shape[0], menu->x, menu->y,                                          menu->r,menu->g,menu->b,255);
    set_menu_item_polygon_coordiantes(menu, &menu->outer_shape[1], menu->x + menu->length, menu->y,                           menu->r,menu->g,menu->b,255);
    set_menu_item_polygon_coordiantes(menu, &menu->outer_shape[2], menu->x + menu->length + 50, menu->y + menu->height,       menu->r,menu->g,menu->b,255);
    set_menu_item_polygon_coordiantes(menu, &menu->outer_shape[3], menu->x, menu->y + menu->height,                           menu->r,menu->g,menu->b,255);

    //
    set_menu_item_polygon_coordiantes(menu, &menu->shadow_shape[0], menu->x, menu->y,                                         50,50,50,50);
    set_menu_item_polygon_coordiantes(menu, &menu->shadow_shape[1], menu->x + menu->length, menu->y,                          50,50,50,50);
    set_menu_item_polygon_coordiantes(menu, &menu->shadow_shape[2], menu->x + menu->length + 50, menu->y + menu->height + 7,  50,50,50,50);
    set_menu_item_polygon_coordiantes(menu, &menu->shadow_shape[3], menu->x, menu->y + menu->height + 7,                      50,50,50,50);
}

void set_menu_text_font(menu_item_s* menu){
    menu->font = TTF_OpenFont("fonts/FreeSans.ttf", 24);
}

void set_menu_text_surface(menu_item_s* menu, int r, int g, int b, int a){
    SDL_Color textColor = {r, g, b, a};
    if(menu->text_surface != NULL){
        SDL_FreeSurface(menu->text_surface);
    }
    menu->text_surface = TTF_RenderText_Solid(menu->font, menu->name, textColor);
}

void set_menu_text_texture(menu_item_s* menu) {
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

void menu_free_all(menu_item_s* item){
    SDL_FreeSurface(item->text_surface);
    SDL_DestroyTexture(item->text_texture);
}

void render_menu_item(menu_item_s* menu){
    SDL_RenderGeometry( menu->renderer, 
        NULL, 
        menu->shadow_shape, 
        (int)COMPUTE_ARRAY_SIZE(menu->shadow_shape), 
        menu->_indices, 
        (int)COMPUTE_ARRAY_SIZE(menu->_indices)
    );

    SDL_RenderGeometry( menu->renderer, 
        NULL, 
        menu->outer_shape, 
        (int)COMPUTE_ARRAY_SIZE(menu->outer_shape), 
        menu->_indices, 
        (int)COMPUTE_ARRAY_SIZE(menu->_indices)
    );

    SDL_RenderCopy(menu->renderer, 
        menu->text_texture,
        NULL, 
        &menu->inner_rect
    );
}

void set_menu_renderer(menu_item_s* menu, SDL_Renderer* renderer){
    menu->renderer = renderer;
}

#endif