// gcc main.c -o main -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_keyboard.h>
#include <unistd.h>
#include <windows.h>

#include "./meta/globals.c"
#include "./meta/preprocessors.c"
#include "./graphics/renderer.c"
#include "./graphics/pages/game_file.c"
#include "./classes/character.c"
#include "./classes/page.c"
#include "./database/sqlite_driver.c"
#include "./graphics/pages/pause_menu.c"
#include "./graphics/pages/saves_file.c"
#include "./classes/menu.c"
#include "./functions/runtime_funcs.c"

// https://stackoverflow.com/questions/60421225/cleanup-function-in-c
// https://stackoverflow.com/questions/368385/implementing-raii-in-pure-c


void switch_mouse_motion(SDL_Event event){
    switch(current){
        case MENU:
        case SETTINGS:
        case SAVES:
            handle_menu_mouse_motion(menus, (int)COMPUTE_ARRAY_SIZE(menus), event.motion);
            break;
        case GAME:
            mx = event.motion.x;
            my = event.motion.y;
            break;
    }
}

void switch_key_down(SDL_Event event, const Uint8* states){
    switch(current){
        case MENU:
            sqlite_count(&load_count_db);
            init_saves();
            init_save_visual(renderer);
            break;
        case SETTINGS:
        case SAVES:
            break;
        case GAME:
            check_all_collisions();
            handle_game_key_down(event.key, &character, states);
            break;
    }
}

void switch_handle_mouse_button(SDL_Event event){
    switch(current){
        case MENU:
        case SETTINGS:
        case SAVES:
            handle_menu_mouse_button(menus, (int)COMPUTE_ARRAY_SIZE(menus), event.button);
            break;
        case GAME:
            break;
    }
}

void render_page_content(){
    switch(current){
        case MENU:
            render_menu_screen();
            break;
        case GAME:
            render_game_screen();
            break;
        case SAVES:
            render_all_saves();
            break;
    }

}

int main(int argc, char *argv[]) {
    srand(time(NULL));    
    init_saves();
    sqlite_create_database();
    sqlite_count(&load_count_db);
    sleep(1);
    sqlite_select(&load_all_saves_db);
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }
    
    window = SDL_CreateWindow("SDL Tester", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              WIDTH, HEIGHT,
                              /*SDL_WINDOW_RESIZABLE | */
                              SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    TTF_Init();
    renderer = SDL_CreateRenderer(window, -1,  SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    init_menu();
    init_character();
    init_enemies();
    init_walls();

    init_save_visual(renderer);
    
    const Uint8* keyboard_state_array = SDL_GetKeyboardState(NULL);
    SDL_Event event;
    bool canStop = false;
    bool newAction = false;
    while (!canStop)
    { 
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);        
        SDL_RenderClear(renderer);

        while (SDL_PollEvent(&event) != 0)
        { 
            
            switch(event.type){
                case SDL_QUIT:
                    canStop = true;
                    break;
                case SDL_MOUSEMOTION:
                    switch_mouse_motion(event);
                    break;
                case SDL_MOUSEBUTTONUP:
                case SDL_MOUSEBUTTONDOWN:
                    switch_handle_mouse_button(event);
                    break;
                case SDL_KEYDOWN:
                    switch_key_down(event, keyboard_state_array);
                    break;
                default:
                    //printf("Event ignored\n");
                    break;
            }
        }

        switch(current){
            case GAME:
                recalculate_speed(&character);
                break;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        render_page_content();
        SDL_RenderPresent(renderer);

    }

    //sqlite_insert(character.x, character.y);

    for(int i = 0; i < COMPUTE_ARRAY_SIZE(menus); ++i){
        menu_free(&menus[i]);
    }
    free_all_saves();
    SDL_DestroyTexture(text_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit(); 
    return 0;
}