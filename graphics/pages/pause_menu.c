#ifndef PAUSE_SECTION
#define PAUSE_SECTION
#include "../../classes/menu.c"
#include "../../meta/globals.c"
#include "../../meta/preprocessors.c"

void init_menu(){
    for(int i = 0; i < COMPUTE_ARRAY_SIZE(menus); ++i){
        set_menu_renderer(&menus[i], renderer);
        set_menu_render_shape(&menus[i]);
        set_menu_text_font(&menus[i]);
        set_menu_text_surface(&menus[i], 255, 255, 255, 255);
        set_menu_text_texture(&menus[i]);
    }
}

void render_menu_screen(){
    for(int i = COMPUTE_ARRAY_SIZE(menus); i >= 0; i--){
        render_menu_item(&menus[i]);
    }
}

#endif