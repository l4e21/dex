#include "game_map.h"

int extract_tile_idx(SCR_ENTRY tile) {
  return tile & SE_ID_MASK;
}

int draw_16_by_16(int idx, int pal, int tile_idx) {
  bg0_map[idx] = SE_PALBANK(pal) | tile_idx;
  bg0_map[idx+1] = SE_PALBANK(pal) | (tile_idx+1);
  bg0_map[idx+32] = SE_PALBANK(pal) | (tile_idx+2);
  bg0_map[idx+33] = SE_PALBANK(pal) | (tile_idx+3);
  return 0;
};

int mado_bedroom_tile_is_solid(int tile_idx) {
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
};

int mado_attic_tile_is_solid(int tile_idx) {
  return 0;  
};

int tile_is_solid(int tile_idx) {
  switch (game_map) {
  case MadoBedroom:
    return mado_bedroom_tile_is_solid(tile_idx);
  case MadoAttic:
    return mado_attic_tile_is_solid(tile_idx);
  case InvalidMap:
    return 0;
  };
  return 0;
};

GameMap mado_bedroom_tile_is_teleport(int tile_idx) {
  if (tile_idx == 49) {
    return MadoBedroom;
  }  
  if (tile_idx == 45) {
    return MadoBedroom;
  }  
  if (tile_idx == 47) {
    return MadoBedroom;
  }  
  return InvalidMap;
};

GameMap mado_attic_tile_is_teleport(int tile_idx) {
  return InvalidMap;
};

GameMap tile_is_teleport(int tile_idx) {
  switch (game_map) {
  case MadoBedroom:
    return mado_bedroom_tile_is_teleport(tile_idx);
  case MadoAttic:
    return mado_attic_tile_is_teleport(tile_idx);
  case InvalidMap:
    return InvalidMap;
  };

  return InvalidMap;
};

int draw_mado_bedroom() {
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

int init_mado_bedroom() {

  // Room Tiles
  dma3_cpy(&pal_bg_mem[0], roomPal, roomPalLen / sizeof(u16));
  memcpy32(&tile_mem[CBB_0][1], roomTiles, roomTilesLen / sizeof(u32));

  // Bed Tiles
  dma3_cpy(&pal_bg_mem[16], bedPal, bedPalLen / sizeof(u16));
  memcpy32(&tile_mem[CBB_0][61], bedTiles, bedTilesLen / sizeof(u32));

  // Rain Tiles
  dma3_cpy(&pal_bg_mem[32], rainPal, rainPalLen / sizeof(u16));
  memcpy32(&tile_mem[CBB_1][1], rainTiles, rainTilesLen / sizeof(u32));
  
  draw_mado_bedroom();

  return 0;
};

int init_mado_attic() {
  return 0;
};

int init_game_map() {
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

int update_mado_bedroom() {
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

int update_game_map() {
  switch (game_map) {
  case MadoBedroom:
    update_mado_bedroom();
    break;
  case MadoAttic:
    break;
  case InvalidMap:
    break;
  };
  
  return 0;
};
