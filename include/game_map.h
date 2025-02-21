#ifndef GAME_MAP_H
#define GAME_MAP_H

#include <stdlib.h>
#include <tonc.h>

// From GRIT
#include "mado.h"
#include "mado_bedroom.h"
#include "mado_attic.h"
#include "bed.h"
#include "rain.h"
#include "emotes.h"

// From include
#include "tile_data.h"

#define CBB_0  0
#define SBB_0 28

#define CBB_1  1
#define SBB_1 29

#define CBB_2  2
#define SBB_2 30

#define CBB_3  3
#define SBB_3 31

enum __GAME_MAP {InvalidMap, MadoBedroom, MadoAttic};
typedef enum __GAME_MAP GameMap;

enum __DIRECTION {Left, Down, Up, Right, InvalidDirection};
typedef enum __DIRECTION Direction;

struct __WARP {
  GameMap to_map;
  int pos_x;
  int pos_y;
  Direction facing;
  GameMap from_map;
  GameMap current_map;
};

typedef struct __WARP Warp;

extern SCR_ENTRY *bg0_map;
extern SCR_ENTRY *bg1_map;
extern SCR_ENTRY *bg2_map;
extern SCR_ENTRY *bg3_map;

extern int time;
extern int rain_offset;

int extract_tile_idx(SCR_ENTRY tile);
int draw_16_by_16(int idx, int pal, int tile_idx, int bg);

int tile_is_solid(GameMap game_map, int x, int y);

int mado_bedroom_tile_is_teleport(Warp* warp, int tile_idx);
int mado_attic_tile_is_teleport(Warp* warp, int tile_idx);
int tile_is_teleport(GameMap game_map, Warp* warp, int tile_idx);

int draw_mado_bedroom();
int draw_mado_attic();

int init_mado_bedroom();
int init_mado_attic();
int init_game_map(GameMap game_map);

#endif
