#pragma once
#include "fusion-c/header/msx_fusion.h"
#include "src/man/entity.c"
#include "src/man/game.h"
#include "src/man/sprites.c"
#include "src/man/graphics.c"
#include "src/sys/render.c"
#include "src/sys/physics.c"
#include "src/sys/collider.c"
#include "src/sys/ai.c"
#include "fusion-c/header/pt3replayer.h"
#include "fusion-c/header/ayfx_player.h"



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

  //Cargamos los efectos de sonido   
  InitFX();
  cargarBufferDeEfectosDeSonido();
  //Cargamos la música
  InitPSG();
  cargarBufferDeMusica();
  PT3Init (songBuffer+99, 0);

  
  //Pantalla de mision
  Cls();
  PutText(100,20, "Captain Kik",0);
  PutText(100,70, "Mision 1:",0);
  PutText(0,80, "Tienes que recoger las botellas de oxigeno creado en Marte para que tu equipo compruebe si es repirable ",0);
  WaitKey();
  //Pintamos el nivel 0
  //pintarPantallaInicio();


  SetRealTimer(0);	
  KeySound(0);
  actual_world=0;
  //Le ponemos que aplique que el mapa ha cambiado para que ponga los objetos y el player
  world_change=1;
  old_contador_columna=0;
 
  player=sys_entity_create_player();  

  numero_columnas=man_graphics_get_num_columns();

  array_enemies=sys_entity_get_array_structs_enemies();
  array_shots=sys_entity_get_array_structs_shots();
  array_objects=sys_entity_get_array_structs_objects();


  scoreboard();
}

void man_game_play(){

  //Game loop
  while(Inkey()!=27){
    //Musica
    DisableInterupt();
    PT3Rout();
    PT3Play();
    //Si se está reproduciendo un efecto se actualizará
    if (TestFX()==1){
      if (JIFFY!=0){
        JIFFY=0;
        UpdateFX();
      }
    }
    EnableInterupt();

    

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
          man_game_reproducir_efecto_sonido(9);
        }
        //Le aplicamos un comportamiento a los enemigos según el tipo de enemigo que sea
        sys_ai_update(enemy);
        sys_render_update(enemy);
        if (enemy->y>180) sys_entity_erase_enemy(i);
    }
    //Objetos
    for (char i=0;i<sys_entity_get_num_objects();++i){
      TEntity *object=&array_objects[i];
      sys_render_update(object);
      //Colisión de objetos con player
      if (sys_collider_entity1_collider_entity2(player, object)){
        sys_entity_erase_object(i);
        man_game_reproducir_efecto_sonido(5);
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
      //Si el disparo ha colisionado con algún enemigo..
      for (int w=0; w<sys_entity_get_num_enemies();w++){
        TEntity *enemy=&array_enemies[i];
        if (enemy->x < shot->x + 16 &&  enemy->x + 16 > shot->x && enemy->y < shot->y + 16 && 16 + enemy->y > shot->y){
            sys_entity_erase_enemy(w);
            sys_entity_erase_shot(i);    
            man_game_reproducir_efecto_sonido(2);
        }
      }  
      //Si el objecto se sale de la pantalla lo matamos
      if (shot->x<0 || shot->x>255 || shot->y>180){
        sys_entity_erase_shot(i);
        //shot_plane-=1; 
      } 

    }
    //Manager del juego
    man_game_update();
    //debug
    debug();
    //Pausa
    wait();
  }
}







void man_game_update(){
  //Según el mundo que cargemos tendremos unos enemigos y unos objetos que coger
  //Estos objetos se mstrarán cuando se cargue el mundo
  if (world_change==1){
    if (actual_world==0){
      char fileNameTileMap1[]="world0.bin";
      cargarTileMapEnRAM(fileNameTileMap1);
      pintarPantallaInicio();
    }

    //Creamos los enemigos del mundo correspondiente
    for (int i=0;i<MAX_enemies;i++){ 
      TCoordinate_enemy* coordinate_enemy=&world_enemies[actual_world][i];
        TEntity *enemy=sys_entity_create_enemy1();
        enemy->x=coordinate_enemy->x;
        enemy->y=coordinate_enemy->y;
        enemy->type=coordinate_enemy->type;
        enemy->dir=7;
        enemy->plane=enemy1_plane+sys_entity_get_num_enemies();
    }
    //Creamos los objetos del mundo correspondiente
    for (int i=0; i<MAX_objects;i++){
      TCoordinate_object* coordinate_object=&world_objects[actual_world][i];
        TEntity *object=sys_entity_create_object();
        object->y=coordinate_object->y;
        object->x=coordinate_object->x;
        object->type=coordinate_object->type;
        object->plane=object1_oxigen_plane+sys_entity_get_num_objects();
    }
  }
  world_change=0;
  

  //Creamos el jefe si ha llegado al final de la fase

  if (man_graphics_get_column_counter()==man_graphics_get_num_columns()-1 && old_contador_columna!=man_graphics_get_column_counter()) {
  //if (man_graphics_get_column_counter()==32) {
    TEntity *boss=sys_entity_create_enemy1();
    boss->x=200;
    boss->y=14*8;
    boss->type=entity_type_boss;
    boss->plane=enemy1_plane+sys_entity_get_num_enemies();
  }


  old_contador_columna=man_graphics_get_column_counter();
  
}



