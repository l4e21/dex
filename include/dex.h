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

enum __INTERACTION {Heart};
typedef enum __INTERACTION Interaction;

enum __CAMERA_BEHAVIOUR {Static, Following};
typedef enum __CAMERA_BEHAVIOUR CameraBehaviour;

struct __MADO {
  int pos_x;
  int pos_y;
  Direction facing;
  Interaction interactionType;
  int movement;
  int interacting;
  OBJ_ATTR *sprite;
  OBJ_ATTR *emote;
  GameMap game_map;
};

typedef struct __MADO Mado;

struct __CAMERA {
  CameraBehaviour behaviour;
  int pos_x;
  int pos_y;
};

typedef struct __CAMERA Camera;

#endif
