#pragma once
#include "./src/man/entity.c"
//para las colisiones
#include "./src/man/graphics.c"
#include "./src/sys/anim.c"
//Nevcesitamos llamar al man_game_crear_disparo()
#include "./src/man/game.h"

//Declarations
void sys_physics_update(TEntity *entity);
void sys_physics_check_keyboard(TEntity *entity);
void entity_jump(TEntity *entity);


//Members
//char posicion_inicial_jump;
char contador_jump;
char array_jump[5];
//Definitions
void sys_physics_update(TEntity *entity){
    if (entity->type==entity_type_player){
        //entity->old_x=entity->x;
        //entity->old_y=entity->y;
        sys_physics_check_keyboard(entity);
        //Sistema de salto 2 parte
        if(entity->jump==1){
            entity->y+=entity->vy;
            if(entity->old_y-22>=entity->y){
                entity->vy*=-1;
                entity->jump=0;
            } 
        }
        //Colision con el margen izquierdo
        if(entity->x<=8) entity->x=8;
        
   
        //Gravedad
        if (get_tile_down_array(entity)<man_graphics_get_tile_suelo() && entity->jump==0){
            //Beep();
            entity->y+=entity->vy;
        } 


        //Si te caes por un agujero te matas y vuelves
        if(entity->y>180){
            man_han_matado_al_player();
        }     
    }else if(entity->type==entity_type_shot){
        if (entity->dir==3)entity->x+=entity->vx;
        else if (entity->dir==7)entity->x-=entity->vx;
    }
}



void sys_physics_check_keyboard(TEntity *entity){
    //movement
    // 0=inactive  1=up 2=up & right 3=right 4=down & right 5=down 6=down & left 7=left 8=up & left 
    char joy = JoystickRead(0);
    if(joy==1){
        //entity->vy=-8;
        entity_jump(entity);
    }
    if(joy==2){
        entity->dir=3;
        entity_jump(entity); 
        entity->x+=entity->vx;
    }
    if(joy==3){
        entity->dir=3;
        if (man_graphics_get_tile_right_array(entity)<man_graphics_get_tile_suelo()) 
            entity->x+=entity->vx;
        sys_anim_update(entity);
    }
    if(joy==4){
    }
    if(joy==5){
        //if(get_game_mode()==1)entity->y+=entity->vy;
    }  
    if(joy==6){
    }  
    if(joy==7) {
        entity->dir=7;
        sys_anim_update(entity);
        if (man_graphics_get_tile_left_array(entity)<man_graphics_get_tile_suelo()) entity->x-=entity->vx; 
    }
    if(joy==8){
        entity->dir=7;
        entity_jump(entity);
        entity->x-=entity->vx; 
    }

    //Leemos el disparo
    char trigger = TriggerRead(0);
    if (trigger!=0) {
        man_game_crear_disparo_player(); 
        man_game_reproducir_efecto_sonido(1);   
    }



}

//Sistema de salto 1 parte
void entity_jump(TEntity *entity){
    //Solo se puede saltar si no se está y aen un salto y si se está pegado a un sólido
    if (entity->jump==0 && get_tile_down_array(entity)>=man_graphics_get_tile_suelo()){
        //Activamos el salto
        entity->jump=1;  
        //invertimos la velocidad en y para que sume negativos y lo suba
        entity->vy*=-1;
        entity->old_y=entity->y;
    }
}




