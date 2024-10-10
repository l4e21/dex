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

struct __MADO {
  int posX;
  int posY;
  OBJ_ATTR *sprite;
};

typedef struct __MADO Mado;

int init_mado(Mado* mado, int x, int y) {
  OBJ_ATTR *sprite = &obj_buffer[0];
  obj_set_attr(sprite,
	       ATTR0_SQUARE,				// Square, regular sprite
	       ATTR1_SIZE_16, 				// 64x64p,
	       ATTR2_PALBANK(0) | 0);		// palbank 0, tile 0
  
  obj_set_pos(sprite, x, y);

  mado->posX = x;
  mado->posY = y;
  mado->sprite = sprite;

  return 0;
};

int draw_room() {
  int row = 1;
  for (int i=0; i<=20*32; i+=2) {
    if (i % 32 == 0) {
      row ^= 1;
    };
    
    bg0_map[i] = SE_PALBANK(1) | (1 + row*2);
    bg0_map[i+1] = SE_PALBANK(1) | (2 + row*2);
  };

  return 0;
};

int game_loop(Mado* mado) {
  int time = 1;
  
  while(1) {
    vid_vsync();
    key_poll();

    // move left/right
    mado->posX += 2*key_tri_horz();

    // move up/down
    mado->posY += 2*key_tri_vert();

    /* if (key_is_down(KEY_DOWN)) { */
    /*   if () { */
    /* 	direction = Down; */
    /*   }; */
    /* } */

    obj_set_pos(mado->sprite, mado->posX, mado->posY);
    oam_copy(oam_mem, obj_buffer, 2);	// only need to update one

    time++;
    
  }
};

int main() {

  // Tile mode, bgs 0, 1
  REG_DISPCNT= DCNT_MODE0 | DCNT_BG0 | DCNT_BG1 | DCNT_OBJ | DCNT_OBJ_1D;
  
  REG_BG0CNT= BG_CBB(CBB_0) | BG_SBB(SBB_0) | BG_REG_64x64 | 1;

  // Position of screen on map
  REG_BG0HOFS= 0;
  REG_BG0VOFS= 0;

  // Room Tiles
  memcpy16(&pal_bg_mem[16], roomPal, roomPalLen / sizeof(u16));
  memcpy32(&tile_mem[CBB_0][1], roomTiles, roomTilesLen / sizeof(u32));
  
  // Mado Sprite
  memcpy32(&tile_mem[4][0], madoTiles, madoTilesLen / sizeof(u32));
  memcpy16(pal_obj_mem, madoPal, madoPalLen / sizeof(u16));
  oam_init(obj_buffer, 128);

  Mado mado = {0};
  init_mado(&mado, 96, 32);

  draw_room();
  game_loop(&mado);
  
};
