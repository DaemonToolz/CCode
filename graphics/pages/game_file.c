#ifndef GAME_SECTION
#define GAME_SECTION
#include "../../meta/preprocessors.c"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../../database/sqlite_driver.c"
#include "../../classes/menu.c"
#include "../../functions/runtime_funcs.c"
#include "../../classes/character.c"
#include "../../meta/globals.c"
#include "../renderer.c"

int load_character_information_cb(void *NotUsed, int argc, char **argv, char **azColName)
{
    for (int i = 0; i < argc; i++)
    {
        if(strcmp( azColName[i], "X") == 0){
            character.x = atoi(argv[i]);
        } 

        if(strcmp( azColName[i], "Y") == 0){
            character.y = atoi(argv[i]);
        } 
        
    }
    return 0;
}

void init_character(){
    sqlite_select(&load_character_information_cb);
    set_character_renderer(&character, renderer);
    set_character_render_shape(&character, ALLY, mx, my);
}

void init_enemies(){
    for(int i = 0; i < COMPUTE_ARRAY_SIZE(enemies); ++i){
        set_character_renderer(&enemies[i], renderer);
        set_character_render_shape(&enemies[i], ENEMY, character.x, character.y);
    }
}

void init_walls(){
    for(int i = 0; i < COMPUTE_ARRAY_SIZE(walls); ++i){
        set_character_renderer(&walls[i], renderer);
        set_character_render_rect_shape(&walls[i], NEUTRAL, 0, 0);
    }
}

void render_game_screen(){
    set_character_render_shape(&character, ALLY, mx, my);
    render_character_item(&character);

    for(int i = 0; i < COMPUTE_ARRAY_SIZE(enemies); ++i){
        set_character_render_shape(&enemies[i], ENEMY, character.x, character.y);
        render_character_item(&enemies[i]);
        character_collides(&character, &enemies[i]);
    }

    for(int j = 0; j < COMPUTE_ARRAY_SIZE(walls); ++j){
        set_character_render_rect_shape(&walls[j], NEUTRAL, 0, 0);
        render_character_item(&walls[j]);
        character_collides(&character, &walls[j]);
        for(int i = 0; i < COMPUTE_ARRAY_SIZE(enemies); ++i){
            character_collides(&enemies[i], &walls[j]);
        }
    }
}

#endif // GAME_SECTION