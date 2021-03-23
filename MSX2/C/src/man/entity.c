#pragma once
//para el memset
#include <string.h>
#include "src/man/sprites.c"


//================================Declarations
#define entity_type_invalid      0x00  
#define entity_type_player       0x01  
#define entity_type_enemy1       0x02  
#define entity_type_enemy2       0x04  
#define entity_type_enemy3       0x08  
#define entity_type_shot         0x10  
#define entity_type_object_oxigen 0x16  
#define entity_type_object_batery 0x32  
#define entity_type_boss         0x64 
#define entity_type_dead         0x80  
#define entity_type_default      entity_type_enemy

#define entity_cmp_render    0x01  //en binario=0000 0001- Renderable entity, entidad dibujable 
#define entity_cmp_movable   0x02  //en binario=0000 0010- Movable entidad, entidad movible (podemos hacerla no moverla para las vidas) 
#define entity_cmp_input     0x04  //en binario=0000 0100- Entity controllable by input
#define entity_cmp_ai        0x08  //en binario=0000 0100- Entity controllable by ai
#define entity_cmp_animated  0x10  //Animated entity
#define entity_cmp_default   0x00  //DEfault components(all bits =0)



//VARIABLES Y ARRAYS
//Members
typedef struct TEntity TEntity;
struct TEntity{
    unsigned char type;
    unsigned char cmp;
    signed int x,y;
    unsigned char old_x,old_y;
    unsigned char w,h;
    signed char vx,vy;
    unsigned char dir;
    unsigned char andando;
    unsigned char jump;
    unsigned char collision;
    unsigned char plane;
    unsigned char sprite;
    unsigned char color;
    unsigned int energy;
};

#define MAX_enemies 10
#define MAX_shots 10
#define MAX_objects 10
TEntity array_structs_enemies[MAX_enemies];
TEntity array_structs_shots[MAX_shots];
TEntity array_structs_objects[MAX_objects];
char num_enemies;
char num_shots;
char num_objects;
//Functions
void sys_entities_init();
TEntity* sys_entity_create_player();
TEntity* sys_enemy_create_enemy();
TEntity* sys_enemy_create_shot();
TEntity* sys_enemy_create_object();
void sys_entity_erase_enemy(char i);
void sys_entity_erase_shot(char i);
void sys_entity_erase_object(char i);
TEntity* sys_enemy_get_array_structs_enemies();
TEntity* sys_enemy_get_array_structs_fires();
TEntity* sys_enemy_get_array_structs_objects();
char sys_entity_get_num_enemies();
char sys_entity_get_num_shots();
char sys_entity_get_num_objects();
char sys_entity_get_max_enemies();
char sys_entity_get_max_shots();
char sys_entity_get_max_objects();
//===================================End declarations






//=======================================Definitions
const TEntity player_template={
    entity_type_player, // Type
    entity_cmp_movable | entity_cmp_render | entity_cmp_input, //Components 
    8*2,8*18,            //x,y  ,20*8 es el suelo, 8*16 plataforma
    8*1,8*16,           //old position
    16,16,             //width, heigh
    4,8,                 //speed X,speed Y 
    3,                   //direction
    0,                   //is it jumpimg?
    0,                   //is it colliding?
    0,                   //plano, inutilizado
    0,                    //Sprite, inutilizado
    10,                  //Color, inutilizado
    100                  //Enenrgy
};
const TEntity object_template={
    entity_type_object_oxigen, // Type1= el enemigo se cae si encuantra un agujero y rebota con bloque sólido
    entity_cmp_movable | entity_cmp_render, //Components 
    0,0,            //x,y  ,20*8 es el suelo, 8*16 plataforma
    0,0,           //old position
    8,8,             //width, heigh
    8,8,                //speed X,speed Y 
    3,                  //direction
    0,                  //is it jumpimg?
    0,                  //is it colliding?
    object1_oxigen_plane, //plano,  inutilizado
    0,                  //Sprite, inutilizado
    10,                 //Color, inutilizado
    100                 //Enenrgy
};
const TEntity enemy1_template={
    entity_type_enemy1, // Type1= el enemigo se cae si encuantra un agujero y rebota con bloque sólido
    entity_cmp_movable | entity_cmp_render, //Components 
    0,0,            //x,y  ,20*8 es el suelo, 8*16 plataforma
    0,0,           //old position
    8,8,             //width, heigh
    8,8,                //speed X,speed Y 
    3,                  //direction
    0,                  //is it jumpimg?
    0,                  //is it colliding?
    enemy1_plane,       //plano,  inutilizado
    0,                  //Sprite, inutilizado
    10,                 //Color, inutilizado
    100                 //Enenrgy
};
const TEntity shot_template={
    entity_type_shot, // Type1= el enemigo se cae si encuantra un agujero y rebota con bloque sólido
    entity_cmp_movable | entity_cmp_render, //Components 
    0,0,            //x,y  ,20*8 es el suelo, 8*16 plataforma
    0,0,           //old position
    8,8,             //width, heigh
    8,8,                //speed X,speed Y 
    3,                  //direction
    0,                  //is it jumpimg?
    0,                  //is it colliding?
    shot_plane,                  //plano,  inutilizado
    0,                  //Sprite, inutilizado
    10,                 //Color, inutilizado
    100                 //Enenrgy
};


