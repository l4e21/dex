#include "dex.h"

int time = 1;
int a_held = 0;
int rain_offset;

int fade_out = 0;
int fade_in = 0;

int camera_map_default(Camera* camera, GameMap game_map) {
  switch (game_map) {
  case MadoBedroom:
    camera->behaviour = Static;
    camera->pos_x = 0;
    camera->pos_y = 0;
    break;
  case MadoAttic:
    camera->behaviour = Following;
    break;
  case InvalidMap:
    break;  
  }
  return 0;
};

int fade_out_iter() {
  u16 tileColor;
  u16 spriteColor;
  u16 tileR;
  u16 tileB;
  u16 tileG;
  u16 spriteR;
  u16 spriteB;
  u16 spriteG;
    
  for (int i = 0; i < 256; i++) {
    tileColor = pal_bg_mem[i];
    spriteColor = pal_obj_mem[i];
    tileR = (tileColor >> 10) & 0x1F;
    tileB = (tileColor >> 5) & 0x1F;
    tileG = tileColor & 0x1F;
    spriteR = (spriteColor >> 10) & 0x1F;
    spriteB = (spriteColor >> 5) & 0x1F;
    spriteG = spriteColor & 0x1F;
    
    pal_bg_mem[i] = RGB15(tileR > 0 ? tileR-1 : 0, tileB > 0 ? tileB-1 : 0, tileG > 0 ? tileB-1 : 0);
    pal_obj_mem[i] = RGB15(spriteR > 0 ? spriteR-1 : 0, spriteB > 0 ? spriteB-1 : 0, spriteG > 0 ? spriteB-1 : 0);
      };
  return 0;
};

int fade_in_iter() {
  u16 tileColor;
  u16 spriteColor;
  u16 tileR;
  u16 tileB;
  u16 tileG;
  u16 spriteR;
  u16 spriteB;
  u16 spriteG;
    
  for (int i = 0; i < 256; i++) {
    tileColor = pal_bg_mem[i];
    spriteColor = pal_obj_mem[i];
    tileR = (tileColor >> 10) & 0x1F;
    tileB = (tileColor >> 5) & 0x1F;
    tileG = tileColor & 0x1F;
    spriteR = (spriteColor >> 10) & 0x1F;
    spriteB = (spriteColor >> 5) & 0x1F;
    spriteG = spriteColor & 0x1F;
    
    pal_bg_mem[i] = RGB15(tileR > 30 ? 31 : tileR+1, tileB > 30 ? 31 : tileB+1, tileG > 30 ? 31 : tileG+1);
    pal_obj_mem[i] = RGB15(spriteR > 30 ? 31 : spriteR+1, spriteB > 30 ? 31 : spriteB+1, spriteG > 30 ? 30 : spriteG+1);
    
      };
  return 0;
};

int init_warp(Warp* warp) {
  warp->to_map = InvalidMap;
  warp->from_map = MadoBedroom;
  return 0;
};

int init_camera(Camera* camera, Mado* mado) {
  camera_map_default(camera, mado->game_map);

  switch (camera->behaviour) {
  case Static:
    /* camera->pos_x = camera->pos_x; */
    /* camera->pos_y = camera->pos_y; */
    break;
    
  case Following:
    camera->pos_x = mado->pos_x;
    camera->pos_y = mado->pos_y;
    break;

  };
  
  return 0;
};

