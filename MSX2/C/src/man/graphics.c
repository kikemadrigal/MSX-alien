#pragma once
#include "fusion-c/header/msx_fusion.h"
#include "fusion-c/header/vdp_graph2.h"
#include "src/man/entity.c"
/**************DECLARACIONES************/

//FUNCIONES
void inicializarPantalla();
//Cargar archivos en memoria RAM y VRAM
void cargarTileSetEnRAM();
//void cargarTileMapEnRAM();
void cargarTileMapEnRAM(char* map);
//Esto es para asociar un archivo al struct de archivos
void FT_SetName( FCB *p_fcb, const char *p_name );
//Esto me lo he inventado, :)
//void cargarArrayFilasTileMap();
void cargarLoaderEnRAM();
void recorrerBufferTileMapYPintarPage1EnPage0();
void pintarPantallaInicio();
void deRamAVramPage1(void);
void deRamAVramPage0(void);
int man_graphics_get_column_counter();
char* man_graphics_get_buffer_tile_map();

void SetAdjust(signed char x, signed char y);

char man_graphics_get_tile_suelo();

int man_graphics_get_tile_left_array(TEntity *entity);
int man_graphics_get_tile_right_array(TEntity *entity);
int get_tile_down_array(TEntity *entity);
int get_tile_down_right_array(TEntity *entity);
int get_tile_down_left_array(TEntity *entity);

int man_graphics_get_column_entity(TEntity *entity);
//int get_tile_right_entity(TEntity *entity);
int get_fila_entity(TEntity *entity);

//VARIABLES Y ARRAYS
char fileNameLoader[]="loader.sc5";
char fileNameTilseSet[]="tileset.sc5";
//char fileNameTileMap1[]="world0.bin";
FCB TFileTileSet;
FCB TFileTileMap;
//En screen5 cada byte define 2 colores, entonces 256px de ancho/2=128 bytes*212 filas=27136 bytes para definir una page
//En el ejemplo de fusion c cada 20 líneas son 2560 bytes
#define tamanobufferTileSet 27136
#define numeroFilas 23
#define numeroColumnas 200
#define tamanoBufferTileMap 11500 // numeroFilas*numeroColumnas
unsigned char bufferTileSetYMap[tamanobufferTileSet];
unsigned int contadorColumnaArray;

char tile_suelo;

/********FINAL DE DECLARAIONES***********/




/**************DEFINICIONES************/
void inicializarPantalla(){
  contadorColumnaArray=32;
  tile_suelo=160;
}

int man_graphics_get_column_counter(){
  return contadorColumnaArray;
}
void man_graphics_set_column_counter(int col_counter){
  contadorColumnaArray+=col_counter;
}
int man_graphics_get_num_columns(){
  //int num_columns=numeroColumnas;
  return numeroColumnas;
}
char* man_graphics_get_buffer_tile_map(){
    return bufferTileSetYMap;
}

void cargarLoaderEnRAM(){
    //Cargamos el archivo en la estructura
    FT_SetName( &TFileTileSet, &fileNameLoader[0] );
    fcb_open( &TFileTileSet );
    //Sino omitimos los 7 primeros bytes (los que defininen la estructura del binario) aparecen  unas marcas arriba a la derecha
    fcb_read( &TFileTileSet, &bufferTileSetYMap[0], 7 ); 
    //Cargamos el archivo definido en la estructura y lo ponemos en la RAM (buffer)
    fcb_read( &TFileTileSet, &bufferTileSetYMap[0], tamanobufferTileSet );  
    fcb_close( &TFileTileSet );
}

void cargarTileSetEnRAM(){
    //Cargamos el archivo en la estructura
    FT_SetName( &TFileTileSet, &fileNameTilseSet[0] );
    fcb_open( &TFileTileSet );
    //Sino omitimos los 7 primeros bytes (los que defininen la estructura del binario) aparecen  unas marcas arriba a la derecha
    fcb_read( &TFileTileSet, &bufferTileSetYMap[0], 7 ); 
    //Cargamos el archivo definido en la estructura y lo ponemos en la RAM (buffer)
    fcb_read( &TFileTileSet, &bufferTileSetYMap[0], tamanobufferTileSet );  
    fcb_close( &TFileTileSet );
}

/*void cargarTileMapEnRAM(){
    FT_SetName( &TFileTileMap, &fileNameTileMap[0] );
    fcb_open( &TFileTileMap );
    //Analizando el archivo word0.bin con un editor hexadecimal vemos que hay que saltar 8 bytes que definen al .bin
    fcb_read( &TFileTileMap, &bufferTileMap[0], 7 );  // Skip 7 first bytes of the file  
    fcb_read( &TFileTileMap, &bufferTileMap[0], tamanoBufferTileMap );  // Read 20 lines of image data (128bytes per line in screen5)
    fcb_close( &TFileTileMap);
}*/
void cargarTileMapEnRAM(char* map){
    //FT_SetName( &TFileTileMap, &fileNameTileMap1[0] );
    FT_SetName( &TFileTileMap, map );
    fcb_open( &TFileTileMap );
    //Analizando el archivo word0.bin con un editor hexadecimal vemos que hay que saltar 8 bytes que definen al .bin
    fcb_read( &TFileTileMap, &bufferTileSetYMap[0], 8 );  // Skip 7 first bytes of the file  
    fcb_read( &TFileTileMap, &bufferTileSetYMap[-1], tamanoBufferTileMap );  // Read 20 lines of image data (128bytes per line in screen5)
    fcb_close( &TFileTileMap);
}
void deRamAVramPage1(void){
  //HMMC transfiere bloques de RAM a VRAM rápidamente en un area rectangular, ver www.tipolisto.es/files/v9938 página 66
  //HMMM(buffer en RAM, posición_x, posición_Y (256 será la page 1), ancho copia, alto copia)
  HMMC(&bufferTileSetYMap[0], 0,256,256,212 ); 
  //LMMC(&bufferTileSetYMap[0], 0,256,256,212,LOGICAL_TAND); 
}
void deRamAVramPage0(void){
  //HMMC transfiere bloques de RAM a VRAM rápidamente en un area rectangular, ver www.tipolisto.es/files/v9938 página 66
  //HMMM(buffer en RAM, posición_x, posición_Y (256 será la page 1), ancho copia, alto copia)
  HMMC(&bufferTileSetYMap[0], 0,0,256,212 ); 
  //LMMC(&bufferTileSetYMap[0], 0,0,256,212,LOGICAL_TAND); 
}