void man_game_desplazar_entidades_a_la_izquierda(){
  player->x-=player->vx;
  for (char i=0;i<sys_entity_get_num_enemies();++i){
    TEntity *enemy=&array_enemies[i];
    
    if (enemy->dir==7) {
       enemy->x-=enemy->vx*2;
    }else if(enemy->dir==3){
      //enemy->x-=enemy->vx;
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
  shot->plane=shot_plane+sys_entity_get_num_shots();
  //shot_plane+=1;
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



void debug(){
    //void Rect ( int X1, int Y1, int X2, int Y2, int color, int OP )
    //void BoxFill (int X1, int Y1, int X2, int yY22, char color, char OP )
  
    BoxFill (0, 23*8, 256, 210, 6, LOGICAL_IMP );

    //PutText(10,190, Itoa(player->x/8,"   ",10),0);
    //PutText(50,190, Itoa(player->y/8,"   ",10),0);
    //PutText(100,190, Itoa(man_graphics_get_column_counter(),"   ",10),0);
    //PutText(150,190, Itoa(man_graphics_get_num_columns(),"   ",10),0);



    //TEntity *enemy=&array_enemies[0]; 
    //TEntity *enemy2=&array_enemies[1]; 
    //PutText(0,190,Itoa(enemy->x/8,"  ",10),8);
    //PutText(50,190,Itoa(enemy2->x/8,"  ",10),8);
    //PutText(100,190,Itoa(enemy2->plane,"  ",10),8);
    //PutText(150,190,Itoa(sys_entity_get_num_enemies(),"  ",10),8);


 
    //TEntity *shot=&array_shots[0];
    //PutText(0,190,Itoa(shot->x,"  ",10),8);
    //PutText(50,190,Itoa(shot->y,"  ",10),8);
    //PutText(100,190,Itoa(sys_entity_get_num_shots(),"  ",10),8);
    //PutText(150,190,Itoa(shot->plane,"  ",10),8);
 

    

    //TEntity *object=&array_objects[0];
    //TEntity *object1=&array_objects[1];
    //PutText(0,200,Itoa(object->x/8,"  ",10),8);
    //PutText(50,200,Itoa(object->y/8,"  ",10),8);
    //PutText(100,200,Itoa(object->plane,"  ",10),8);
    //PutText(150,200,Itoa(object1->plane,"  ",10),8);
 
    PutText(150,200,Itoa(TestFX(),"  ",10),8);
    //PutText(200,200,Itoa(man_graphics_get_num_columns(),"  ",10),8);
}

void scoreboard(){
    BoxFill (0, 23*8, 256, 210, 6, LOGICAL_IMP );
    PutText(0,190,"hola",8);
}

void wait(){
    __asm
      halt
      halt
      halt
  __endasm;
}
void cargarBufferDeMusica(){
    FT_SetName( &TFileMusic, &fileNameSong[0]);
    fcb_open( &TFileMusic );
    fcb_read( &TFileMusic, &songBuffer[0], SONG_BUFFER_TAM );  
    fcb_close( &TFileMusic );
}
void cargarBufferDeEfectosDeSonido(){
    afbdata=MMalloc(SONG_EFFECT_TAM);
    FT_SetName( &TFileEffects, "effects.afb");
    fcb_open( &TFileEffects );
    fcb_read( &TFileEffects, afbdata, SONG_EFFECT_TAM );  
    fcb_close( &TFileEffects );
}
void man_game_reproducir_efecto_sonido(char effect){
  //El 1 es un ruido flojo y corto
  //2 dos ruidos cortos y flojos
  // 3 un rebote
  // 4 son 5 captanillas que van desapareciendo
  // 5 3 captanillas cortas
  // 6 rayo laser pistola
  // 7 laser que va desapareciendo
  // 8 explosión que va desapareciendo
  // 9 golpe con explosión
  // 10 una especie de robot
  //man_game_sincronizaFX(150);
  PlayFX(effect);
}

//----- Sincroniza el tiempo de FX ------
void man_game_sincronizaFX(char temp) {

	char i;

	for (i=0; i<temp; i++) {
		
		if (TestFX()==1){
      if (JIFFY!=0){
      JIFFY=0;
          UpdateFX();
      }
    }
	}
}	
//char generar_numero_aleatorio (char a, char b){
//    char random; 
//    random = rand()%(b-a)+a;  
//    return(random);
//}

