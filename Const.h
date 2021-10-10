#define FRAME_COUNT_SEC 0x3C

#define LOGO_TILE_COUNT 31
#define LOGO_X_OFFSET 72
#define LOGO_Y_OFFSET 25

#define RIGHT_EDGE 0xFA
#define BOTTOM_EDGE 0xE6
#define PLAYER_WIDTH 0x20
#define DIR_LEFT 0x00
#define DIR_RIGHT 0x01

#define GRASS_WIDTH 0x20
#define GRASS_BORDER 0x17
#define GRASS_POINT_COUNT 0x0A
#define GRASS_MAX_HEIGHT 0x0C
#define GRASS_CRITICAL_HEIGHT 0x08
#define GRASS_SBLOCK 0x23

#define RAIN_COUNT 0x08
#define DEFAULT_RAIN_DELAY 0x06
#define STARS_COUNT 0x20
#define GAME_BG_COLOR 0x0C
#define GAME_MAX_POINTS 0x63

#define GAME_PAUSED 0x01
#define GAME_UNPAUSED 0x00
#define GAME_ST_MENU 0x00
#define GAME_ST_LEVEL_PLAYING 0x01
#define GAME_ST_LEVEL_WIN 0x02
#define GAME_WIN_TIME 0x80
#define GAME_ST_LEVEL_END_WIN 0x03
#define GAME_ST_LEVEL_LOSE 0x04
#define GAME_ST_LEVEL_END_LOSE 0x05
#define GAME_ST_OPTIONS 0x04
#define GAME_DF_EASY 0x00
#define GAME_DF_MEDIUM 0x01
#define GAME_DF_HARD 0x02
#define GAME_DF_EXTREME 0x03
#define GAME_MAX_LEVEL 0x0C

#define CUT_ST_IDLE 0x00
#define CUT_ST_C1 0x01
#define CUT_ST_C2 0x02
#define CUT_ST_C3 0x03
#define CUT_ST_RECOVER 0x05
#define CUT_ST_RECOVER_TIME 0x30

#define PPU_PALLETE 0x23C0

#define SCREEN_TILES_X 0x20
#define SCREEN_TILES_Y 0x1E
#define Y_EDGE 0x1C

#define ABILITIES_COUNT 0x05						
#define ABILITY_EX_LEVEL 0x03
#define ABILITY_MAX_LEVEL 0x05
#define ABILITY_CUT_SPEED 0x00
#define ABILITY_CUT_POWER 0x01
#define ABILITY_CUT_SIZE 0x02
#define ABILITY_MOVE_SPEED 0x03
#define ABILITY_SPECIAL 0x04

#define SP_COUNT 0x03
#define SP_MAX_DELAY 0x10
#define SP_IDLE 0x00
#define SP_SSCYTHE_SIZE 0x10
#define SP_SSCYTHE 0x00
#define SP_SSCYTHE_SPEED 0x03
#define SP_SSCYTHE_SPEED_EX 0x05
#define SP_SSCYTHE_DMG 0x01
#define SP_SSCYTHE_DMG_EX 0x02
#define SP_SSCYTHE_ALEVEL 0x01

#define SP_DASH 0x01
#define SP_DASH_ALEVEL 0x02
#define SP_DASH_LEFT 0x01
#define SP_DASH_RIGHT 0x02
#define SP_DASH_LENGTH 0x0C
#define SP_DASH_JUMP 0x05
#define SP_DASH_SPEED_LOW 0x02
#define SP_DASH_SPEED_MED 0x03
#define SP_DASH_SPEED_MAX 0x04


#define SP_SLASH 0x02
#define SP_SLASH_ALEVEL 0x04
#define SP_SLASH_TRIPLE_DOWN 0x03
#define SP_SLASH_ACTIVE 0x01
#define SP_SLASH_COUNT 0x03
#define SP_SLASH_COUNT_EX 0x05
#define SP_SLASH_FAST_DELAY 0x03
#define SP_SLASH_FAST_DELAY_EX 0x01

