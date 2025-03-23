
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include "../classes/menu.c"
#include "../classes/page.c"
#include "../classes/character.c"
#include "../meta/preprocessors.c"

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
        item->function();
    }
}

void handle_menu_right_click(menu_item_s* item)  {

}


void sdl_stop_event(){
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

void handle_game_key_down(SDL_KeyboardEvent e, character_template_s* target)
{

    switch (e.keysym.scancode)
    {
        case SDL_SCANCODE_W:
        case SDL_SCANCODE_Z:
            target->y-=5;
            break;
        case SDL_SCANCODE_S:
            target->y+=5;
            break;
        case SDL_SCANCODE_A:
        case SDL_SCANCODE_Q:
            target->x-=5;
            break;
        case SDL_SCANCODE_D:
            target->x+=5;
            break;
        case SDL_SCANCODE_ESCAPE:
            current = MENU;
            break;
    }
}

void start_game(){
    current = GAME;
}

