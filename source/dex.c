#include "dex.h"

int time = 1;
GameMap game_map = MadoBedroom;
int a_held = 0; 
int rain_offset = 69;

int init_mado(Mado* mado, int x, int y) {
    // Mado Sprite
  memcpy32(&tile_mem[4][0], madoTiles, madoTilesLen / sizeof(u32));
  dma3_cpy(pal_obj_mem, madoPal, madoPalLen / sizeof(u16));

  // Emotes Sprite
  memcpy32(&tile_mem[4][40], emotesTiles, emotesTilesLen / sizeof(u32));
  dma3_cpy(&pal_obj_mem[16], emotesPal, emotesPalLen/ sizeof(u16));

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
  
  mado->facing = InvalidMap;
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
  case InvalidDirection:
    break;
  };
  return 0;
};


int move_mado(Mado* mado) {
  // Check if mado can move, if so set movement
  if (!mado->movement && !mado->interacting) {
    // Check if on teleport tile first
    GameMap teleportToMap = tile_is_teleport(extract_tile_idx(bg0_map[(mado->posX/8) + 32*((mado->posY/8) + 2)]));

    if (teleportToMap != InvalidMap) {
      // Initialise map change
      /* game_map = teleportToMap; */
    }
    
    // Try moving, face the direction anyway
    else if (key_is_down(KEY_UP)) {
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

    case InvalidDirection:
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

int game_loop(Mado* mado) {
  
  while(1) {
    vid_vsync();
    key_poll();
    move_mado(mado);
    interact(mado);
    
    oam_copy(oam_mem, obj_buffer, 16);	// only need to update one

    update_game_map();
    
    time++;
    
  }
  return 0;
};

int main() {

  // Tile mode, bgs 0 (tiles), 1 (Foregrounds), later 2 (backgrounds)
  // For sprites, 
  REG_DISPCNT= DCNT_MODE0 | DCNT_BG0 | DCNT_BG1 | DCNT_OBJ | DCNT_OBJ_1D;
  
  REG_BG0CNT= BG_CBB(CBB_0) | BG_SBB(SBB_0) | BG_REG_64x64 | BG_PRIO(1);

  REG_BG1CNT= BG_CBB(CBB_1) | BG_SBB(SBB_1) | BG_REG_64x64 | BG_PRIO(0);

  
  // Position of screen on map
  REG_BG0HOFS= 0;
  REG_BG0VOFS= 0;

  REG_BG1HOFS= 0;
  REG_BG1VOFS= 0;

  oam_init(obj_buffer, 128);

  Mado mado = {0};
  
  init_mado(&mado, 96, 100);
  init_game_map();
  game_loop(&mado);
  
};