#define PLAYER_DEF_X 0x20
#define PLAYER_DEF_Y 0x8C

#define BIRDS_COUNT 0x04
#define BIRDS_WIDTH 0x10
#define BIRDS_HEIGHT 0x08
#define BIRDS_TAG_HIT 0x05
#define BIRDS_TAG_DIE 0x19

#define MOON_X 0x18
#define MOON_Y 0x03

#define CHR_ZERO 0x30
#define CHR_DOT 0x3B
#define CHR_CELL_TOP 0x22
#define CHR_FCELL_TOP 0x23
#define CHR_CELL_BOTTOM 0x24
#define CHR_FCELL_BOTTOM 0x25
#define CHR_BORDER_CORNER 0x26
#define CHR_BORDER_HORZ 0x27
#define CHR_BORDER_VERT 0x2A
#define CHR_LIVE 0xA7
#define CHR_TIME 0xA8
#define CHR_GRASS_COL 0xA9
#define CHR_DANGER_LOW 0xAA
#define CHR_DANGER_HIGH 0xAB
#define CHR_CROSS 0x2F
#define CHR_SP_BLANK 0x0F
#define CHR_BG_BLANK 0x7F
#define CHR_SC_ANIM_DELAY 0x04

#define GCS_NORMAL 0x00
#define GCS_CHECK 0x01

#define SND_MENU_MOVE 0x00
#define SND_MENU_START 0x01
#define SND_MENU_UPGRADE 0x02
#define SND_MENU_ERROR 0x03
#define SND_GAME_CUT 0x04
#define SND_MENU_HIT 0x05
#define SND_MENU_FLASH 0x06
#define SND_GAME_LOSE 0x07
#define SND_GAME_WIN 0x08
#define SND_GAME_LIGHTNING 0x09
#define SND_GAME_WIND 0x0A
#define SND_BIRD_DEATH 0x0B

#define MSC_MENU 0x00
#define MSC_GAME_WIN 0x01

#define EG_PRINT_DELAY 0x05

#define ability_ex (player.abilities.elevel >= ABILITY_EX_LEVEL)
#define ability_max (player.abilities.elevel == ABILITY_MAX_LEVEL)
#define total_height adr
#define single_sprout tmp
#define corr_power marker
#define menu_items tmp
#define menu_position index

/*
76543210
||||||||
||||||++- Palette (4 to 7) of sprite
|||+++--- Unimplemented
||+------ Priority (0: in front of background; 1: behind background)
|+------- Flip sprite horizontally
+-------- Flip sprite vertically
*/

const unsigned char title_palette_bg[] = {
0x0F, 0x0F, 0x21, 0x20, 
0x0F, 0x01, 0x21, 0x20,
0x0F, 0x0F, 0x08, 0x0A,
0x0F, 0x00, 0x09, 0x0A
}; 

const unsigned char title_palette_sp[] = {
0x0F, 0x13, 0x23, 0x03, 
0x0F, 0x13, 0x23, 0x03, 
0x0F, 0x12, 0x22, 0x03,
0x0F, 0x00, 0x00, 0x00
}; 

const unsigned char game_palette_bg[] = {
0x0C, 0x0E, 0x06, 0x1B, 
0x0C, 0x01, 0x21, 0x20,
0x0C, 0x0F, 0x08, 0x0A,
0x0F, 0x00, 0x09, 0x0A
}; 

const unsigned char game_palette_sp[] = {
0x0C, 0x00, 0x0F, 0x30, 
0x0C, 0x0F, 0x0F, 0x10, 
0x0C, 0x01, 0x21, 0x31,
0x0C, 0x00, 0x00, 0x00
}; 

const unsigned char upgrade_palette_bg[] = {
0x0C, 0x0F, 0x10, 0x30, 
0x0C, 0x01, 0x21, 0x20,
0x0C, 0x0F, 0x08, 0x0A,
0x0C, 0x00, 0x09, 0x0A
}; 

