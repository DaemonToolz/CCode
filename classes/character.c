#ifndef character_type
#define character_type
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include "../meta/preprocessors.c"
#include "item.c"
#include <stdio.h>
#include <time.h>

typedef enum {
    NEUTRAL = 0,
    ALLY,
    ENEMY
} character_enum;

typedef struct {
    SDL_Point point;
    time_t    triggered_at;
} location_history;

typedef struct {
    // variables
    char   name[50];
    item_s items[6];

    // display
    int x;
    int y;

    // Technical
    bool can_move;

    int width;
    int height;

    // Graphics
    SDL_Renderer *renderer;
    SDL_Vertex   outer_shape[4];
    SDL_Point    outer_shape_points[4];
    int _indices[6];

    int total_history;
    location_history history[1];
} character_template_s;

void compute_velocity_vector(character_template_s* c, double* vector){
    if(c->total_history == 0){
        return;
    }
    double dx = c->x - c->history->point.x;
    double dy = c->y - c->history->point.y;
    double d = sqrt(dx*dx + dy*dy);
    *(vector) = dx/d * 5, 
    *(vector + 1) = dy/d * 5;
}

void set_rotation_for_point(double angle_rad, int* x, int* y, float origin_point_x, float origin_point_y){
    double angleCos = cos(angle_rad);
    double angleSin = sin(angle_rad);

    *x -= origin_point_x;
    *y -= origin_point_y;
    
    int translated_y = *y;
    *y = (angleSin * (*x) + angleCos * translated_y) + origin_point_y;
    *x = (angleCos * (*x) - angleSin * translated_y) + origin_point_x;
}

void set_character_item_polygon_coordinates(SDL_Vertex* vertex, int x, int y, int r, int g, int b, int a){
    // Position
    vertex->position.x = x;
    vertex->position.y = y;
    // Colors
    vertex->color.r = r;
    vertex->color.g = g;
    vertex->color.b = b;
    vertex->color.a = a;
    // Text
    vertex->tex_coord.x = 1;
    vertex->tex_coord.y = 1;
}


void set_character_render_rect_shape(character_template_s* c, character_enum type, int mx, int my){
    // {SDL_FPoint, SDL_Color, SDL_FPoint}
    c->_indices[0] = 0;
    c->_indices[1] = 1;
    c->_indices[2] = 2;
    c->_indices[3] = 2;
    c->_indices[4] = 3;
    c->_indices[5] = 0;

    int rgba[4];
    switch(type) {
        case ALLY:
            rgba[0] = 50;
            rgba[1] = 175;
            rgba[2] = 50;
            rgba[3] = 255;
            break;
        case ENEMY: 
            rgba[0] = 175;
            rgba[1] = 50;
            rgba[2] = 50;
            rgba[3] = 255;
            break;
        case NEUTRAL: 
            rgba[0] = 50;
            rgba[1] = 50;
            rgba[2] = 50;
            rgba[3] = 255;
            break;
    }

    //     
    SDL_Point point[4] = {
        {c->x - (c->width/2), c->y - (c->height/2)},
        {c->x + (c->width/2), c->y - (c->height/2)},
        {c->x + (c->width/2), c->y + (c->height/2)},
        {c->x - (c->width/2), c->y + (c->height/2)},
    };

    if(c->can_move && mx > 0 && my > 0) {
        int delta_y = point[3].y - my; 
        int delta_x = point[3].x - mx;
        double angle_rad = atan2(delta_y, delta_x) - 1.5;
        SDL_Point center = {
            (point[0].x + point[1].x + point[2].x + point[3].x) / 4,
            (point[0].y + point[1].y + point[2].y + point[3].y) / 4
        };
        for(int i = 0; i < COMPUTE_ARRAY_SIZE(point); ++i){
            set_rotation_for_point(angle_rad, &(point[i].x), &(point[i].y), center.x, center.y);
        }      
    }

    for(int i = 0; i < COMPUTE_ARRAY_SIZE(point); ++i){
        c->outer_shape_points[i] = point[i];
    
        set_character_item_polygon_coordinates(
            &c->outer_shape[i], 
            point[i].x, 
            point[i].y,  
            rgba[0],
            rgba[1],
            rgba[2],
            255);
    }     

}

