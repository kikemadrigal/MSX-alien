#pragma once
#include "fusion-c/header/msx_fusion.h"
//rand y srand http://cplusplus.com/reference/cstdlib/rand/
#include <stdlib.h>
#include "src/man/entity.c"
#include "src/sys/physics.c"
//Declarations
void sys_ai_init();
void sys_ai_update(TEntity *entity);
void sys_ai_update_enemy_behavior_1(TEntity *enemy);
void sys_ai_update_enemy_boss1(TEntity *enemy);
int sys_ai_contador;


//Definitions
void sys_ai_init(){
    sys_ai_contador=0;
}


void sys_ai_update(TEntity *entity){
    if (entity->type==entity_type_enemy1)sys_ai_update_enemy_behavior_1(entity);
    if (entity->type==entity_type_boss){
        if(man_game_get_world()==0) sys_ai_update_enemy_boss1(entity);
    }
}

void sys_ai_update_enemy_behavior_1(TEntity *enemy){
    //Cuando haya un choque con algún objeto pared se cambiará su velocidad para que reste y la dirección
    if (man_graphics_get_tile_left_array(enemy)>man_graphics_get_tile_suelo() || man_graphics_get_tile_right_array(enemy)>man_graphics_get_tile_suelo() ){
        if(enemy->dir==7) enemy->dir=3;
        else if(enemy->dir==3) enemy->dir=7;
        //Beep();
    }
    //if (get_tile_down_array(enemy)<man_graphics_get_tile_suelo()) enemy->y+=enemy->vy;
    if (get_tile_down_right_array(enemy)<man_graphics_get_tile_suelo() || get_tile_down_left_array(enemy)<man_graphics_get_tile_suelo()) {
        if(enemy->dir==7) enemy->dir=3;
        else if(enemy->dir==3) enemy->dir=7;
    }
    if(enemy->dir==7){
        enemy->x-=enemy->vx;
    }else if(enemy->dir==3){
        enemy->x+=enemy->vx;
    }
}

void sys_ai_update_enemy_boss1(TEntity *enemy){
    ++sys_ai_contador;
    enemy->old_x=enemy->x;
    enemy->old_y=enemy->y;
    char aleatorio=rand()%(6-3)+3;
    if (sys_ai_contador<=3)enemy->x-=enemy->vx;
    else if (sys_ai_contador>3)enemy->x+=enemy->vx;
    if (sys_ai_contador==6) {
        //enemy->y= rand()%(160-100)+100; 
        enemy->vy=-enemy->vy ; 
        sys_ai_contador=0;
    }
    if (sys_ai_contador==aleatorio){
        man_game_crear_disparo_boss(enemy);
    }

    enemy->y+=enemy->vy ;

}