const unsigned char upgrade_palette_sp[] = {
0x0C, 0x00, 0x0F, 0x30, 
0x0C, 0x0F, 0x0F, 0x10, 
0x0C, 0x01, 0x21, 0x31,
0x0C, 0x00, 0x00, 0x00
}; 

const unsigned char upgrade_cost[] = {
	3, 5, 7, 8, 9,
	3, 3, 5, 5, 9,
	3, 4, 5, 7, 9,
	1, 2, 3, 4, 9,
	1, 3, 5, 7, 9
};

const unsigned char BlueSpr[]={
	  0,  0,0x50,1,
	  0,  8,0x60,1,
	  8,  0,0x50,1|OAM_FLIP_H,
	  8,  8,0x60,1|OAM_FLIP_H,
	128
};

const unsigned char CURSOR[] = {
	  0,  0,  0x14, 2,
	  0,  13, 0x14, 2|OAM_FLIP_V,
	  12, 0,  0x14, 2|OAM_FLIP_H,
	  12, 13, 0x14, 2|OAM_FLIP_H|OAM_FLIP_V,
	128
};

const unsigned char LOGO[] = {
 32,   8, 0x08, 0b00000001, 
 64,   8, 0x08, 0b01000001, 
 64,  24, 0x0D, 0b01000001, 
 24,  24, 0x0A, 0b00000001, 
 32,  32, 0x09, 0b00000001, 
 64,  48, 0x07, 0b00000001, 
 88,  24, 0x0D, 0b00000001, 
 32,  48, 0x06, 0b11000001, 
 72,  32, 0x06, 0b01000001, 
 80,  32, 0x06, 0b00000001, 
 56,   8, 0x0A, 0b00000001, 
 16,  32, 0x09, 0b00000001, 
 16,  24, 0x06, 0b00000001, 
 72,  40, 0x06, 0b10000001, 
 48,  24, 0x09, 0b00000001, 
 88,  40, 0x09, 0b00000001, 
 64,  40, 0x09, 0b00000001, 
 48,   8, 0x0B, 0b00000001, 
 88,  48, 0x07, 0b00000001, 
 64,  32, 0x0C, 0b00000001, 
 48,  40, 0x07, 0b00000001, 
 32,  24, 0x06, 0b01000001, 
 16,  48, 0x06, 0b10000001, 
 16,  40, 0x09, 0b00000001, 
 80,  40, 0x06, 0b11000001, 
 24,  48, 0x0A, 0b00000001, 
 32,  40, 0x09, 0b00000001, 
 88,  32, 0x0C, 0b01000001, 
 48,  16, 0x09, 0b00000001, 
 40,   8, 0x0A, 0b00000001, 
 48,  32, 0x09, 0b00000001
};

const unsigned char STATUS_BAR[] = {
	0, CHR_LIVE,      CHR_CROSS, 0, 0, 0, 0, 0, 
	0, CHR_GRASS_COL, CHR_CROSS, 0, 0, 0, 0, 0, 0, 0, 
	0, CHR_TIME, 0, 0x30, 0x30, 0, 0, 0,  
	0, 0, CHR_DANGER_LOW,  0,   0, CHR_DANGER_HIGH
};


