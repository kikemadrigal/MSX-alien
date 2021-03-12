#pragma once
#include "fusion-c/header/msx_fusion.h"
#include "src/man/entity.c"
char sys_collider_entity1_collider_entity2(TEntity *entity1, TEntity *entity2);




char sys_collider_entity1_collider_entity2(TEntity *entity1, TEntity *entity2){
    //if (enemiX < player.x + 16 &&  enemiX + 16 > player.x && enemiY < player.y + 32 && 16 + enemiY > player.y){
    if (entity2->x < entity1->x + 16 &&  entity2->x + 16 > entity1->x && entity2->y < entity1->y + 16 && 16 + entity2->y > entity1->y){
        return 1;
    }else{
        return 0;
    }
}
