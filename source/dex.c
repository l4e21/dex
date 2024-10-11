#include "dex.h"

int time = 1;
int room = 0; // Bedroom
int a_held = 0; // Bedroom

int extract_tile_idx(SCR_ENTRY tile) {
  return tile & SE_ID_MASK;
}


int tile_is_solid(int tile_idx) {
  return (// Walls
	  tile_idx == 9 || tile_idx == 10 || tile_idx == 11 || tile_idx == 12
	  || tile_idx == 13 || tile_idx == 14|| tile_idx == 15 || tile_idx == 16
	  // Blackspace
	  || tile_idx == 0 || tile_idx == 1 || tile_idx == 2 || tile_idx == 3
	  // Beds
	  || tile_idx == 69 || tile_idx == 70 || tile_idx == 71 || tile_idx == 72
	  || tile_idx == 81 || tile_idx == 82 || tile_idx == 83 || tile_idx == 84
	  || tile_idx == 93 || tile_idx == 94 || tile_idx == 95 || tile_idx == 96
	  || tile_idx == 109 || tile_idx == 110 || tile_idx == 111 || tile_idx == 112

	  );
}


int init_mado(Mado* mado, int x, int y) {
  OBJ_ATTR *sprite = &obj_buffer[0];
  OBJ_ATTR *emote = &obj_buffer[1];
  obj_set_attr(sprite,
	       ATTR0_SQUARE,				// Square, regular sprite
	       ATTR1_SIZE_16, 				// 64x64p,
	       ATTR2_PALBANK(0) | ATTR2_PRIO(1) | 0);		// palbank 0, tile 0
  
  obj_set_pos(sprite, x, y);

  obj_set_attr(emote,
	       ATTR0_SQUARE,				// Square, regular sprite
	       ATTR1_SIZE_16, 				// 64x64p,
	       ATTR2_PALBANK(1) | ATTR2_PRIO(1) | 40);		// palbank 0, tile 0
  
  obj_set_pos(emote, x, y-64);
  
  mado->facing = None;
  mado->movement = 0;
  mado->posX = x;
  mado->posY = y;
  mado->sprite = sprite;
  mado->emote = emote;

  return 0;
};

int turn_mado(Mado* mado) {
  if (mado->facing == Up) {
    obj_set_attr(mado->sprite,
		 ATTR0_SQUARE,		
		 ATTR1_SIZE_16,      
		 ATTR2_PALBANK(0) | ATTR2_PRIO(1) | 4);	      
  }
  else if (mado->facing == Down) {
    obj_set_attr(mado->sprite,
		 ATTR0_SQUARE,		
		 ATTR1_SIZE_16,      
		 ATTR2_PALBANK(0) | ATTR2_PRIO(1) | 0);	      
  }
  else if (mado->facing == Right) {
    obj_set_attr(mado->sprite,
		 ATTR0_SQUARE,		
		 ATTR1_SIZE_16,      
		 ATTR2_PALBANK(0) | ATTR2_PRIO(1) | 12);	      
  }
  else if (mado->facing == Left) {
    obj_set_attr(mado->sprite,
		 ATTR0_SQUARE,		
		 ATTR1_SIZE_16,      
		 ATTR2_PALBANK(0) | ATTR2_PRIO(1) | 8);	      
  };
  return 0;
};


int turn_moving_mado(Mado* mado) {
  if (mado->facing == Up) {
    obj_set_attr(mado->sprite,
		 ATTR0_SQUARE,		
		 ATTR1_SIZE_16,      
		 ATTR2_PALBANK(0)  | ATTR2_PRIO(1) | (mado->movement < 4 ? 24 : 28));	      
  }
  else if (mado->facing == Down) {
    obj_set_attr(mado->sprite,
		 ATTR0_SQUARE,		
		 ATTR1_SIZE_16,      
		 ATTR2_PALBANK(0) | ATTR2_PRIO(1) | (mado->movement < 4 ? 16 : 20));	      
  }
  else if (mado->facing == Right) {
    obj_set_attr(mado->sprite,
		 ATTR0_SQUARE,		
		 ATTR1_SIZE_16,      
		 ATTR2_PALBANK(0) | ATTR2_PRIO(1) | 36);	      
  }
  else if (mado->facing == Left) {
    obj_set_attr(mado->sprite,
		 ATTR0_SQUARE,		
		 ATTR1_SIZE_16,      
		 ATTR2_PALBANK(0) | ATTR2_PRIO(1) | 32);	      
  };
  return 0;
};