const unsigned char SCYTHE_STATE[8][17] = {
	{0, 0, 0x70, 0,
	 8, 0, 0x71, 0,
	 0, 8, 0x72, 0,
	 8, 8, 0x73, 0,
	 128},
	{0, 0, 0x74, 0,
	 8, 0, 0x75, 0,
	 0, 8, CHR_SP_BLANK, 0,
	 8, 8, 0x76, 0,
	 128},
	{0, 0, 0x77, 0,
	 8, 0, 0x78, 0,
	 0, 8, 0x79, 0,
	 8, 8, 0x7A, 0,
	 128},
	{0, 0, CHR_BG_BLANK, 0,
	 8, 0, 0x7B, 0,
	 0, 8, 0x7C, 0,
	 8, 8, 0x7D, 0,
	 128},
	{0, 0, 0x73, 0|OAM_FLIP_H|OAM_FLIP_V,
	 8, 0, 0x72, 0|OAM_FLIP_H|OAM_FLIP_V,
	 0, 8, 0x71, 0|OAM_FLIP_H|OAM_FLIP_V,
	 8, 8, 0x70, 0|OAM_FLIP_H|OAM_FLIP_V,
	 128},
    {0, 0, 0x76, 0|OAM_FLIP_H|OAM_FLIP_V,
	 8, 0, CHR_SP_BLANK, 0|OAM_FLIP_H|OAM_FLIP_V,
	 0, 8, 0x75, 0|OAM_FLIP_H|OAM_FLIP_V,
	 8, 8, 0x74, 0|OAM_FLIP_H|OAM_FLIP_V,
	 128},
	{0, 0, 0x7A, 0|OAM_FLIP_H|OAM_FLIP_V,
	 8, 0, 0x79, 0|OAM_FLIP_H|OAM_FLIP_V,
	 0, 8, 0x78, 0|OAM_FLIP_H|OAM_FLIP_V,
	 8, 8, 0x77, 0|OAM_FLIP_H|OAM_FLIP_V,
	 128},
	{0, 0, 0x7D, 0|OAM_FLIP_H|OAM_FLIP_V,
	 8, 0, 0x7C, 0|OAM_FLIP_H|OAM_FLIP_V,
	 0, 8, 0x7B, 0|OAM_FLIP_H|OAM_FLIP_V,
	 8, 8, CHR_SP_BLANK, 0|OAM_FLIP_H|OAM_FLIP_V,
	 128}
};


const unsigned char OTM_R[] = {
	  0,  0, 0x00,0|OAM_BEHIND,
	  0,  8, 0x10,0|OAM_BEHIND,
	  0,  16,0x20,0|OAM_BEHIND,
	  0,  24,0x30,0|OAM_BEHIND,
	  8,   0,0x01,0|OAM_BEHIND,
	  8,   8,0x11,0|OAM_BEHIND,
	  8,  16,0x21,0|OAM_BEHIND,
	  8,  24,0x31,0|OAM_BEHIND,
	  8,  32,0x41,0|OAM_BEHIND,
	  8,  40,0x51,0|OAM_BEHIND,
	  8,  48,0x61,0|OAM_BEHIND,
	  16,  0,0x02,0|OAM_BEHIND,
	  16,  8,0x12,0|OAM_BEHIND,
	  16, 16,0x22,0|OAM_BEHIND,
	  16, 24,0x32,0|OAM_BEHIND,
	  16, 32,0x42,0|OAM_BEHIND,
	  16, 40,0x52,0|OAM_BEHIND,
	  16, 48,0x62,0|OAM_BEHIND,
	  24,  0,0x03,0|OAM_BEHIND,
	  24,  8,0x13,0|OAM_BEHIND,
	  24, 16,0x23,0|OAM_BEHIND,
	  24, 24,0x33,0|OAM_BEHIND,
	  24, 32,0x43,0|OAM_BEHIND,
	  24, 40,0x53,0|OAM_BEHIND,
	  24, 48,0x63,0|OAM_BEHIND,
	  128
};

