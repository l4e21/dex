#include "dex.h"

int extract_tile_idx(SCR_ENTRY tile) {
  return tile & SE_ID_MASK;
}


int tile_is_solid(int tile_idx) {
  return (// Walls
	  tile_idx == 9 || tile_idx == 10 || tile_idx == 11 || tile_idx == 12
	  || tile_idx == 73 || tile_idx == 74|| tile_idx == 75 || tile_idx == 76
	  // Blackspace
	  || tile_idx == 0 || tile_idx == 1 || tile_idx == 2 || tile_idx == 3
	  // Beds
	  || tile_idx == 21 || tile_idx == 22 || tile_idx == 23 || tile_idx == 24
	  || tile_idx == 33 || tile_idx == 34 || tile_idx == 35 || tile_idx == 36
	  /* || tile_idx == 18 || tile_idx == 20 */
	  /* || tile_idx == 30 || tile_idx == 32 */
	  || tile_idx == 65 || tile_idx == 66 || tile_idx == 67 || tile_idx == 68

	  );
}


int init_mado(Mado* mado, int x, int y) {
  OBJ_ATTR *sprite = &obj_buffer[0];
  obj_set_attr(sprite,
	       ATTR0_SQUARE,				// Square, regular sprite
	       ATTR1_SIZE_16, 				// 64x64p,
	       ATTR2_PALBANK(0) | 0);		// palbank 0, tile 0
  
  obj_set_pos(sprite, x, y);

  mado->facing = None;
  mado->movement = 0;
  mado->posX = x;
  mado->posY = y;
  mado->sprite = sprite;

  return 0;
};

int turn_mado(Mado* mado) {
  if (mado->facing == Up) {
    obj_set_attr(mado->sprite,
		 ATTR0_SQUARE,		
		 ATTR1_SIZE_16,      
		 ATTR2_PALBANK(0) | 4);	      
  }
  else if (mado->facing == Down) {
    obj_set_attr(mado->sprite,
		 ATTR0_SQUARE,		
		 ATTR1_SIZE_16,      
		 ATTR2_PALBANK(0) | 0);	      
  }
  else if (mado->facing == Right) {
    obj_set_attr(mado->sprite,
		 ATTR0_SQUARE,		
		 ATTR1_SIZE_16,      
		 ATTR2_PALBANK(0) | 12);	      
  }
  else if (mado->facing == Left) {
    obj_set_attr(mado->sprite,
		 ATTR0_SQUARE,		
		 ATTR1_SIZE_16,      
		 ATTR2_PALBANK(0) | 8);	      
  };
  return 0;
};