int init_mado(Mado* mado, int x, int y, GameMap game_map) {
    // Mado Sprite
  memcpy32(&tile_mem[4][0], madoTiles, madoTilesLen / sizeof(u32));
  dma3_cpy(pal_obj_mem, madoPal, madoPalLen / sizeof(u16));

  // Emotes Sprite
  memcpy32(&tile_mem[4][160], emotesTiles, emotesTilesLen / sizeof(u32));
  dma3_cpy(&pal_obj_mem[16], emotesPal, emotesPalLen/ sizeof(u16));

  OBJ_ATTR *sprite = &obj_buffer[0];
  OBJ_ATTR *emote = &obj_buffer[1];
  obj_set_attr(sprite,
	       ATTR0_SQUARE,				// Square, regular sprite
	       ATTR1_SIZE_32, 				// 64x64p,
	       ATTR2_PALBANK(0) | ATTR2_PRIO(2) | 0);		// palbank 0, tile 0
  
  /* obj_set_pos(sprite, x, y); */

  obj_set_attr(emote,
	       ATTR0_SQUARE,				// Square, regular sprite
	       ATTR1_SIZE_16, 				// 64x64p,
	       ATTR2_PALBANK(1) | ATTR2_PRIO(2) | 160);		// palbank 0, tile 0
  
  /* obj_set_pos(emote, x, y-64); */
  
  mado->facing = Down;
  mado->movement = 0;
  mado->pos_x = x;
  mado->pos_y = y;
  mado->sprite = sprite;
  mado->emote = emote;
  mado->game_map = game_map;

  return 0;
};

int turn_mado(Mado* mado) {
  if (mado->facing == Up) {
    obj_set_attr(mado->sprite,
		 ATTR0_SQUARE,		
		 ATTR1_SIZE_32,      
		 ATTR2_PALBANK(0) | ATTR2_PRIO(2) | 16);	      
  }
  else if (mado->facing == Down) {
    obj_set_attr(mado->sprite,
		 ATTR0_SQUARE,		
		 ATTR1_SIZE_32,      
		 ATTR2_PALBANK(0) | ATTR2_PRIO(2) | 0);	      
  }
  else if (mado->facing == Right) {
    obj_set_attr(mado->sprite,
		 ATTR0_SQUARE,		
		 ATTR1_SIZE_32,      
		 ATTR2_PALBANK(0) | ATTR2_PRIO(2) | 48);	      
  }
  else if (mado->facing == Left) {
    obj_set_attr(mado->sprite,
		 ATTR0_SQUARE,		
		 ATTR1_SIZE_32,      
		 ATTR2_PALBANK(0) | ATTR2_PRIO(2) | 32);	      
  };
  return 0;
};


int turn_moving_mado(Mado* mado) {
  if (mado->facing == Up) {
    obj_set_attr(mado->sprite,
		 ATTR0_SQUARE,		
		 ATTR1_SIZE_32,      
		 ATTR2_PALBANK(0)  | ATTR2_PRIO(2) | (mado->movement < 4 ? 96 : 112));	      
  }
  else if (mado->facing == Down) {
    obj_set_attr(mado->sprite,
		 ATTR0_SQUARE,		
		 ATTR1_SIZE_32,      
		 ATTR2_PALBANK(0) | ATTR2_PRIO(2) | (mado->movement < 4 ? 64 : 80));	      
  }
  else if (mado->facing == Right) {
    obj_set_attr(mado->sprite,
		 ATTR0_SQUARE,		
		 ATTR1_SIZE_32,      
		 ATTR2_PALBANK(0) | ATTR2_PRIO(2) | 144);	      
  }
  else if (mado->facing == Left) {
    obj_set_attr(mado->sprite,
		 ATTR0_SQUARE,		
		 ATTR1_SIZE_32,      
		 ATTR2_PALBANK(0) | ATTR2_PRIO(2) | 128);	      
  };
  return 0;
};

int mado_try_move(Mado* mado) {
  // Mado's position is set to the top left of the sprite
  switch (mado->facing) {
  case Up:
    if (!tile_is_solid(mado->game_map, mado->pos_x/8, mado->pos_y/8)
	&&
	!tile_is_solid(mado->game_map, (mado->pos_x/8)+ 1, mado->pos_y/8)) {
      mado->movement = 8;
    }
    break;
  case Down:
    if(!tile_is_solid(mado->game_map, mado->pos_x/8, (mado->pos_y/8) + 3)
       &&
       !tile_is_solid(mado->game_map, (mado->pos_x/8) + 1, (mado->pos_y/8) + 3)) {
      mado->movement = 8;
    }
    break;
  case Left:
    if(!tile_is_solid(mado->game_map, (mado->pos_x/8) - 1, (mado->pos_y/8) + 2)
       &&
       !tile_is_solid(mado->game_map, (mado->pos_x/8) - 1, (mado->pos_y/8) + 2)) {
      mado->movement = 8;
    }
    break;
  case Right:
    if(!tile_is_solid(mado->game_map, (mado->pos_x/8) + 2, (mado->pos_y/8) + 2)
       &&
       !tile_is_solid(mado->game_map, (mado->pos_x/8) + 2, (mado->pos_y/8) + 2)) {
      mado->movement = 8;
    }
    break;
  case InvalidDirection:
    break;
  };
  return 0;
};

