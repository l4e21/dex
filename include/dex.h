#ifndef DEX_H
#define DEX_H

#include <tonc.h>
#include <stdlib.h>
#include <string.h>

#include "game_map.h"

OBJ_ATTR obj_buffer[128];
OBJ_AFFINE *obj_aff_buffer= (OBJ_AFFINE*)obj_buffer;

SCR_ENTRY *bg0_map= se_mem[SBB_0];
SCR_ENTRY *bg1_map= se_mem[SBB_1];

enum __DIRECTION {Left, Down, Up, Right, InvalidDirection};
typedef enum __DIRECTION Direction;

enum __INTERACTION {Heart};
typedef enum __INTERACTION Interaction;

struct __MADO {
  int posX;
  int posY;
  Direction facing;
  Interaction interactionType;
  int movement;
  int interacting;
  OBJ_ATTR *sprite;
  OBJ_ATTR *emote;
};

typedef struct __MADO Mado;

#endif
