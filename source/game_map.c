#include "game_map.h"

int extract_tile_idx(SCR_ENTRY tile) {
  return tile & SE_ID_MASK;
}

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
    return MadoAttic;
  }  
  if (tile_idx == 45) {
    return MadoAttic;
  }  
  if (tile_idx == 47) {
    return MadoAttic;
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
