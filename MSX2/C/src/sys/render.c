#pragma once
#include "fusion-c/header/msx_fusion.h"
#include "fusion-c/header/vdp_graph2.h"
#include "src/man/entity.c"
#include "src/man/sprites.c"
//Para el man_get_wrold
#include "src/man/game.c"
#include "src/sys/anim.c"
//Declarations
void sys_render_init();
void sys_render_update(TEntity *entity);
void sys_render_update_player(TEntity *player);
void sys_render_update_enemy(TEntity *enemy);
void sys_render_update_shot(TEntity *fire);
void sys_render_update_object(TEntity *object);
void sys_render_update_boss(TEntity *entity);
//Definitions
void sys_render_init(){
  SetColors(15,1,1);
  SetBorderColor(0x01);
  Screen(5);
}

void sys_render_update(TEntity *entity){
    if (entity->type==entity_type_player)sys_render_update_player(entity);
    if (entity->type==entity_type_enemy1)sys_render_update_enemy(entity);
    if (entity->type==entity_type_shot)sys_render_update_shot(entity);
    if (entity->type==entity_type_object_oxigen)sys_render_update_object(entity);
    //if (entity->type==entity_type_object_batery)sys_render_update_object(entity);
    if (entity->type==entity_type_boss)sys_render_update_boss(entity);
}
void sys_render_update_player(TEntity *player){
    //void PutSprite( char sprite_n, char pattern_n, char x, char y, char color)
    // 1 El plano o su definición en la tabla de atributos, es donde alamcenará la posición x e y, y su sprite
    // 2 la definición en sprite pattern que va de 4 en 4
    // 3 posición eje x
    // 4 posición eje y
    // 5 color  
    //PutSprite( pplano, psprite, px,py, pcolor );
    //Si se mueve a la derecha
    if (player->dir==3){
        if (player->jump==1){
            PutSprite( player->plane, player_jump_right_pattern, player->x,player->y,0 );
        }else{
            if(player->andando ==0 ){
                PutSprite( player->plane, player_right_pattern, player->x,player->y,0 );
            } else {
                PutSprite( player->plane, player_right_walking_pattern, player->x,player->y,  0);
            }
        }
    //Si se mueve a la izquierda
    }else if(player->dir==7){
        if (player->jump==1){
            PutSprite( player->plane, player_Jump_left_pattern, player->x,player->y,0 );
        }else{
            if(player->andando ==0){
                PutSprite( player->plane, player_left_pattern, player->x,player->y, 0 );
            } else {
                PutSprite( player->plane, player_left_walking_pattern, player->x,player->y,  0 );
            }
        } 
    }
}



void sys_render_update_enemy(TEntity *enemy){   
    sys_anim_update(enemy);
    //No queremos qu ecuando se salga de la pantalla pinte a los enemigos
    if (enemy->dir==3 && enemy->x<256 && enemy->x>0){
        if (enemy->andando){
            PutSprite(enemy->plane,enemy1_right_pattern,enemy->x,enemy->y,0);
        }else{
            PutSprite(enemy->plane,enemy1_right_walking_pattern,enemy->x,enemy->y,0);
        } 
        //todo poner la ara derecha a los enemigos
    }else if (enemy->dir==7 && enemy->x<256 && enemy->x>0){
        if (enemy->andando){
            PutSprite(enemy->plane,enemy1_left_pattern,enemy->x,enemy->y,0);
        }else{
            PutSprite(enemy->plane,enemy1_left_walking_pattern,enemy->x,enemy->y,0);
        } 
    }else{
         PutSprite(enemy->plane,enemy1_left_walking_pattern,0,212,0);
    }
}

void sys_render_update_shot(TEntity *shot){
   if (shot->plane!=0) PutSprite(shot->plane,shot_pattern,shot->x,shot->y,0);
}
 
void sys_render_update_object(TEntity *object){

    if (object->x>0 && object->x<256 ){
        PutSprite(object->plane,object_oxigen_pattern,object->x,object->y,0);
    }
    //if (object->type==entity_type_object_oxigen  && sys_entity_get_num_objects()>0) PutSprite(object->plane,object_oxigen_pattern,object->x,object->y,0);

}

void sys_render_update_boss(TEntity *entity){
    if(man_game_get_world()==0){
        //entity->x=entity->x;
        //HMMM(buffer en RAM, posición_x, posición_Y (256 será la page 1), ancho copia, alto copia)
        //HMMM(0,256+100,entity->old_x,entity->old_y,24,24);
        //void BoxFill (int X1, int Y1, int X2, int yY22, char color, char OP )
        BoxFill (entity->old_x, entity->old_y, entity->old_x+24, entity->old_y+24, 14,0 );
        LMMM(120,256+24,entity->x,entity->y,24,24,3); 
        
    }
}