int mado_try_move(Mado* mado) {
  // Mado's position is set to the top left of the sprite
  switch (mado->facing) {
  case Up:
    if (!tile_is_solid(extract_tile_idx(bg0_map[(mado->posX/8) + 32*((mado->posY/8))]))
	&&
	!tile_is_solid(extract_tile_idx(bg0_map[((mado->posX/8)+1) + 32*((mado->posY/8))]))) {
      mado->movement = 8;
    }
    break;
  case Down:
    if(!tile_is_solid(extract_tile_idx(bg0_map[(mado->posX/8) + 32*((mado->posY/8) + 3)]))
       &&
       !tile_is_solid(extract_tile_idx(bg0_map[((mado->posX/8) + 1) + 32*((mado->posY/8) + 3)]))
       ) {
      mado->movement = 8;
    }
    break; 
  case Left:
    if(!tile_is_solid(extract_tile_idx(bg0_map[((mado->posX/8) - 1) + 32*((mado->posY/8) + 2)]))
       &&
       !tile_is_solid(extract_tile_idx(bg0_map[((mado->posX/8) - 1) + 32*((mado->posY/8) + 2)]))
       ) {
      mado->movement = 8;
    }
    break;
  case Right:
    if(!tile_is_solid(extract_tile_idx(bg0_map[((mado->posX/8) + 2) + 32*((mado->posY/8) + 2)]))
       &&
       !tile_is_solid(extract_tile_idx(bg0_map[((mado->posX/8) + 2) + 32*((mado->posY/8) + 2)]))) {
      mado->movement = 8;
    }
    break;
  case None:
    break;
  };
  return 0;
};


int move_mado(Mado* mado) {
  // Check if mado can move, if so set movement
  if (!mado->movement && !mado->interacting) {
    if (key_is_down(KEY_UP)) {
      mado->facing = Up;
      mado_try_move(mado);
    }    
    else if (key_is_down(KEY_DOWN)) {
      mado->facing = Down;
      mado_try_move(mado);
    }    
    else if (key_is_down(KEY_RIGHT)) {
      mado->facing = Right;
      mado_try_move(mado);
    }    
    else if (key_is_down(KEY_LEFT)) {
      mado->facing = Left;
      mado_try_move(mado);
    }
    
  }

  else if (mado->interacting) {
    // Can't move during an interaction
    return 0;
  }

  // If there is movement left, then move
  else {
    switch (mado->facing) {
    case Up:
      mado->posY -= 1;
      mado->movement -= 1;
      break;
    case Down:
      mado->posY += 1;
      mado->movement -= 1;
      break;
    case Left:
      mado->posX -= 1;
      mado->movement -= 1;
      break;
    case Right:
      mado->posX += 1;
      mado->movement -= 1;
      break;

    case None:
      // This is an error situation so don't move?
      break;

    };
  };

  if (!mado->movement) {
    turn_mado(mado);
  }
  else {
    turn_moving_mado(mado);
  };
    
		  
  obj_set_pos(mado->sprite, mado->posX, mado->posY);
  obj_set_pos(mado->emote, mado->posX, mado->posY-16);
  
  return 0;
};



int interact(Mado* mado) {
  // Check if mado can interact, if so interact
  if (!mado->interacting && !mado->movement) {
    if (key_is_down(KEY_A)) {
      mado->interactionType = Heart;
      mado->interacting = 24;
      obj_unhide(mado->emote, 0);
    }        
  }

  else if (mado->movement) {
    // Can't interact during moving
    return 0;
  }

  else {
    // Interaction is occurring
    switch (mado->interactionType) {
    case Heart:
      if (mado->interacting == 1) {
	obj_set_attr(mado->emote,
		     ATTR0_SQUARE,				// Square, regular sprite
		     ATTR1_SIZE_16, 				// 64x64p,
		     ATTR2_PALBANK(1) | ATTR2_PRIO(1) | 40);		// palbank 0, tile 0
	obj_set_pos(mado->emote, mado->posX, mado->posY-16);
	obj_hide(mado->emote);
      }
      else {
	obj_set_attr(mado->emote,
		     ATTR0_SQUARE,				// Square, regular sprite
		     ATTR1_SIZE_16, 				// 64x64p,
		     ATTR2_PALBANK(1) | ATTR2_PRIO(1) | (40 + (mado->interacting > 16 ? 0 : (mado->interacting > 8 ? 4 : 8))));		// palbank 0, tile 0
	obj_set_pos(mado->emote, mado->posX, mado->posY-16);
      };
      break;
    }
    mado->interacting -=1;
  };
  
  return 0;
};

int draw_16_by_16(int idx, int pal, int tile_idx) {
  bg0_map[idx] = SE_PALBANK(pal) | tile_idx;
  bg0_map[idx+1] = SE_PALBANK(pal) | (tile_idx+1);
  bg0_map[idx+32] = SE_PALBANK(pal) | (tile_idx+2);
  bg0_map[idx+33] = SE_PALBANK(pal) | (tile_idx+3);
  return 0;
};