void FT_SetName( FCB *p_fcb, const char *p_name )  // Routine servant à vérifier le format du nom de fichier
{
  char i, j;
  memset( p_fcb, 0, sizeof(FCB) );
  for( i = 0; i < 11; i++ ) {
    p_fcb->name[i] = ' ';
  }
  for( i = 0; (i < 8) && (p_name[i] != 0) && (p_name[i] != '.'); i++ ) {
    p_fcb->name[i] =  p_name[i];
  }
  if( p_name[i] == '.' ) {
    i++;
    for( j = 0; (j < 3) && (p_name[i + j] != 0) && (p_name[i + j] != '.'); j++ ) {
      p_fcb->ext[j] =  p_name[i + j] ;
    }
  }
}




void recorrerBufferTileMapYPintarPage1EnPage0(){
  HMMM(8,0, 0,0,256,184);
  
  for (int f=0; f<numeroFilas;f++){
    HMMM(((bufferTileSetYMap[f*numeroColumnas+contadorColumnaArray]-(bufferTileSetYMap[f*numeroColumnas+contadorColumnaArray]/32)*32 ) )*8,(bufferTileSetYMap[f*numeroColumnas+contadorColumnaArray]/32)*8+256, 256-8,8*f,8,8);
    //LMMM(((bufferTileSetYMap[f*numeroColumnas+contadorColumnaArray]-(bufferTileSetYMap[f*numeroColumnas+contadorColumnaArray]/32)*32 ) )*8,(bufferTileSetYMap[f*numeroColumnas+contadorColumnaArray]/32)*8+256, 256-8,8*f,8,8, LOGICAL_TAND);
  }
  contadorColumnaArray++;
}


void pintarPantallaInicio(){
  for (int f=0; f<numeroFilas;f++){
    for (int c=0; c<32;c++){
      HMMM(((bufferTileSetYMap[f*numeroColumnas+c]-(bufferTileSetYMap[f*numeroColumnas+c]/32)*32))*8,(bufferTileSetYMap[f*numeroColumnas+c]/32)*8+256, c*8,f*8,8,8);
    }
  }
}





void SetAdjust(signed char x, signed char y) // x and y must be between -7 and +6
{
    char vx,vy,value;
      vx=x;
      if (x<0)
        vx=16+x;
      vy=y;
      if (y<0)
        vy=16+y;
      value = (vy<<4) | vx ;
      Poke(0xFFF1,value);     // Reg 18 Save
      VDPwrite(18,value);
}
void verArray(){
  int columna=0;
  int fila=0;
  for (int i=0; i<0x20; i++){
    //Aquí quewremos ver la línea 23
    PutText(columna*24,fila,Itoa(bufferTileSetYMap[22*numeroColumnas+i],"  ",10),8);
    columna++;
    if (columna==10){
      columna=0;
      fila+=8;
    } 
  }
}

char man_graphics_get_tile_suelo(){
  return tile_suelo;
}


int man_graphics_get_column_entity(TEntity *entity){
  //Será la posición en x/8 pixeles el tile
  int tile_column=(entity->x/8)+(contadorColumnaArray-32);
  return tile_column;
}

int get_fila_entity(TEntity *entity){
  //Será la posición y / 8 pixeles
  int tile_file=(entity->y/8);
  return tile_file;
}

int man_graphics_get_tile_left_array(TEntity *entity){
  //Recuerda que el sprite es de 16*16, es decir hay que bajar 1 tiles para ver el pie,
  //hay mover a la izquierda 1 tile para ver el tile izquierdo y 2 tiles a la derecha para ver el tile de la derecha
  int tile_izquierdo=bufferTileSetYMap[((get_fila_entity(entity)+1)*numeroColumnas)+man_graphics_get_column_entity(entity)-1];
  return tile_izquierdo;
}
int man_graphics_get_tile_right_array(TEntity *entity){
  int tile_derecho=bufferTileSetYMap[((get_fila_entity(entity)+1)*numeroColumnas)+(man_graphics_get_column_entity(entity)+2)];
  return tile_derecho;
}
int get_tile_down_array(TEntity *entity){
  int tile_abajo=bufferTileSetYMap[((get_fila_entity(entity)+2)*numeroColumnas)+(man_graphics_get_column_entity(entity)+1)];
  return tile_abajo;
}
int get_tile_down_right_array(TEntity *entity){
  int tile_abajo=bufferTileSetYMap[((get_fila_entity(entity)+2)*numeroColumnas)+(man_graphics_get_column_entity(entity))];
  return tile_abajo;
}
int get_tile_down_left_array(TEntity *entity){
  int tile_abajo=bufferTileSetYMap[((get_fila_entity(entity)+2)*numeroColumnas)+(man_graphics_get_column_entity(entity)+2)];
  return tile_abajo;
}