int init_move_mado(Mado* mado) {
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

    turn_mado(mado);

    return 0;
};


int move_mado(Mado* mado) {
  // Wraparound Logic
  switch (mado->facing) {
  case Up:
    if (mado->pos_y == 0) {
      mado->pos_y = 512;
    }
    else {
    mado->pos_y -= 1;
    };
    mado->movement -= 1;
    break;
  case Down:
    if (mado->pos_y == 512) {
      mado->pos_y = 0;
    }
    else {
    mado->pos_y += 1;
    };
    mado->movement -= 1;
    break;
  case Left:
    if (mado->pos_x == 0) {
      mado->pos_x = 512;
    }
    else {
    mado->pos_x -= 1;
    };
    mado->movement -= 1;
    break;
  case Right:
    if (mado->pos_x == 512) {
      mado->pos_x = 0;
    }
    else {
    mado->pos_x += 1;
    };
    mado->movement -= 1;
    break;

  case InvalidDirection:
    // This is an error situation so don't move?
    break;

  };
		  
  return 0;
};

int init_interact(Mado* mado) {
  if (key_is_down(KEY_A)) {
    mado->interactionType = Heart;
    mado->interacting = 24;
    obj_unhide(mado->emote, 0);
  };
  
  return 0;
};

int interact(Mado* mado) {
  switch (mado->interactionType) {
  case Heart:
    if (mado->interacting == 1) {
      obj_set_attr(mado->emote,
		   ATTR0_SQUARE,				// Square, regular sprite
		   ATTR1_SIZE_16, 				// 64x64p,
		   ATTR2_PALBANK(1) | ATTR2_PRIO(2) | 160);		// palbank 0, tile 0
      obj_hide(mado->emote);
    }
    else {
      obj_set_attr(mado->emote,
		   ATTR0_SQUARE,				// Square, regular sprite
		   ATTR1_SIZE_16, 				// 64x64p,
		   ATTR2_PALBANK(1) | ATTR2_PRIO(2) | (160 + (mado->interacting > 16 ? 0 : (mado->interacting > 8 ? 4 : 8))));		// palbank 0, tile 0
      };
    break;
  }
  mado->interacting -=1;  
  return 0;
};


int init_teleport(Warp* warp) {
  fade_out = 60;
  fade_in = 60;
  return 0;
};

int teleport(Mado* mado, Warp* warp, Camera* camera) {
  if(fade_out) {
    fade_out_iter();
    fade_out--;
  }

  else if (fade_in) {
    fade_in_iter();
    fade_in--;
  }

  else {
    // Re-initialise a lot of stuff
    warp->from_map = mado->game_map;
    init_game_map(warp->to_map);
    init_mado(mado, warp->pos_x, warp->pos_y, warp->to_map);
    init_camera(camera, mado);
    warp->to_map = InvalidMap;
  };
  return 0;
};


int mado_act(Mado* mado, Warp* warp, Camera* camera) {
    if (warp->to_map != InvalidMap) {
      // Mado is teleporting
      teleport(mado, warp, camera);
    }

    else if (mado->movement) {
      // Mado is moving
      turn_moving_mado(mado);
      move_mado(mado);
    }

    else if (mado->interacting) {
      // Mado is interacting
      interact(mado);
    }

    else {
    // Init action with priority on teleport then movement then interaction
      tile_is_teleport(mado->game_map, warp, extract_tile_idx(bg0_map[(mado->pos_x/8) + 32*((mado->pos_y/8) + 2)]));
      
      if (warp->to_map == InvalidMap) {
	init_interact(mado);

	if (!mado->movement) {
	  init_move_mado(mado);
	};
      }

      else {
	init_teleport(warp);
      };

    };
    return 0;
    };


