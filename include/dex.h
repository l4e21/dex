#include <tonc.h>
#include <stdlib.h>
#include <string.h>
#include "mado.h"
#include "room.h"

#define CBB_0  0
#define SBB_0 28

OBJ_ATTR obj_buffer[128];
OBJ_AFFINE *obj_aff_buffer= (OBJ_AFFINE*)obj_buffer;
SCR_ENTRY *bg0_map= se_mem[SBB_0];

enum __DIRECTION {Left, Down, Up, Right, None};
typedef enum __DIRECTION Direction;

struct __MADO {
  int posX;
  int posY;
  Direction facing;
  int movement;
  OBJ_ATTR *sprite;
};

typedef struct __MADO Mado;
