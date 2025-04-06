#ifndef RUNTIME_FUNC
#define RUNTIME_FUNC
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include "../classes/menu.c"
#include "../classes/page.c"
#include "../classes/save.c"
#include "../classes/character.c"
#include "../meta/preprocessors.c"
#include "../database/sqlite_driver.c"
#include "../graphics/pages/saves_file.c"
#include "../meta/globals.c"



page_enum current = MENU;

bool is_within_bounds(menu_item_s* item, int x, int y){
    // Too far left
    if (x < item->x) return false;
    // Too far right
    if (x > item->x + item->length) return false;
    // Too high
    if (y < item->y) return false;
    // Too low
    if (y > item->y + item->height) return false;
    // Within bounds
    return true;
}

void handle_menu_left_click(menu_item_s* item) {
    if(item->function != NULL){
        item->function(item);
    }
}

void handle_menu_right_click(menu_item_s* item)  {

}


void sdl_stop_event(void* caller){
    printf("================ Received stop event\n");
    SDL_Event user_event;
    SDL_zero(user_event);  /* SDL will copy this entire struct! Initialize to keep memory checkers happy. */
    user_event.type = SDL_QUIT;
    user_event.user.code = 2;
    user_event.user.data1 = NULL;
    user_event.user.data2 = NULL;
    SDL_PushEvent(&user_event);
}



void handle_menu_mouse_motion(menu_item_s menus[], int size, SDL_MouseMotionEvent E){

}

void handle_save_mouse_button(menu_item_s save[], int size, SDL_MouseButtonEvent E) {
    for(int i = 0; i < size; i++){
        if (is_within_bounds(&save[i], E.x, E.y)) {
            if (E.button == SDL_BUTTON_LEFT) {
                handle_menu_left_click((&save[i]));
            } else if (E.button == SDL_BUTTON_RIGHT) {
                handle_menu_right_click((&save[i]));
            }
        }
    }
}

void handle_menu_mouse_button(menu_item_s menus[], int size, SDL_MouseButtonEvent E) {
    for(int i = 0; i < size; i++){
        if (is_within_bounds(&menus[i], E.x, E.y)) {
            if (E.button == SDL_BUTTON_LEFT) {
                handle_menu_left_click((&menus[i]));
            } else if (E.button == SDL_BUTTON_RIGHT) {
                handle_menu_right_click((&menus[i]));
            }
        }
    }
}

void handle_game_key_down(SDL_KeyboardEvent e, character_template_s* target, const Uint8* states)
{
    
    SDL_Point point;
    point.x = 0;
    point.y = 0;
    if(states[SDL_SCANCODE_W] || states[SDL_SCANCODE_Z]){
        point.y -= 1;
    }

    if(states[SDL_SCANCODE_S]){
        point.y += 1;
    }

    if(states[SDL_SCANCODE_A] || states[SDL_SCANCODE_Q]){
        point.x -= 1;
    }

    if(states[SDL_SCANCODE_D]){
        point.x += 1;
    }


    switch (e.keysym.scancode)
    {
        case SDL_SCANCODE_ESCAPE:
            sqlite_select(&load_all_saves_db);
            current = MENU;
            break;
    }

    if (current == GAME){
        move_to_location(target, point);
    }
    
}

void start_game(void* caller){
    current = GAME;
}

void show_saves(void* caller){
    current = SAVES;
}

#endif