#pragma once
#include "src/man/entity.c"
#include "src/man/game.h"
#include "src/man/sprites.c"
#include "src/man/graphics.c"
#include "src/sys/render.c"
#include "src/sys/physics.c"
#include "src/sys/collider.c"
#include "src/sys/ai.c"





void man_game_init(){
  //Ponemos la pantalla en screen 5
  sys_render_init();
  //Pantalla de carga
  cargarLoaderEnRAM();
  deRamAVramPage0();
  //reservamos 10*21bytes en la RAM para trabajar en ese espacio de memoria
  sys_entities_init();
  sys_ai_init();
  //Tamaño y cosas de MSX para los sprites
  man_sprites_init();
  //Los sprites están definidos en la RAM en unos archivos.c los cargamos en la VRAM
  man_sprites_load();

  inicializarPantalla();
  cargarTileSetEnRAM();
  deRamAVramPage1();

  cargarTileMapEnRAM();
  //Pantalla de mision
  Cls();
  PutText(0,80, "Tienes que recoger las botellas de oxigeno creado en Marte para que tu equipo compruebe si es repirable ",0);
  WaitKey();
  //Pintamos el nivel 0
  pintarPantallaInicio();


  SetRealTimer(0);	
  KeySound(0);
  actual_world=0;
  //Le ponemos que aplique que el mapa ha cambiado para que ponga los objetos y el player
  world_change=1;
  old_contador_columna=0;
 
  player=sys_entity_create_player();  
  //El contador de columna se actualizará en el graphics update
  //contador_columna=man_graphics_get_column_counter();
  numero_columnas=man_graphics_get_num_columns();
  //Según el mundo que cargemos tendremos unos enemigos y unos objetos que coger
  //man_game_update();

  array_enemies=sys_entity_get_array_structs_enemies();
  array_shots=sys_entity_get_array_structs_shots();
  array_objects=sys_entity_get_array_structs_objects();


  //scoreboard();
}

void man_game_play(){

  //Game loop
  while(1){
    //Pantalla
    if (player->x/8>14 && man_graphics_get_column_counter()<man_graphics_get_num_columns()-1){
      man_game_desplazar_entidades_a_la_izquierda();
      recorrerBufferTileMapYPintarPage1EnPage0();
    }
    //Player
    sys_physics_update(player);
    sys_render_update(player);
    //Enemigos
    for (char i=0;i<sys_entity_get_num_enemies();++i){
        TEntity *enemy=&array_enemies[i];
        sys_physics_update(enemy);
        if (sys_collider_entity1_collider_entity2(player, enemy)){
          man_han_matado_al_player();
          Beep();
        }
        //Le aplicamos un comportamiento a los enemigos según el tipo de enemigo que sea
        sys_ai_update(enemy);
        sys_render_update(enemy);
        //Si el enemigo se sale de la pantalla lo matamos
        //if (enemy->x<0) sys_entity_erase_enemy(i);
        //else if (enemy->x>255) sys_entity_erase_enemy(i);
        if (enemy->y>180) sys_entity_erase_enemy(i);
    }
    //Objetos
    for (char i=0;i<sys_entity_get_num_objects();++i){
      TEntity *object=&array_objects[i];
      sys_render_update(object);
      if (sys_collider_entity1_collider_entity2(player, object)){
        sys_entity_erase_object(i);
        Beep();
      }
      //Si el objecto se sale de la pantalla lo matamos
      if (object->x<0) sys_entity_erase_object(i);
      else if (object->y>180) sys_entity_erase_object(i);
    }
    //Disparos
    for (char i=0;i<sys_entity_get_num_shots();++i){
      TEntity *shot=&array_shots[i];
      sys_physics_update(shot);
      sys_render_update(shot);
      //if(sys_collider_enemy_with_shot(shot))Beep;
      for (int w=0; w<sys_entity_get_num_enemies();w++){
        TEntity *enemy=&array_enemies[i];
        if (enemy->x < shot->x + 16 &&  enemy->x + 16 > shot->x && enemy->y < shot->y + 16 && 16 + enemy->y > shot->y){
            sys_entity_erase_enemy(w);
            sys_entity_erase_shot(i);
            Beep();
        }
      }  
      //Si el objecto se sale de la pantalla lo matamos
      if (shot->x<0) sys_entity_erase_shot(i);
      else if (shot->x>255) sys_entity_erase_shot(i);
      else if (shot->y>180) sys_entity_erase_shot(i);
    }
    //Manager del juego
    man_game_update();
    //Puntuación
    //scoreboard();
    //Pausa
    wait();
  }
}







