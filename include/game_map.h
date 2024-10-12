#ifndef GAME_MAP_H
#define GAME_MAP_H

#include <stdlib.h>
#include <tonc.h>

enum __GAME_MAP {InvalidMap, MadoBedroom, MadoAttic};
typedef enum __GAME_MAP GameMap;

extern GameMap game_map;

int extract_tile_idx(SCR_ENTRY tile);
int mado_bedroom_tile_is_solid(int tile_idx);
int mado_attic_tile_is_solid(int tile_idx);
int tile_is_solid(int tile_idx);
GameMap mado_bedroom_tile_is_teleport(int tile_idx);
GameMap mado_attic_tile_is_teleport(int tile_idx);
GameMap tile_is_teleport(int tile_idx);

#endif