const unsigned char OTM_RC_1[] = {
	  0,  24, 0xB0,0|OAM_BEHIND,
	  0,  32, 0xC0,0|OAM_BEHIND,
	  8,  16, 0xA1,0|OAM_BEHIND,
	  8,  24, 0xB1,0|OAM_BEHIND,
	  
	  16, 8, 0x92, 0|OAM_BEHIND,
	  16, 16,0xA2, 0|OAM_BEHIND,
	  16, 24,0xB2, 0|OAM_BEHIND,
	  16, 32,0xC2, 0|OAM_BEHIND,
	  16, 40,0xD2, 0|OAM_BEHIND,
	  16, 48,0xE2, 0|OAM_BEHIND,
	  
	  24, 0, 0x83, 0|OAM_BEHIND,
	  24, 8, 0x93, 0|OAM_BEHIND,
	  24, 16,0xA3, 0|OAM_BEHIND,
	  24, 24,0xB3, 0|OAM_BEHIND,
	  24, 32,0xC3, 0|OAM_BEHIND,
	  24, 40,0xD3, 0|OAM_BEHIND,
	  24, 48,0xE3, 0|OAM_BEHIND,
	  
	  32, 0, 0x84,0|OAM_BEHIND,
	  32, 8, 0x94,0|OAM_BEHIND,
	  32, 16,0xA4,0|OAM_BEHIND,
	  32, 24,0xB4,0|OAM_BEHIND,
	  32, 32,0xC4,0|OAM_BEHIND,
	  32, 40,0xD4,0|OAM_BEHIND,
	  32, 48,0xE4,0|OAM_BEHIND,

	  128
};

const unsigned char OTM_RC_2[] = {
	  0, 40, 0xD5,0|OAM_BEHIND,
	  0, 48, 0xE5,0|OAM_BEHIND,
	  
	  8, 8,  0x96, 0|OAM_BEHIND,
	  8, 16, 0xA6, 0|OAM_BEHIND,
	  8, 24, 0xB6, 0|OAM_BEHIND,
	  8, 32, 0xC6, 0|OAM_BEHIND,
	  8, 40, 0xD6, 0|OAM_BEHIND,
	  8, 48, 0xE6, 0|OAM_BEHIND,
	  
	  16, 0, 0x87, 0|OAM_BEHIND,
	  16, 8, 0x97, 0|OAM_BEHIND,
	  16, 16,0xA7, 0|OAM_BEHIND,
	  16, 24,0xB7, 0|OAM_BEHIND,
	  16, 32,0xC7, 0|OAM_BEHIND,
	  16, 40,0xD7, 0|OAM_BEHIND,
	  16, 48,0xE7, 0|OAM_BEHIND,
	  
	  24, 0, 0x88, 0|OAM_BEHIND,
	  24, 8, 0x98, 0|OAM_BEHIND,
	  24, 16,0xA8, 0|OAM_BEHIND,
	  24, 24,0xB8, 0|OAM_BEHIND,
	  24, 32,0xC8, 0|OAM_BEHIND,
	  24, 40,0xD8, 0|OAM_BEHIND,
	  24, 48,0xE8, 0|OAM_BEHIND,

	  128
};

const unsigned char OTM_RC_3[] = {
	  0, 8, 0x9A, 0|OAM_BEHIND,
	  0, 16,0xAA, 0|OAM_BEHIND,
	  0, 24,0xBA, 0|OAM_BEHIND,
	  0, 32,0xCA, 0|OAM_BEHIND,
	  0, 40,0xDA, 0|OAM_BEHIND,
	  0, 48,0xEA, 0|OAM_BEHIND,

	  8, 0, 0x8B, 0|OAM_BEHIND,
	  8, 8, 0x9B, 0|OAM_BEHIND,
	  8, 16,0xAB, 0|OAM_BEHIND,
	  8, 24,0xBB, 0|OAM_BEHIND,
	  8, 32,0xCB, 0|OAM_BEHIND,
	  8, 40,0xDB, 0|OAM_BEHIND,
	  8, 48,0xEB, 0|OAM_BEHIND,
	  
	  16, 0, 0x8C, 0|OAM_BEHIND,
	  16, 8, 0x9C, 0|OAM_BEHIND,
	  16, 16,0xAC, 0|OAM_BEHIND,
	  16, 24,0xBC, 0|OAM_BEHIND,
	  16, 32,0xCC, 0|OAM_BEHIND,
	  16, 40,0xDC, 0|OAM_BEHIND,
	  16, 48,0xEC, 0|OAM_BEHIND,

	  24, 24,0xBD, 0|OAM_BEHIND,
	  24, 32,0xCD, 0|OAM_BEHIND,
	  24, 40,0xDD, 0|OAM_BEHIND,
	  
	  128
};

