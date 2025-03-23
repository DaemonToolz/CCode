#ifndef item_type
#define item_type
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../meta/preprocessors.c"

typedef struct {
    // variables
    char name[50];
    void (*action)();
} item_s;


#endif