void set_character_render_shape(character_template_s* c, character_enum type, int mx, int my){
    // {SDL_FPoint, SDL_Color, SDL_FPoint}
    c->_indices[0] = 0;
    c->_indices[1] = 1;
    c->_indices[2] = 2;
    c->_indices[3] = 2;
    c->_indices[4] = 3;
    c->_indices[5] = 0;

    int rgba[4];
    switch(type) {
        case ALLY:
            rgba[0] = 50;
            rgba[1] = 175;
            rgba[2] = 50;
            rgba[3] = 255;
            break;
        case ENEMY: 
            rgba[0] = 175;
            rgba[1] = 50;
            rgba[2] = 50;
            rgba[3] = 255;
            break;
        case NEUTRAL: 
            rgba[0] = 50;
            rgba[1] = 50;
            rgba[2] = 50;
            rgba[3] = 255;
            break;
    }

    //     
    SDL_Point point[4] = {
        {c->x - 25, c->y},
        {c->x, c->y - 50},
        {c->x + 25, c->y},
        {c->x, c->y - 25}   
    };

    if(c->can_move && mx > 0 && my > 0) {
        int delta_y = point[3].y - my; 
        int delta_x = point[3].x - mx;
        double angle_rad = atan2(delta_y, delta_x) - 1.5;
        SDL_Point center = {
            (point[0].x + point[1].x + point[2].x + point[3].x) / 4,
            (point[0].y + point[1].y + point[2].y + point[3].y) / 4
        };
        for(int i = 0; i < COMPUTE_ARRAY_SIZE(point); ++i){
            set_rotation_for_point(angle_rad, &(point[i].x), &(point[i].y), center.x, center.y);
        }      
    }

    for(int i = 0; i < COMPUTE_ARRAY_SIZE(point); ++i){
        c->outer_shape_points[i] = point[i];
    
        set_character_item_polygon_coordinates(
            &c->outer_shape[i], 
            point[i].x, 
            point[i].y,  
            rgba[0],
            rgba[1],
            rgba[2],
            255);
    }     
}

void character_free_all(character_template_s* c){
}

void render_character_item(character_template_s* c){
    SDL_RenderGeometry( 
        c->renderer, 
        NULL, 
        c->outer_shape, 
        (int)COMPUTE_ARRAY_SIZE(c->outer_shape), 
        c->_indices, 
        (int)COMPUTE_ARRAY_SIZE(c->_indices)
    );
}

void set_character_renderer(character_template_s* c, SDL_Renderer* renderer){
    c->renderer = renderer;
    c->total_history = 0;
}

void move_to_location(character_template_s* c, SDL_Point point){
    if(c->total_history == COMPUTE_ARRAY_SIZE(c->history)){
        for(int i = 1; i < COMPUTE_ARRAY_SIZE(c->history); i++){
            c->history[i-1] = c->history[i];
        }
    }
    
    c->history[c->total_history].point.x = c->x;
    c->history[c->total_history].point.y = c->y;
    c->history[c->total_history].triggered_at = time(0);
    c->x += point.x;
    c->y += point.y;

    printf("New Position (%i %i) -> (%i %i) \n", c->x, c->y, c->x + point.x, c->y + point.y);

    if(c->total_history < COMPUTE_ARRAY_SIZE(c->history)){
        c->total_history++;
    }
}

// Function to check if a point lies on a given line segment
bool on_segment(SDL_Point* p, SDL_Point* q, SDL_Point* r) {
    if (q->x <= max(p->x, r->x) && q->x >= min(p->x, r->x)
        && q->y <= max(p->y, r->y) && q->y >= min(p->y, r->y))
        return true;
    return false;
}

// Function to find the orientation of the ordered triplet
// (p, q, r) 0 -> p, q and r are collinear 1 -> Clockwise 2
// -> Counterclockwise
int orientation(SDL_Point* p, SDL_Point* q, SDL_Point* r) {
    double val = (q->y - p->y) * (r->x - q->x)
                 - (q->x - p->x) * (r->y - q->y);
    if (val == 0)
      
        // Collinear
        return 0;
  
    // Clock or counterclock wise
    return (val > 0) ? 1 : 2;
}

// Function to check if two lines (p1-q1 and p2-q2) intersect
bool line_to_line_intersect(SDL_Point* p1, SDL_Point* q1, SDL_Point* p2, SDL_Point* q2, double* x, double* y) {
  
    // Find the four orientations needed for general and
    // special cases
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    // General case: lines intersect if they have different
    // orientations
    if (o1 != o2 && o3 != o4) {
      
        // Compute intersection point
        double a1 = q1->y - p1->y;
        double b1 = p1->x - q1->x;
        double c1 = a1 * p1->x + b1 * p1->y;

        double a2 = q2->y - p2->y;
        double b2 = p2->x - q2->x;
        double c2 = a2 * p2->x + b2 * p2->y;

        double determinant = a1 * b2 - a2 * b1;

        if (determinant != 0) {
            *x = (c1 * b2 - c2 * b1) / determinant;
            *y = (a1 * c2 - a2 * c1) / determinant;
            return true;
        }
    }

    // Special Cases: check if the lines are collinear and
    // overlap
    if (o1 == 0 && on_segment(p1, p2, q1))
        return true;
    if (o2 == 0 && on_segment(p1, q2, q1))
        return true;
  	if (o3 == 0 && on_segment(p2, p1, q2)) 
        return true;
    if (o4 == 0 && on_segment(p2, q1, q2)) 
        return true;
  
    // Lines do not intersect in any case
    return false;
}

