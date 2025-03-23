#ifndef character_type
#define character_type
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include "../meta/preprocessors.c"
#include "item.c"
#include <stdio.h>

typedef enum {
    ALLY  = 0,
    ENEMY = 1
} character_enum;

typedef struct {
    // variables
    char   name[50];
    int    number;
    item_s items[6];

    // display
    int x;
    int y;

    // Graphics
    SDL_Vertex    outer_shape[4];
    SDL_Renderer *renderer;

    int _indices[6];
} character_template_s;

void set_character_item_polygon_coordiantes(character_template_s* character, SDL_Vertex* vertex, int x, int y, int r, int g, int b, int a){
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



void set_character_render_shape(character_template_s* character, character_enum type, int mx, int my){
    // {SDL_FPoint, SDL_Color, SDL_FPoint}
    character->_indices[0] = 0;
    character->_indices[1] = 1;
    character->_indices[2] = 2;
    character->_indices[3] = 2;
    character->_indices[4] = 3;
    character->_indices[5] = 0;

    int rgba[4];
    if(type == ALLY) {
        rgba[0] = 50;
        rgba[1] = 175;
        rgba[2] = 50;
        rgba[3] = 255;
    } else {
        rgba[0] = 175;
        rgba[1] = 50;
        rgba[2] = 50;
        rgba[3] = 255;
    }

    //     

    int rsx0 = character->x - 25,   rsy0 = character->y;
    int rsx1 = character->x,        rsy1 = character->y - 50;
    int rsx2 = character->x + 25,   rsy2 = character->y;
    int rsx3 = character->x     ,   rsy3 = character->y - 25;

    if(mx > 0 && my > 0) {
        int delta_y = rsy3 - my; 
        int delta_x = rsx3 - mx;
        double angle_rad = atan2(delta_y, delta_x) - 1.5;
    
        double angleCos = cos(angle_rad);
        double angleSin = sin(angle_rad);

        float ox = (rsx0 + rsx1 + rsx2) / 3;
        float oy = (rsy0 + rsy1 + rsy2) / 3;

        rsx0 -= ox;
        rsx1 -= ox;
        rsx2 -= ox;

        rsy0 -= oy;
        rsy1 -= oy;
        rsy2 -= oy;
        
        int translated_y = rsy0;
        rsy0 = (angleSin * rsx0 + angleCos * translated_y) + oy;
        rsx0 = (angleCos * rsx0 - angleSin * translated_y) + ox;

        translated_y = rsy1;
        rsy1 = (angleSin * rsx1 + angleCos * translated_y) + oy;
        rsx1 = (angleCos * rsx1 - angleSin * translated_y) + ox;

        translated_y = rsy2;
        rsy2 = (angleSin * rsx2 + angleCos * translated_y) + oy;
        rsx2 = (angleCos * rsx2 - angleSin * translated_y) + ox;
    }

    // 
    set_character_item_polygon_coordiantes(character, &character->outer_shape[0], rsx0, rsy0,  rgba[0],rgba[1],rgba[2],255);
    set_character_item_polygon_coordiantes(character, &character->outer_shape[1], rsx1, rsy1,  rgba[0],rgba[1],rgba[2],255);
    set_character_item_polygon_coordiantes(character, &character->outer_shape[2], rsx2, rsy2,  rgba[0],rgba[1],rgba[2],255);
    set_character_item_polygon_coordiantes(character, &character->outer_shape[3], rsx3, rsy3,  rgba[0],rgba[1],rgba[2],255);

}

void character_free_all(character_template_s* item){
}

void render_character_item(character_template_s* character){

    SDL_RenderGeometry( 
        character->renderer, 
        NULL, 
        character->outer_shape, 
        (int)COMPUTE_ARRAY_SIZE(character->outer_shape), 
        character->_indices, 
        (int)COMPUTE_ARRAY_SIZE(character->_indices)
    );
}

void set_character_renderer(character_template_s* character, SDL_Renderer* renderer){
    character->renderer = renderer;
}



#endif