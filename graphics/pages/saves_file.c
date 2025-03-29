#ifndef SAVE_SECTION
#define SAVE_SECTION
#include "../../database/sqlite_driver.c"
#include "../../classes/save.c"
#include "../../meta/globals.c"
#include "../../meta/preprocessors.c"
//#include "./game_file.c"

int _total_loaded = 0;

void free_all_saves(){
    if(all_saves != NULL){
        free(all_saves);
    }
}

int load_count_db(void *NotUsed, int argc, char **argv, char **azColName)
{
    free_all_saves();
    for (int i = 0; i < argc; i++)
    {
        all_saves = malloc(atoi(argv[i]));
    }
    return 0;
}

void init_save(){
    for(int i = 0; i < COMPUTE_ARRAY_SIZE(all_saves); ++i){
        set_save_renderer(&all_saves[i], renderer);
        set_save_render_shape(&all_saves[i]);
        set_save_text_font(&all_saves[i]);
        set_save_text_surface(&all_saves[i], 255, 255, 255, 255);
        set_save_text_texture(&all_saves[i]);
    }
}

void render_all_saves(){
    for(int i = COMPUTE_ARRAY_SIZE(all_saves); i >= 0; i--){
        render_save_item(&all_saves[i]);
    }
}

void load_save_cb(void* caller){
    save_item_s item = (save_item_s*)caller;
    //load_character_information(item.pX, item.pY);
}

int load_all_saves_db(void *NotUsed, int argc, char **argv, char **azColName){
    save_item_s item = {
        "SAVE", _total_loaded, 
        &load_save_cb, 
        0, 
        HEIGHT - (256), 
        78, 
        64, 
        atoi(argv[0]),
        atoi(argv[1]),
        150, 150, 150, 100
    };

    all_saves[_total_loaded] = item;

    _total_loaded++;
}

void init_saves(){
    _total_loaded = 0;
}
#endif