int update_mado_bedroom(Mado* mado) {
  if (time % 2 == 0) {
    rain_offset -= 4;
    if (rain_offset < 0) {
      rain_offset = 69;
    };
    // Less memory efficient, less granular, more VRAM bandwidth, less cpu usage, less complexity
    // For other foregrounds, it might be better to go by index-based tile swapping
    memcpy32(&tile_mem[CBB_1][rain_offset], rainTiles, rainTilesLen / sizeof(u32));
  };

  return 0;
};

int update_mado_attic(Mado* mado) {    
  return 0;
};

int update_game_map(Mado* mado) {
  switch (mado->game_map) {
  case MadoBedroom:
    update_mado_bedroom(mado);
    break;
  case MadoAttic:
    update_mado_attic(mado);
    break;
  case InvalidMap:
    break;
  };
  
  return 0;
};

int update_camera(Camera* camera, Mado* mado) {
  switch (camera->behaviour) {
  case Static:
    break;
  case Following:
    camera->pos_x = mado->pos_x - (SCREEN_WIDTH/2);
    camera->pos_y = mado->pos_y - (SCREEN_HEIGHT/2); 
 };

  return 0;
}

int game_loop(Mado* mado, Warp* warp, Camera* camera) {
  /* char textBuffer[20]; */
  /* sprintf(textBuffer, "HI WORLD"); */
  while(1) {
    vid_vsync();
    key_poll();
    /* tte_write("#{P:0,0}");        // Goto (72, 64). */
    /* tte_write(textBuffer);      // Print "Hello world!" */

    mado_act(mado, warp, camera);
    update_camera(camera, mado);

    REG_BG0HOFS= camera->pos_x;
    REG_BG0VOFS= camera->pos_y;
    REG_BG1HOFS= camera->pos_x;
    REG_BG1VOFS= camera->pos_y;
    /* REG_BG2HOFS= camera->pos_x; */
    /* REG_BG2VOFS= camera->pos_y; */
    
    switch (camera->behaviour) {
    case Static:
      obj_set_pos(mado->sprite, mado->pos_x, mado->pos_y);
      obj_set_pos(mado->emote, mado->pos_x, mado->pos_y-16);
      break;
    case Following:
      obj_set_pos(mado->sprite, SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
      obj_set_pos(mado->emote, SCREEN_WIDTH/2, (SCREEN_HEIGHT/2)-16);
      break;
    };

    oam_copy(oam_mem, obj_buffer, 20);	// only need to update one

    update_game_map(mado);
    
    time++;
    
  }
  return 0;
};

int main() {

  // Text Menus will be added via windowing functions
  
  REG_DISPCNT = DCNT_MODE0 | DCNT_BG2 | DCNT_BG0 | DCNT_BG3 | DCNT_BG1 | DCNT_OBJ | DCNT_OBJ_1D;

  // Main layer
  REG_BG0CNT = BG_CBB(CBB_0) | BG_SBB(SBB_0) | BG_REG_64x64 | BG_PRIO(2);

  // Foreground
  REG_BG1CNT = BG_CBB(CBB_1) | BG_SBB(SBB_1) | BG_REG_64x64 | BG_PRIO(0);

  // Background
  REG_BG2CNT = BG_CBB(CBB_2) | BG_SBB(SBB_2) | BG_REG_64x64 | BG_PRIO(3);

  // Decoration layer
  REG_BG3CNT = BG_CBB(CBB_3) | BG_SBB(SBB_3) | BG_REG_64x64 | BG_PRIO(1);
  
  oam_init(obj_buffer, 128);

  Mado mado = {0};
  Warp warp = {0};
  Camera camera = {0};
  
  init_mado(&mado, 96, 100, MadoBedroom);
  init_camera(&camera, &mado);
  init_warp(&warp);
  init_game_map(mado.game_map);

  game_loop(&mado, &warp, &camera);
  
};