int turn_moving_mado(Mado* mado) {
  if (mado->facing == Up) {
    obj_set_attr(mado->sprite,
		 ATTR0_SQUARE,		
		 ATTR1_SIZE_16,      
		 ATTR2_PALBANK(0) | (mado->movement < 4 ? 24 : 28));	      
  }
  else if (mado->facing == Down) {
    obj_set_attr(mado->sprite,
		 ATTR0_SQUARE,		
		 ATTR1_SIZE_16,      
		 ATTR2_PALBANK(0) | (mado->movement < 4 ? 16 : 20));	      
  }
  else if (mado->facing == Right) {
    obj_set_attr(mado->sprite,
		 ATTR0_SQUARE,		
		 ATTR1_SIZE_16,      
		 ATTR2_PALBANK(0) | 36);	      
  }
  else if (mado->facing == Left) {
    obj_set_attr(mado->sprite,
		 ATTR0_SQUARE,		
		 ATTR1_SIZE_16,      
		 ATTR2_PALBANK(0) | 32);	      
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
  if (!mado->movement) {
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
	tile_idx = 73;
      }
      
      // Floor
      else if (i < 544) {
	tile_idx = 13;
      }

      else {
	tile_idx = 0;
      }
      
    }

    else {
      tile_idx = 0;
    };

    bg0_map[i] = SE_PALBANK(0) | (tile_idx + col + row*2);
    tile_idx = 9;
  };

  // Details

  // Bed L
  bg0_map[198] = SE_PALBANK(0) | 17;
  bg0_map[199] = SE_PALBANK(0) | 18;
  bg0_map[230] = SE_PALBANK(0) | 19;
  bg0_map[231] = SE_PALBANK(0) | 20;
  bg0_map[200] = SE_PALBANK(0) | 21;
  bg0_map[201] = SE_PALBANK(0) | 22;
  bg0_map[232] = SE_PALBANK(0) | 23;
  bg0_map[233] = SE_PALBANK(0) | 24;
  bg0_map[202] = SE_PALBANK(0) | 25;
  bg0_map[203] = SE_PALBANK(0) | 26;
  bg0_map[234] = SE_PALBANK(0) | 27;
  bg0_map[235] = SE_PALBANK(0) | 28;

  bg0_map[262] = SE_PALBANK(0) | 29;
  bg0_map[263] = SE_PALBANK(0) | 30;
  bg0_map[294] = SE_PALBANK(0) | 31;
  bg0_map[295] = SE_PALBANK(0) | 32;
  bg0_map[264] = SE_PALBANK(0) | 33;
  bg0_map[265] = SE_PALBANK(0) | 34;
  bg0_map[296] = SE_PALBANK(0) | 35;
  bg0_map[297] = SE_PALBANK(0) | 36;
  bg0_map[266] = SE_PALBANK(0) | 37;
  bg0_map[267] = SE_PALBANK(0) | 38;
  bg0_map[298] = SE_PALBANK(0) | 39;
  bg0_map[299] = SE_PALBANK(0) | 40;

  bg0_map[328] = SE_PALBANK(0) | 41;
  bg0_map[329] = SE_PALBANK(0) | 42;
  bg0_map[360] = SE_PALBANK(0) | 43;
  bg0_map[361] = SE_PALBANK(0) | 44;  

  // Bed R
  bg0_map[210] = SE_PALBANK(0) | 61;
  bg0_map[211] = SE_PALBANK(0) | 62;
  bg0_map[242] = SE_PALBANK(0) | 63;
  bg0_map[243] = SE_PALBANK(0) | 64;
  bg0_map[212] = SE_PALBANK(0) | 65;
  bg0_map[213] = SE_PALBANK(0) | 66;
  bg0_map[244] = SE_PALBANK(0) | 67;
  bg0_map[245] = SE_PALBANK(0) | 68;
  bg0_map[214] = SE_PALBANK(0) | 69;
  bg0_map[215] = SE_PALBANK(0) | 70;
  bg0_map[246] = SE_PALBANK(0) | 71;
  bg0_map[247] = SE_PALBANK(0) | 72;

  bg0_map[274] = SE_PALBANK(0) | 29;
  bg0_map[275] = SE_PALBANK(0) | 30;
  bg0_map[306] = SE_PALBANK(0) | 31;
  bg0_map[307] = SE_PALBANK(0) | 32;
  bg0_map[276] = SE_PALBANK(0) | 33;
  bg0_map[277] = SE_PALBANK(0) | 34;
  bg0_map[308] = SE_PALBANK(0) | 35;
  bg0_map[309] = SE_PALBANK(0) | 36;
  bg0_map[278] = SE_PALBANK(0) | 37;
  bg0_map[279] = SE_PALBANK(0) | 38;
  bg0_map[310] = SE_PALBANK(0) | 39;
  bg0_map[311] = SE_PALBANK(0) | 40;

  bg0_map[340] = SE_PALBANK(0) | 41;
  bg0_map[341] = SE_PALBANK(0) | 42;
  bg0_map[372] = SE_PALBANK(0) | 43;
  bg0_map[373] = SE_PALBANK(0) | 44;  

  // Cracks etc
  bg0_map[432] = SE_PALBANK(0) | 45;
  bg0_map[433] = SE_PALBANK(0) | 46;
  bg0_map[464] = SE_PALBANK(0) | 47;
  bg0_map[465] = SE_PALBANK(0) | 48;

  bg0_map[408] = SE_PALBANK(0) | 49;
  bg0_map[409] = SE_PALBANK(0) | 50;
  bg0_map[440] = SE_PALBANK(0) | 51;
  bg0_map[441] = SE_PALBANK(0) | 52;  

  bg0_map[470] = SE_PALBANK(0) | 57;
  bg0_map[471] = SE_PALBANK(0) | 58;
  bg0_map[502] = SE_PALBANK(0) | 59;
  bg0_map[273] = SE_PALBANK(0) | 60;  

  bg0_map[240] = SE_PALBANK(0) | 53;
  bg0_map[241] = SE_PALBANK(0) | 54;
  bg0_map[272] = SE_PALBANK(0) | 55;
  bg0_map[273] = SE_PALBANK(0) | 56;  

  bg0_map[110] = SE_PALBANK(0) | 5;
  bg0_map[111] = SE_PALBANK(0) | 6;
  bg0_map[142] = SE_PALBANK(0) | 7;
  bg0_map[143] = SE_PALBANK(0) | 8;

  return 0;
};

int game_loop(Mado* mado) {
  int time = 1;
  
  while(1) {
    vid_vsync();
    key_poll();
    move_mado(mado);
    
    oam_copy(oam_mem, obj_buffer, 1);	// only need to update one

    time++;
    
  }
};

int main() {

  // Tile mode, bgs 0, 1
  REG_DISPCNT= DCNT_MODE0 | DCNT_BG0 | DCNT_OBJ | DCNT_OBJ_1D;
  
  REG_BG0CNT= BG_CBB(CBB_0) | BG_SBB(SBB_0) | BG_REG_64x64 | 1;

  // Position of screen on map
  REG_BG0HOFS= 0;
  REG_BG0VOFS= 0;

  // Room Tiles
  memcpy16(&pal_bg_mem[0], roomPal, roomPalLen / sizeof(u16));
  memcpy32(&tile_mem[CBB_0][1], roomTiles, roomTilesLen / sizeof(u32));

  /* // Bed Tiles */
  /* memcpy16(&pal_bg_mem[16], bedPal, bedPalLen / sizeof(u16)); */
  /* memcpy32(&tile_mem[CBB_0][17], bedTiles, bedTilesLen / sizeof(u32)); */
  
  // Mado Sprite
  memcpy32(&tile_mem[4][0], madoTiles, madoTilesLen / sizeof(u32));
  memcpy16(pal_obj_mem, madoPal, madoPalLen / sizeof(u16));
  oam_init(obj_buffer, 128);

  Mado mado = {0};
  init_mado(&mado, 96, 100);

  draw_room();
  game_loop(&mado);
  
};
