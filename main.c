// gcc main.c -o main -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf

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
const int WIDTH = 800, HEIGHT = 600;
SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *text_texture;
SDL_Rect text_rect;

menu_item_s menus[2] = {
    {"Home", 0, NULL, 5, 5, 256, 94},
    {"Exit", 0, &sdl_stop_event, 5, 104, 256, 94}
};

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }
    
    //SDL_SetRelativeMouseMode(SDL_TRUE);
    

    window = SDL_CreateWindow(menus[0].name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              WIDTH, HEIGHT,
                              SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    TTF_Init();

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    
    for(int i = 0; i < COMPUTE_ARRAY_SIZE(menus); ++i){
        set_renderer(&menus[i], renderer);
        set_render_rectangle(&menus[i], window);
        set_text_font(&menus[i]);
        set_text_surface(&menus[i], 255, 255, 255, 255);
        set_text_texture(&menus[i]);
    }

    SDL_Event event;
    bool canStop = false;
    bool newAction = false;
    //display_text(renderer, 256, 256, "No action", font, &text_texture, &text_rect);
    while (!canStop)
    { 
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);        
        SDL_RenderClear(renderer);
        sleep(0.1);
        while (SDL_PollEvent(&event) != 0)
        { 
            
            switch(event.type){
                case SDL_QUIT:
                    //display_text(renderer, 256, 256, "Quit event", font, &text_texture, &text_rect);
                    canStop = true;
                    break;
                case SDL_MOUSEMOTION:
                    handle_mouse_motion(menus, (int)COMPUTE_ARRAY_SIZE(menus), event.motion);
                    break;
                case SDL_MOUSEBUTTONUP:
                case SDL_MOUSEBUTTONDOWN:
                    //display_text(renderer, 256, 256, "Click event", font, &text_texture, &text_rect);
                    handle_mouse_button(menus, (int)COMPUTE_ARRAY_SIZE(menus), event.button);
                    break;
                case SDL_KEYDOWN:
                    //display_text(renderer, 256, 256, SDL_GetKeyName(SDL_GetKeyFromScancode(SDL_SCANCODE_W)), font, &text_texture, &text_rect);
                    break;
                default:
                    printf("Event ignored\n");
                    break;
            }
        }

       
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        for(int i = 0; i < COMPUTE_ARRAY_SIZE(menus); ++i){
            render_menu_item(&menus[i]);
        }
        SDL_RenderPresent(renderer);

    }

    for(int i = 0; i < COMPUTE_ARRAY_SIZE(menus); ++i){
        free_all(&menus[i]);
    }

    SDL_DestroyTexture(text_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit(); 
    return 0;
}