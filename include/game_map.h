#ifndef GAME_MAP_H
#define GAME_MAP_H

#include <stdlib.h>
#include <tonc.h>

#include "mado.h"
#include "room.h"
#include "bed.h"
#include "rain.h"
#include "emotes.h"

#define CBB_0  0
#define SBB_0 28

#define CBB_1  1
#define SBB_1 31

enum __GAME_MAP {InvalidMap, MadoBedroom, MadoAttic};
typedef enum __GAME_MAP GameMap;

extern GameMap game_map;
extern SCR_ENTRY *bg0_map;
extern SCR_ENTRY *bg1_map;
extern int time;
extern int rain_offset;

int extract_tile_idx(SCR_ENTRY tile);
int draw_16_by_16(int idx, int pal, int tile_idx);

int mado_bedroom_tile_is_solid(int tile_idx);
int mado_attic_tile_is_solid(int tile_idx);
int tile_is_solid(int tile_idx);

GameMap mado_bedroom_tile_is_teleport(int tile_idx);
GameMap mado_attic_tile_is_teleport(int tile_idx);
GameMap tile_is_teleport(int tile_idx);

int draw_mado_bedroom();

int init_mado_bedroom();
int init_game_map();

int update_mado_bedroom();
int update_game_map();

#endif