void man_game_update(){
  //Estos objetos se mstrarán cuando se cargue el mundo
  if (world_change==1){

    if(actual_world==0){
      for (int i=0; i<objetos_por_mundo;i++){
        TCoordinate* coordinate=&world_objects[actual_world][i];
        if (coordinate->x<32*8){
          TEntity *object=sys_entity_create_object();
          object->y=coordinate->y;
          object->x=coordinate->x;
          object->type=coordinate->type;
        }
      }
    }else if(actual_world==1){

    }
    world_change=0;
  } 

  //Estos objetos se hirán cargando según la pantalla se vaya moviendo
  //el old_contador_columna es para que no se repita si estas en la misma columna
  if (old_contador_columna!=man_graphics_get_column_counter()){
    //Mostramos los enemigos según la posición de la columna
    for (int i=0;i<enemigos_por_mundo;i++ ){
      if ( man_graphics_get_column_counter()==world_enemies[actual_world][i]) {
        TEntity *enemy=sys_entity_create_enemy1();
        enemy->y=20*8;
        enemy->x=255;
        enemy->dir=7;
      }
    }
    //Mostramos los obejtos según la posición de la columna
    for (int i=0; i<objetos_por_mundo;i++){
      TCoordinate* coordinate=&world_objects[actual_world][i];
      if ( man_graphics_get_column_counter()==coordinate->x/8){
        TEntity *object=sys_entity_create_object();
        object->y=coordinate->y;
        object->x=254;
        object->type=coordinate->type;
      }
    }
    //Creamos el jefe si ha llegado al final de la fase
    //if (man_graphics_get_column_counter()==man_graphics_get_num_columns()-1) {
    if (man_graphics_get_column_counter()==man_graphics_get_num_columns()-1) {
       TEntity *boss=sys_entity_create_enemy1();
       boss->x=200;
       boss->y=14*8;
       boss->type=entity_type_boss;
     }
    old_contador_columna=man_graphics_get_column_counter();
  }
}



void man_game_desplazar_entidades_a_la_izquierda(){
  player->x-=player->vx;
  for (char i=0;i<sys_entity_get_num_enemies();++i){
    TEntity *enemy=&array_enemies[i];
    if (enemy->type==entity_type_enemy1 && enemy->dir==7) {
       enemy->x-=enemy->vx*2;
    }
  }
  for (char i=0;i<sys_entity_get_num_objects();++i){
    TEntity *object=&array_objects[i];
       object->x-=object->vx;
  }
}

void man_game_crear_disparo_player(){
  TEntity *shot=sys_entity_create_shot();
  shot->x=player->x+8;
  shot->y=player->y+6;
  shot->dir=player->dir;
}
void man_game_crear_disparo_boss(TEntity *enemy){
  TEntity *shot=sys_entity_create_shot();
  shot->x=enemy->x-20;
  shot->y=enemy->y;
  shot->dir=7;
  Beep();
}
 void man_han_matado_al_player(){
    player->x=0*8;
    player->y=0*8;
 }
char man_game_get_world(){
    return actual_world;
}



void scoreboard(){
    //void Rect ( int X1, int Y1, int X2, int Y2, int color, int OP )
    //void BoxFill (int X1, int Y1, int X2, int yY22, char color, char OP )

    BoxFill (0, 23*8, 256, 210, 6, LOGICAL_IMP );

    //PutText(10,190, Itoa(player->x/8,"   ",10),0);
    //PutText(50,190, Itoa(player->y/8,"   ",10),0);
    //PutText(100,190, Itoa(man_graphics_get_column_counter(),"   ",10),0);
    //PutText(150,190, Itoa(man_graphics_get_num_columns(),"   ",10),0);



    TEntity *enemy=&array_enemies[0];
    PutText(0,190,Itoa(man_graphics_get_tile_left_array(enemy),"  ",10),8);
    PutText(50,190,Itoa(man_graphics_get_tile_right_array(enemy),"  ",10),8);
    PutText(100,190,Itoa(enemy->x,"  ",10),8);
    PutText(150,190,Itoa(sys_entity_get_num_enemies(),"  ",10),8);



    //TEntity *shot=&array_shots[0];
    //PutText(0,190,Itoa(shot->x,"  ",10),8);
    //PutText(50,190,Itoa(shot->y,"  ",10),8);
    //PutText(100,190,Itoa(sys_entity_get_num_shots(),"  ",10),8);
    //PutText(150,190,Itoa(object->plane,"  ",10),8);




    //TEntity *object=&array_objects[1];
    //PutText(0,200,Itoa(object->x/8,"  ",10),8);
    //PutText(50,200,Itoa(object->y/8,"  ",10),8);
    //PutText(100,200,Itoa(object->type,"  ",10),8);
    //PutText(150,200,Itoa(sys_entity_get_num_objects(),"  ",10),8);
        
    PutText(0,200,Itoa(man_graphics_get_column_counter(),"  ",10),8);



}

void wait(){
    __asm
      halt
      halt
      halt
      halt
      halt
      halt
  __endasm;
}


//char generar_numero_aleatorio (char a, char b){
//    char random; 
//    random = rand()%(b-a)+a;  
//    return(random);
//}

