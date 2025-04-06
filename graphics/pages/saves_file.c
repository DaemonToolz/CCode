

#ifndef SAVE_SECTION
#define SAVE_SECTION

#include "../../database/sqlite_driver.c"
#include "../../classes/save.c"
#include "../../meta/globals.c"
#include "../../meta/preprocessors.c"

//#include "./game_file.c"
save_item_s* all_saves;

int _total_loaded = 0;
int _total_to_load = 0;
int _current_row = 0;
int _current_col = 0;
int _per_col = 4;

void free_all_saves(){
    if(all_saves != NULL){
        free(all_saves);
        printf("Cleared\n");
    }
}

int load_count_db(void *NotUsed, int argc, char **argv, char **azColName)
{
    free_all_saves();
    for (int i = 0; i < argc; i++) {
        _total_to_load = atoi(argv[i]);
        all_saves = (save_item_s*)malloc(sizeof(save_item_s) * atoi(argv[i]));
        printf("%i to load, final size %li\n", _total_to_load, COMPUTE_ARRAY_SIZE(all_saves));
    }
    if(all_saves == NULL){
        printf("Not loaded\n");
        return 1;
    }
    printf("Loaded\n");
    return 0;
}

void init_save_visual(SDL_Renderer* renderer){
    printf("%i total saves to render\n", _total_loaded);
    for(int i = 0; i < _total_loaded; i++){
        set_save_renderer(&all_saves[i], renderer);
        set_save_render_shape(&all_saves[i]);
        set_save_text_font(&all_saves[i]);
        set_save_text_surface(&all_saves[i], 255, 255, 255, 255);
        set_save_text_texture(&all_saves[i]);
    }
}

void render_all_saves(){
    for(int i = 0; i < _total_loaded; i++){
        render_save_item(&all_saves[i]);
    }
}

void load_save_cb(void* caller){
    save_item_s* item = (save_item_s*)caller;
    printf("%i %i", item->pX, item->pY);
    //character.x = item->pX;
    //character.y = item->pY;
    //load_character_information(item.pX, item.pY);
}

int load_all_saves_db(void *NotUsed, int argc, char **argv, char **azColName){
    printf("New data (%s, %s)\n", argv[0], argv[1]);
    save_item_s item = {
        "SAVE", _total_loaded, 
        &load_save_cb, 
        ((_current_col + 1) * 156), 
        ((_current_row + 1) * 156), 
        96, 
        96, 
        atoi(argv[0]),
        atoi(argv[1]),
        150, 150, 150, 100
    };
    _current_col = (_current_col + 1)%_per_col;
    printf("Save will be (%i, %i) at (%i, %i) \n", _current_col, _current_row, item.x, item.y);
    if(_current_col == 0){
        _current_row++;
    }
    printf("%i total out of %i saves loaded and is not null (%i) \n", _total_loaded + 1, _total_to_load, (all_saves == NULL ? 0:1));
    *(all_saves + _total_loaded) = item;

    _total_loaded++;
    return 0;
}

void init_saves(){
    _total_loaded = 0;
    _current_col = 0;
    _current_row = 0;
}
#endif