#pragma once
#include "src/man/entity.c"

//=================Declarations
//Functions
void man_game_init();
void man_game_play();
void man_game_update();
void man_game_desplazar_entidades_a_la_izquierda();
void man_game_crear_disparo_player();
void man_game_crear_disparo_boss(TEntity *enemy);
void man_han_matado_al_player();
char man_game_get_world();
void scoreboard();
void debug();
void wait();
void cargarBufferDeMusica();
void cargarBufferDeEfectosDeSonido();
void man_game_reproducir_efecto_sonido(char efecto);
//Members
//Music
#define SONG_BUFFER_TAM 264
unsigned char songBuffer[SONG_BUFFER_TAM];  
char fileNameSong[]={"song1.pt3"};
FCB TFileMusic;
//Efectos
#define SONG_EFFECT_TAM 1207
//unsigned char songEffectsBuffer[SONG_EFFECT_TAM]; 
//char fileNameSongEffects[]={"effects.afb"};
FCB TFileEffects; 

TEntity* player;
TEntity* array_enemies;
TEntity* array_shots;
TEntity* array_objects;
char actual_world;
char world_change;
char number_shot;
unsigned int contador_columna;
int numero_columnas;
unsigned int old_contador_columna;
#define numero_mundos 2
//#define enemigos_por_mundo 10  
//#define objetos_por_mundo 10 

//char world_enemies[numero_mundos][MAX_enemies]={
//  {32,74,72,73,74,75,131,142,185,186},
//  {32,33,34,60,65,135,145,155,160,171}
//};
typedef struct TCoordinate_enemy TCoordinate_enemy;
struct TCoordinate_enemy{
    unsigned int x;
    unsigned int y;
    unsigned char type;
}; 


TCoordinate_enemy world_enemies[][MAX_enemies]={
    {//wolrd 0
        { //coordinate_object 0
          32*8,
          20*8,
          entity_type_enemy1
        },
        { //coordinate_object 1
          42*8,
          20*8,
          entity_type_enemy1
        },
        { //coordinate_object 2
          65*8,
          20*8,
          entity_type_enemy1
        },
        { //coordinate_object 3
          72*8,
          20*8,
          entity_type_enemy1
        },
        { //coordinate_object 4
          72*8,
          17*8,
          entity_type_enemy1
        },
        { //coordinate_object 5
          77*8,
          18*8,
          entity_type_enemy1
        },
        { //coordinate_object 6
          118*8,
          14*8,
          entity_type_enemy1
        },
        { //coordinate_object 7
          144*8,
          20*8,
          entity_type_enemy1
        },
        { //coordinate_object 8
          160*8,
          20*8,
          entity_type_enemy1
        },
        { //coordinate_object 9
          174*8,
          20*8,
          entity_type_enemy1
        }
  },
  {//world 1
    {
      8*8,
      20*8,
      entity_type_enemy1
    },
    {
      5*8,
      15*8,
      entity_type_enemy1
    },
    {
      25*8,
      20*8,
      entity_type_enemy1
    },
    {
      25*8,
      20*8,
      entity_type_enemy1
    }
  }
};










typedef struct TCoordinate_object TCoordinate_object;
struct TCoordinate_object{
    unsigned int x;
    unsigned int y;
    unsigned char type;
}; 
TCoordinate_object world_objects[][MAX_objects]={
    {//wolrd 0
        { //coordinate_object 0
          9*8,
          16*8,
          entity_type_object_oxigen
        },
        { //coordinate_object 1
          25*8,
          20*8,
          entity_type_object_oxigen
        },
        { //coordinate_object 2
          37*8,
          13*8,
          entity_type_object_oxigen
        },
        { //coordinate_object 3
          52*8,
          17*8,
          entity_type_object_oxigen
        },
        { //coordinate_object 4
          64*8,
          15*8,
          entity_type_object_oxigen
        },
        { //coordinate_object 5
          80*8,
          20*8,
          entity_type_object_oxigen
        },
        { //coordinate_object 6
          100*8,
          20*8,
          entity_type_object_oxigen
        },
        { //coordinate_object 7
          163*8,
          20*8,
          entity_type_object_oxigen
        },
        { //coordinate_object 8
          100*8,
          20*8,
          entity_type_object_oxigen
        },
        { //coordinate_object 9
          110*8,
          20*8,
          entity_type_object_oxigen
        }
  },
  {//world 1
    {
      8*8,
      14*8,
      entity_type_object_oxigen
    },
    {
      5*8,
      15*8,
      entity_type_object_oxigen
    },
    {
      25*8,
      20*8,
      entity_type_object_batery
    },
    {
      25*8,
      20*8,
      entity_type_object_batery
    }
  }
};