int draw_room() {
  int row = 1;
  int col = 1;
  int tile_idx = 0;
  
  for (int i=0; i<=20*32; i++) {
    if (i % 32 == 0) {
      // Newline logic
      row ^= 1;
    };

    // Alternate tiles because 16 by 16
    col ^= 1;

    if ((i % 32 > 3) && (i % 32 < 26)) {

      if (i < 64) {
	tile_idx = 0;
      }

      // Wall
      else if (i < 128) {
	tile_idx = 9;
      }

      // Wall
      else if (i < 192) {
	tile_idx = 13;
      }
      
      // Floor
      else if (i < 544) {
	tile_idx = 17;
      }

      else {
	tile_idx = 0;
      }
      
    }

    else {
      tile_idx = 0;
    };

    bg0_map[i] = SE_PALBANK(0) | (tile_idx + col + row*2);

    if (i < 544 && i > 64 && (i % 32 > 2) && (i % 32 < 27)) {
    bg1_map[i] = SE_PALBANK(2) | (69 + col + row*2);
    };
  };

  // Details

  // Bed L
  draw_16_by_16(198, 1, 65);
  draw_16_by_16(200, 1, 69);
  draw_16_by_16(202, 1, 73);
  
  draw_16_by_16(262, 1, 77);
  draw_16_by_16(264, 1, 81);
  draw_16_by_16(266, 1, 85);

  draw_16_by_16(328, 1, 101);
  
  // Bed R
  draw_16_by_16(210, 1, 89);
  draw_16_by_16(212, 1, 93);
  draw_16_by_16(214, 1, 97);
  
  draw_16_by_16(274, 1, 105);
  draw_16_by_16(276, 1, 109);
  draw_16_by_16(278, 1, 85);

  draw_16_by_16(340, 1, 101);
  

  // Stairs
  draw_16_by_16(342, 0, 41);
  draw_16_by_16(344, 0, 45);
  draw_16_by_16(408, 0, 49);
  draw_16_by_16(406, 0, 53);
  draw_16_by_16(280, 0, 57);

  // Cracks etc
  draw_16_by_16(432, 0, 25);
  /* draw_16_by_16(408, 0, 29); */
  draw_16_by_16(470, 0, 33);
  draw_16_by_16(240, 0, 37);
  draw_16_by_16(110, 0, 5);
  draw_16_by_16(400, 0, 29);
  
  return 0;
};


int game_loop(Mado* mado) {
  int rain_offset = 69;
  
  while(1) {
    vid_vsync();
    key_poll();
    move_mado(mado);
    interact(mado);
    
    oam_copy(oam_mem, obj_buffer, 16);	// only need to update one

    if (time % 2 == 0) {
      rain_offset -= 4;
      if (rain_offset < 0) {
	rain_offset = 69;
      };
      
      memcpy32(&tile_mem[CBB_1][rain_offset], rainTiles, rainTilesLen / sizeof(u32));
    }; 

    time++;
    
  }
};

int main() {

  // Tile mode, bgs 0, 1
  REG_DISPCNT= DCNT_MODE0 | DCNT_BG0 | DCNT_BG1 | DCNT_OBJ | DCNT_OBJ_1D;
  
  REG_BG0CNT= BG_CBB(CBB_0) | BG_SBB(SBB_0) | BG_REG_64x64 | BG_PRIO(1);

  REG_BG1CNT= BG_CBB(CBB_1) | BG_SBB(SBB_1) | BG_REG_64x64 | BG_PRIO(0);

  
  // Position of screen on map
  REG_BG0HOFS= 0;
  REG_BG0VOFS= 0;

  REG_BG1HOFS= 0;
  REG_BG1VOFS= 0;

  // Room Tiles
  dma3_cpy(&pal_bg_mem[0], roomPal, roomPalLen / sizeof(u16));
  memcpy32(&tile_mem[CBB_0][1], roomTiles, roomTilesLen / sizeof(u32));

  /* // Bed Tiles */
  dma3_cpy(&pal_bg_mem[16], bedPal, bedPalLen / sizeof(u16));
  memcpy32(&tile_mem[CBB_0][61], bedTiles, bedTilesLen / sizeof(u32));

  /* // rain Tiles */
  dma3_cpy(&pal_bg_mem[32], rainPal, rainPalLen / sizeof(u16));
  memcpy32(&tile_mem[CBB_1][1], rainTiles, rainTilesLen / sizeof(u32));
  
  // Mado Sprite
  memcpy32(&tile_mem[4][0], madoTiles, madoTilesLen / sizeof(u32));
  dma3_cpy(pal_obj_mem, madoPal, madoPalLen / sizeof(u16));

  // Emotes Sprite
  memcpy32(&tile_mem[4][40], emotesTiles, emotesTilesLen / sizeof(u32));
  dma3_cpy(&pal_obj_mem[16], emotesPal, emotesPalLen/ sizeof(u16));

  oam_init(obj_buffer, 128);

  Mado mado = {0};
  init_mado(&mado, 96, 100);
  
  draw_room();
  game_loop(&mado);
  
};
