#ifndef GLOBAL_VARIABLES
#define GLOBAL_VARIABLES
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <unistd.h>
#include <windows.h>
#include "../classes/menu.c"
#include "../classes/character.c"
#include "../classes/page.c"
#include "../classes/save.c"
#include "../functions/runtime_funcs.c"


const int WIDTH = 800, HEIGHT = 600;
int mx = -1, my = -1;
SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *text_texture;
SDL_Rect text_rect;

character_template_s character = { "AEST_01", {}, 200, 200, true, 0, 0, {25, 25}, {0, 0} };


character_template_s enemies[3] = {
    { "EEST_01", {}, 400, 400, true, 0, 0, {5, 5}, {0, 0}  },
    { "EEST_02", {}, 100, 250, true, 0, 0, {5, 5}, {0, 0}  },
    { "EEST_03", {}, 500, 150, true, 0, 0, {5, 5}, {0, 0}  },
};

character_template_s walls[4] = {
    { "WALL_01", {}, 400, 5,    false, 800, 10, {0, 0}, {0, 0}  },
    { "WALL_02", {}, 5,   300,  false, 10, 800, {0, 0}, {0, 0}  },
    { "WALL_03", {}, 400, 595,  false, 800, 10, {0, 0}, {0, 0}  },
    { "WALL_04", {}, 795, 595,  false, 10, 800, {0, 0}, {0, 0}  }
};


menu_item_s menus[4] = {
    {"Play", 0, &start_game,        0, HEIGHT - (256), 78, 64, 150, 150, 150, 100},
    {"Saves", 0, &show_saves,       0, HEIGHT - (192), 128, 64, 150, 150, 150, 255},
    {"Settings", 0, NULL,           0, HEIGHT - (128), 178, 64, 150, 150, 150, 255},
    {"Exit", 0, &sdl_stop_event,    0, HEIGHT - (64), 228, 64, 255, 75, 75, 255}
};


#endif