float random_number(float min, float max) {
    float difference = (max - min);
    float result = (((float)(difference + 1)/RAND_MAX) * rand() + min);
    if((result / 2) > (max - min)){
        return -result;
    } else {
        return result;
    }
}

bool character_collides(character_template_s *main, character_template_s *to_compare)
{
    bool collision = true;
    double cX = 0.0, cY = 0.0;
    for(int i = 1; i < COMPUTE_ARRAY_SIZE(main->_indices); i++){

        for(int j = 1; j < COMPUTE_ARRAY_SIZE(to_compare->_indices); j++){
            collision = line_to_line_intersect(
                &(main->outer_shape_points[main->_indices[i-1]]),
                &(main->outer_shape_points[main->_indices[i]]),
                &(to_compare->outer_shape_points[to_compare->_indices[j-1]]),
                &(to_compare->outer_shape_points[to_compare->_indices[j]]),
                &cX,
                &cY
            );

            if(collision){
                double* v1;
                double* v2;
                int newSize = sizeof(*v1);
                v1 = calloc(2,  newSize);
                v2 = calloc(2,  newSize);

                compute_velocity_vector(main, v1);
                compute_velocity_vector(to_compare, v2);
                double output_vector[2] = {round(*(v1) - *(v2)), round(*(v1 + 1) - *(v2 + 1)) };
                if(output_vector[0] == 0){
                    output_vector[0] = 1;
                }

                if(output_vector[1] == 0){
                    output_vector[1] = 1;
                }

                printf("%f x / %f y \n", output_vector[0], output_vector[1] );
                printf("Main translation (%i -> %i)x / (%i -> %i) y \n", 
                    main->x, 
                    main->x + (int)output_vector[0], 
                    main->y,
                    main->y + (int)output_vector[1] 
                );
    
                printf("Target translation (%i -> %i)x / (%i -> %i) y \n", 
                    to_compare->x, 
                    to_compare->x - (int)output_vector[0], 
                    to_compare->y,
                    to_compare->y - (int)output_vector[1] 
                );

                // If v1 is faster than v2
                if(output_vector[0] > 0){
                    if(main->x > to_compare->x){
                        if(main->can_move){
                            main->x += output_vector[0];
                        }
                        if(to_compare->can_move){
                            to_compare->x -= output_vector[0];
                        }
                    } else {
                        if(main->can_move){
                            main->x -= output_vector[0];
                        }
                        if(to_compare->can_move){
                            to_compare->x += output_vector[0];
                        } 
                    }
                } else {         
                    if(main->x > to_compare->x){
                        if(main->can_move){
                            main->x -= output_vector[0];
                        }

                        if(to_compare->can_move){
                            to_compare->x += output_vector[0];
                        }
                    } else {
                        if(main->can_move){
                            main->x += output_vector[0];
                        }

                        if(to_compare->can_move){
                            to_compare->x -= output_vector[0];
                        }
                    }
                }

                if(output_vector[1] > 0){
                    if(main->y < to_compare->y){
                        if(main->can_move){
                            main->y -= output_vector[1];  
                        }
                        if(to_compare->can_move){
                            to_compare->y += output_vector[1];
                        }
                    } else {
                        if(main->can_move){
                            main->y += output_vector[1];  
                        }
                        if(to_compare->can_move){
                            to_compare->y -= output_vector[1];
                        }
                    }
                } else {
                    if(main->y < to_compare->y){
                        if(main->can_move){
                            main->y += output_vector[1]; 
                        } 
                        if(to_compare->can_move){
                            to_compare->y -= output_vector[1];
                        }
                    } else {
                        if(main->can_move){
                            main->y -= output_vector[1];  
                        }
                        if(to_compare->can_move){
                            to_compare->y += output_vector[1];
                        }
                    }
                }

                free(v1);
                free(v2);
            }
        }

    }
    return collision;
}

#endif