//Life cicle
void sys_entities_init(){
    //Ponemos a 0 todos los valores del array de estructuras
    //void * memset ( void * ptr, int value, size_t num );
    memset(array_structs_enemies,0,sizeof(array_structs_enemies) );
    memset(array_structs_shots,0,sizeof(array_structs_shots) );
    memset(array_structs_objects,0,sizeof(array_structs_objects) );
    num_enemies=0;
    num_shots=0;
    num_objects=0;
}



TEntity* sys_entity_create_player(){
    return &player_template;

}
TEntity* sys_entity_create_enemy1(){
    TEntity* enemy=&array_structs_enemies[num_enemies];
    memcpy(enemy,&enemy1_template,sizeof(TEntity));
    ++num_enemies;
    //enemy->plane=num_enemies*4+enemy1_plane;

    return enemy;
}  
TEntity* sys_entity_create_shot(){
    TEntity* shot=&array_structs_shots[num_shots]; 
    memcpy(shot,&shot_template,sizeof(TEntity));
    ++num_shots;
    //shot->plane=num_objects*4+shot_plane;

    return shot;
}  
TEntity* sys_entity_create_object(){
    TEntity* object=&array_structs_objects[num_objects];
    memcpy(object,&object_template,sizeof(TEntity));
    ++num_objects;
    //object->plane=num_objects*4+object1_oxigen_plane;

    return object;
}  

void sys_entity_erase_enemy(char i){
   --num_enemies;
   TEntity *enemy=&array_structs_enemies[i];
   PutSprite(enemy->plane , player_Jump_left_pattern, 0,212,0 );
   //array_structs_enemies[i]=array_structs_enemies[num_enemies];
   memcpy(&array_structs_enemies[i],&array_structs_enemies[num_enemies],sizeof(TEntity));
}
void sys_entity_erase_shot(char i){
    --num_shots;
    TEntity *shot=&array_structs_shots[i];
    PutSprite(shot->plane, shot_pattern, 0,212,0 );
    //array_structs_shots[i]=array_structs_shots[num_shots];
    memcpy(&array_structs_shots[i],&array_structs_shots[num_shots],sizeof(TEntity));
}
void sys_entity_erase_object(char i){
    --num_objects;
    TEntity *object=&array_structs_objects[i];
    PutSprite(object->plane, object_oxigen_pattern, 0,212,0 );
    //array_structs_objects[i]=array_structs_objects[num_objects];
    memcpy(&array_structs_objects[i],&array_structs_objects[num_objects],sizeof(TEntity));
}  
//End life cicle

//Geters & setters
TEntity* sys_entity_get_array_structs_enemies(){
    return array_structs_enemies;
}
TEntity* sys_entity_get_array_structs_shots(){
    return array_structs_shots;
}
TEntity* sys_entity_get_array_structs_objects(){
    return array_structs_objects;
}


char sys_entity_get_num_enemies(){
    return num_enemies;
}
char sys_entity_get_num_shots(){
    return num_shots;
}
char sys_entity_get_num_objects(){
    return num_objects;
}


char sys_entity_get_max_enemies(){
    return MAX_enemies;
}
char sys_entity_get_max_shot(){
    return MAX_enemies;
}
char sys_entity_get_max_objects(){
    return MAX_enemies;
}
//End getters & setters









