// gcc main.c -o main -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <unistd.h>
#include <windows.h>
#include "./classes/menu.c"
#include "./functions/runtime_funcs.c"
#include "./meta/preprocessors.c"
#include "./graphics/renderer.c"
#include "./graphics/pages/game_file.c"
#include "./classes/character.c"
#include "./classes/page.c"
#include "./database/sqlite_driver.c"
#include "./meta/globals.c"


void init_menu(){
    for(int i = 0; i < COMPUTE_ARRAY_SIZE(menus); ++i){
        set_menu_renderer(&menus[i], renderer);
        set_menu_render_shape(&menus[i]);
        set_menu_text_font(&menus[i]);
        set_menu_text_surface(&menus[i], 255, 255, 255, 255);
        set_menu_text_texture(&menus[i]);
    }
}

void switch_mouse_motion(SDL_Event event){
    switch(current){
        case MENU:
        case SETTINGS:
            handle_menu_mouse_motion(menus, (int)COMPUTE_ARRAY_SIZE(menus), event.motion);
            break;
        case GAME:
            mx = event.motion.x;
            my = event.motion.y;
            break;
    }
}

void switch_key_down(SDL_Event event){
    switch(current){
        case MENU:
        case SETTINGS:
            
            break;
        case GAME:
            for(int i = 0; i < COMPUTE_ARRAY_SIZE(enemies); ++i){
                character_collides(&character, &enemies[i]);
            }

            for(int j = 0; j < COMPUTE_ARRAY_SIZE(walls); ++j){
                character_collides(&character, &walls[j]);
                for(int i = 0; i < COMPUTE_ARRAY_SIZE(enemies); ++i){
                    character_collides(&enemies[i], &walls[j]);
                }
            }

            handle_game_key_down(event.key, &character);
            break;
    }
}

void switch_handle_mouse_button(SDL_Event event){
    switch(current){
        case MENU:
        case SETTINGS:
            handle_menu_mouse_button(menus, (int)COMPUTE_ARRAY_SIZE(menus), event.button);
            break;
        case GAME:
            break;
    }
}

void render_page_content(){
    switch(current){
        case MENU:
            for(int i = COMPUTE_ARRAY_SIZE(menus); i >= 0; i--){
                render_menu_item(&menus[i]);
            }
            break;
        case GAME:
            render_game_screen();
            break;
    }

}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    sqlite_create_database();
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
                    switch_key_down(event);
                    break;
                default:
                    //printf("Event ignored\n");
                    break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        render_page_content();
        SDL_RenderPresent(renderer);

    }

    sqlite_insert(character.x, character.y);
    for(int i = 0; i < COMPUTE_ARRAY_SIZE(menus); ++i){
        menu_free_all(&menus[i]);
    }

    SDL_DestroyTexture(text_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit(); 
    return 0;
}