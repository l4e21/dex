#include "game_map.h"
SCR_ENTRY *bg_map;
SCR_ENTRY *bg0_map= se_mem[SBB_0];
SCR_ENTRY *bg1_map= se_mem[SBB_1];
SCR_ENTRY *bg2_map= se_mem[SBB_2];
SCR_ENTRY *bg3_map= se_mem[SBB_3];


int extract_tile_idx(SCR_ENTRY tile) {
  return tile & SE_ID_MASK;
}

// Draw 16 by 16 tile on a 32 by 32 quadrant
int draw_16_by_16(int idx, int pal, int tile_idx, int bg) {
  switch (bg) {
  case 0:
    bg_map = bg0_map;
    break;
  case 1:
    bg_map = bg1_map;
    break;
  case 2:
    bg_map = bg2_map;
    break;
  case 3:
    bg_map = bg3_map;
    break;
  };
  
  bg_map[idx] = SE_PALBANK(pal) | tile_idx;
  bg_map[idx+1] = SE_PALBANK(pal) | (tile_idx+1);
  bg_map[idx+32] = SE_PALBANK(pal) | (tile_idx+2);
  bg_map[idx+33] = SE_PALBANK(pal) | (tile_idx+3);
  return 0;
};

int tile_is_solid(GameMap game_map, int x, int y) {
  int quadrant;
  
  if (x > 32 && y > 32) {
    quadrant = 3;
  }
  else if (x > 32) {
    quadrant = 1;
  } 
  else if (y > 32) {
    quadrant = 2;
  }
  else {
    quadrant = 0;
  };
  
  int tile_idx_3 = extract_tile_idx(bg3_map[(x % 32) + 32*(y % 32) + 32*32*quadrant]);
  int tile_idx_0 = extract_tile_idx(bg0_map[(x % 32) + 32*(y % 32) + 32*32*quadrant]);
  
  for (int i=0;i<50;i++) {
    switch (game_map) {
    case MadoBedroom:
      if ((tile_idx_0 && tile_idx_0 == mado_bedroom_solid_tiles[i]) || (tile_idx_3 && tile_idx_3 == mado_bedroom_3_solid_tiles[i])) {
	return 1;
      };
      break;
    case MadoAttic:
      if ((tile_idx_0 && tile_idx_0 == mado_attic_solid_tiles[i])) {
	return 1;
      };
      break;
    case InvalidMap:
      return 0;
    };
  };

  return 0;
}

//
// TELEPORT
//


int mado_bedroom_tile_is_teleport(Warp* warp, int tile_idx) {
  if (tile_idx == 49) {
    warp->to_map = MadoAttic;
    warp->pos_x = 96;
    warp->pos_y = 100;
  }  
  if (tile_idx == 45) {
    warp->to_map = MadoAttic;
    warp->pos_x = 96;
    warp->pos_y = 100;
  }  
  if (tile_idx == 47) {
    warp->to_map = MadoAttic;
    warp->pos_x = 96;
    warp->pos_y = 100;
  }  
  return 0;
};

int mado_attic_tile_is_teleport(Warp* warp, int tile_idx) {
  warp->to_map = InvalidMap;
  return 0;
};

int tile_is_teleport(GameMap game_map, Warp* warp, int tile_idx) {
  warp->to_map = InvalidMap;
  switch (game_map) {
  case MadoBedroom:
    mado_bedroom_tile_is_teleport(warp, tile_idx);
    break;
  case MadoAttic:
    mado_attic_tile_is_teleport(warp, tile_idx);
    break;
  case InvalidMap:
    break;
  };

  return 0;
};

//
// DRAWING
// For some reason, it does it in quadrants for 64 by 64 backgrounds

int draw_mado_bedroom() {
  // Primary
  for (int j=0;j<16;j++) {
    for (int i=0;i<16;i++) {
      draw_16_by_16(i*2 + j*32*2, 0, mado_bedroom_tile_map[j][i], 0);
      /*  */
      // Foreground
      if (i > 0 && i < 13 && j < 9) {
	draw_16_by_16(i*2+1 + j*32*2, 2, 69, 1);
      };
    };
  };
    
  // Bed L
  draw_16_by_16(198, 1, 5, 3);
  draw_16_by_16(200, 1, 9, 3);
  draw_16_by_16(202, 1, 13, 3);
  
  draw_16_by_16(262, 1, 17, 3);
  draw_16_by_16(264, 1, 21, 3);
  draw_16_by_16(266, 1, 25, 3);

  draw_16_by_16(328, 1, 41, 3);
  
  /* // Bed R */
  draw_16_by_16(210, 1, 29, 3);
  draw_16_by_16(212, 1, 33, 3);
  draw_16_by_16(214, 1, 37, 3);
  
  draw_16_by_16(274, 1, 45, 3);
  draw_16_by_16(276, 1, 49, 3);
  draw_16_by_16(278, 1, 25, 3);

  draw_16_by_16(340, 1, 41, 3);
  
  return 0;
};

int draw_mado_attic() {  
  for (int j=0; j<16;j++) {
    for (int i=0; i<16;i++) {
      draw_16_by_16(i*2 + 32*j*2, 0, mado_attic_tile_map[j][i], 0);
      draw_16_by_16(i*2 + 32*j*2 + 32*32, 0, mado_attic_tile_map[j][i+16], 0);
      draw_16_by_16(i*2 + 32*j*2 + 32*32*2, 0, mado_attic_tile_map[j+16][i], 0);
      draw_16_by_16(i*2 + 32*j*2 + 32*32*3, 0, mado_attic_tile_map[j+16][i+16], 0);
    };
  };
  return 0;
};


//
// INITIALISATION
//

int clear_all() {
  CBB_CLEAR(CBB_0);
  CBB_CLEAR(CBB_1);
  CBB_CLEAR(CBB_2);
  CBB_CLEAR(CBB_3);
  SBB_CLEAR(SBB_0);
  SBB_CLEAR(SBB_1);
  SBB_CLEAR(SBB_2);
  SBB_CLEAR(SBB_3);

  return 0;
};


int init_mado_bedroom() {
  clear_all();
  
  rain_offset = 69;

  // Room Tiles, main layer
  dma3_cpy(pal_bg_mem, mado_bedroomPal, mado_bedroomPalLen / sizeof(u16));
  memcpy32(&tile_mem[CBB_0][1], mado_bedroomTiles, mado_bedroomTilesLen / sizeof(u32)); /*  */

  // Bed Tiles, decoration layer
  dma3_cpy(&pal_bg_mem[16], bedPal, bedPalLen / sizeof(u16));
  memcpy32(&tile_mem[CBB_3][1], bedTiles, bedTilesLen / sizeof(u32));

  // Rain Tiles, foreground layer
  dma3_cpy(&pal_bg_mem[32], rainPal, rainPalLen / sizeof(u16));
  memcpy32(&tile_mem[CBB_1][1], rainTiles, rainTilesLen / sizeof(u32));
  
  draw_mado_bedroom();

  return 0;
};

int init_mado_attic() {
  clear_all();

  // Attic Tiles
  dma3_cpy(&pal_bg_mem[0], mado_atticPal, mado_atticPalLen / sizeof(u16));
  memcpy32(&tile_mem[CBB_0][1], mado_atticTiles, mado_atticTilesLen / sizeof(u32));
  
  draw_mado_attic();
  
  return 0;
};

int init_game_map(GameMap game_map) {
  switch (game_map) {
  case MadoBedroom:
    init_mado_bedroom();
    break;
  case MadoAttic:
    init_mado_attic();
    break;
  case InvalidMap:
    break;
  };
  
  return 0;
};
