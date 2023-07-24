#ifndef _CONFIG_H
#define _CONFIG_H

#define SCREEN_W              800
#define SCREEN_H              600

#define SHEET_WIDTH           32
#define SHEET_HEIGHT          32
#define MAX_TEXTURE_ID        ((SHEET_WIDTH * SHEET_HEIGHT) - 1)

#define TILE_RENDER_SIZE      32
#define TILE_SIZE             16

#define PLAYER_DEFAULT_SPRITE 97
#define PLAYER_FACING_UP      65
#define PLAYER_FACING_DOWN    64
#define PLAYER_FACING_LEFT    96
#define PLAYER_FACING_RIGHT   97

#define PLAYER_NUM_INV_SLOTS  5
#define PLAYER_INV_IDX_START  7
#define PLAYER_NUM_TOTAL_INV  PLAYER_INV_IDX_START + PLAYER_NUM_INV_SLOTS

#define CHEST_INV_SIZE        24

#define ENTITY_DEFAULT_SPRITE 451
#define INVALID_ITEM_SPRITE   33

#endif