const unsigned char OTM_L[] = {
	  24, 0, 0x00,0|OAM_FLIP_H|OAM_BEHIND,
	  24, 8, 0x10,0|OAM_FLIP_H|OAM_BEHIND,
	  24, 16,0x20,0|OAM_FLIP_H|OAM_BEHIND,
	  24, 24,0x30,0|OAM_FLIP_H|OAM_BEHIND,
	  16, 0, 0x01,0|OAM_FLIP_H|OAM_BEHIND,
	  16, 8, 0x11,0|OAM_FLIP_H|OAM_BEHIND,
	  16, 16,0x21,0|OAM_FLIP_H|OAM_BEHIND,
	  16, 24,0x31,0|OAM_FLIP_H|OAM_BEHIND,
	  16, 32,0x41,0|OAM_FLIP_H|OAM_BEHIND,
	  16, 40,0x51,0|OAM_FLIP_H|OAM_BEHIND,
	  16, 48,0x61,0|OAM_FLIP_H|OAM_BEHIND,
	  8,   0,0x02,0|OAM_FLIP_H|OAM_BEHIND,
	  8,   8,0x12,0|OAM_FLIP_H|OAM_BEHIND,
	  8,  16,0x22,0|OAM_FLIP_H|OAM_BEHIND,
	  8,  24,0x32,0|OAM_FLIP_H|OAM_BEHIND,
	  8,  32,0x42,0|OAM_FLIP_H|OAM_BEHIND,
	  8,  40,0x52,0|OAM_FLIP_H|OAM_BEHIND,
	  8,  48,0x62,0|OAM_FLIP_H|OAM_BEHIND,
	  0,   0,0x03,0|OAM_FLIP_H|OAM_BEHIND,
	  0,   8,0x13,0|OAM_FLIP_H|OAM_BEHIND,
	  0,  16,0x23,0|OAM_FLIP_H|OAM_BEHIND,
	  0,  24,0x33,0|OAM_FLIP_H|OAM_BEHIND,
	  0,  32,0x43,0|OAM_FLIP_H|OAM_BEHIND,
	  0,  40,0x53,0|OAM_FLIP_H|OAM_BEHIND,
	  0,  48,0x63,0|OAM_FLIP_H|OAM_BEHIND,
	  128
};

//BR, BL, TR, TL, palette 0-3

const unsigned char initial_pallete[8 * 8] = {
	0b01010101, 0b01010101, 0b01010101, 0b01010101, 0b01010101, 0b01010101, 0b01010101, 0b01010101,
	0b01010101, 0b01010101, 0b01010101, 0b01010101, 0b01010101, 0b01010101, 0b01010101, 0b01010101,
	0b01010101, 0b01010101, 0b01010101, 0b01010101, 0b01010101, 0b01010101, 0b01010101, 0b01010101,
	0b01010101, 0b01010101, 0b01010101, 0b01010101, 0b01010101, 0b01010101, 0b01010101, 0b01010101,
	0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111,
	0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111,
	0b10101010, 0b10101010, 0b10101010, 0b10101010, 0b10101010, 0b10101010, 0b10101010, 0b10101010,
	0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000
};


const unsigned char abilities_icons[4 * ABILITIES_COUNT] = {
	0x80, 0x81, 0x90, 0x91,
	0x82, 0x83, 0x92, 0x93,
	0x84, 0x85, 0x94, 0x95,
	0x86, 0x87, 0x96, 0x97,
	0x88, 0x89, 0x98, 0x99
};

