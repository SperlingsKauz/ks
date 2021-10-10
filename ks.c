#include "LIB/neslib.h"
#include "LIB/nesdoug.h"
#include "Const.h" 
#include "Misc.c" 
#include "ks.h" 


#pragma bss-name(push, "ZEROPAGE")

unsigned char pad1;
unsigned char pad1_new;
unsigned char pad2;


unsigned char tmp;
unsigned char state;
unsigned char index;
unsigned char marker;
unsigned char x;
unsigned char y;

unsigned int adr;
unsigned char frame_tick;
unsigned char rain_delay = DEFAULT_RAIN_DELAY;

unsigned char rain_drop_x[RAIN_COUNT];
unsigned char rain_drop_y[RAIN_COUNT];

struct StGrass grass = {
	0, //delay
	20, //delay max
	1,  //instant count
	18, //col_for_point
	};

#pragma bss-name(push, "BSS")

unsigned char debug = 0;

unsigned char points = 0;
unsigned int score = 0;

unsigned char game_calc_state = 0;

unsigned char bird_x[BIRDS_COUNT];
unsigned char bird_y[BIRDS_COUNT];
unsigned char bird_sprite[BIRDS_COUNT];
unsigned char bird_state[BIRDS_COUNT];
unsigned char bird_health[BIRDS_COUNT];
unsigned char bird_tag[BIRDS_COUNT];
unsigned char bird_speed_delay[BIRDS_COUNT];
unsigned char bird_x_speed[BIRDS_COUNT];
unsigned char bird_pallete[BIRDS_COUNT];

struct StPlayer player = {
	PLAYER_DEF_X, //x
	PLAYER_DEF_Y, //y
	32, //width
	64, //height
	0, //move delay
	2, //move max delay
	0, //move increase
	0, //move max increase
	DIR_LEFT, //direction
		{1,  //cut power 
		 2,  //cut size 
		 0,  //cutting state
		 10, //cut delay
		 10, //max cut delay
		 0   //count
		 },
		 { //abilities
		  0, //selection on upgrade screen
			{0, 0, 0, 0, 0}, //abilities
		  0	 //e-level
		 },
		 { //specials
			0, //delayed
			 {0, 0, 0}, //is_active
			 {0, 0, 0}  //state
		 }};

struct StSmallScythe small_scythe = { 
	0, //x
	0, //y
	16, //width
	8, //height
	0, //sprite
	0, //state
	0, //x_speed
	3  //y_speed
};

struct StGame game = {
	1, //level
	GAME_MAX_LEVEL, //last_level
	GAME_UNPAUSED, //paused 
	GAME_ST_MENU, //state
	//GAME_ST_UPGRADE, //state
	0, //play time
	10, //max_play_time
	0, //difficulty
	3, //lives
		{0, 1, 0} //lightning
	};


//----GENERAL-----------------------------------------------

void nmi_wait_and_prepare(void) {
	ppu_wait_nmi();
	pad1 = pad_poll(0);
	pad1_new = get_pad_new(0);
	clear_vram_buffer();
}

void clear_screen(void) {
	clear_vram_buffer();
	vram_adr(NAMETABLE_A);
	for(x = 0x00; x != 0x20; ++x) {
		for(y = 0x00; y != 0x1D; ++y) {
			vram_put(0x00);
		}
	}
	oam_clear();
}

void fade_in(void) {
	pal_fade_to(5, 0);
	delay(10);
}

void fade_out(void) {
	delay(10);
	pal_fade_to(0, 4);
}

void delay_with_state_skip(unsigned char delay_length) {
	for(tmp = 1; tmp != delay_length; ++tmp) {
		nmi_wait_and_prepare();
		if (state == 1) break;
		if (pad1_new & PAD_START) state = 1;
	}
}

void put_str(unsigned int address, const char *str) {
	vram_adr(address);
	while(1) {
		if (!*str) break;
		vram_put((*str++) - 0x00);
	}
} 

void put_char(unsigned int address, const char *str, unsigned char num) {
	unsigned char npos = 0;
	while(1) {
		if (!*str) break;
		if (npos == num) {
			one_vram_buffer(*str, address);
			break;
		} else {
			*str++;
			++npos;
		}
	}
} 


void put_number(unsigned int address, unsigned int number, unsigned char zero_count) {
	adr = number;
	x = 0;
	
	while(adr != 0) {
		++x;
		adr /= 10;
	}

	if(number == 0) {
		vram_adr(address + 1);
		vram_put(CHR_ZERO);
	} else {
		for(y = 0x00; y != x; ++y) {
			adr = number % 10;
			number /= 10;
			vram_adr(address + x - y);
			vram_put(CHR_ZERO + adr);
		}
		vram_adr(address + x + 1);
		for(y = 0x00; y != zero_count; ++y) {
			vram_put(CHR_ZERO);
		}
	}
}


void set_bg_pallete(const char *pallete) {
	vram_adr(PPU_PALLETE);
	for(x = 0x00; x != 0x40; ++x) {
		vram_put(pallete[x]);
	}
}

//----TITLE-----------------------------------------------

void process_title(void) {
	title_draw_background();
	title_draw_menu();
	music_play(MSC_MENU);
	x = state = 0;
	y = 0x15;
	
	while(1) {
		nmi_wait_and_prepare();
	
		//draw logo
		oam_clear();
		for(tmp = 0x00; tmp != LOGO_TILE_COUNT * 4; tmp += 4) {
			oam_spr(LOGO[tmp] + LOGO_X_OFFSET, LOGO[tmp + 1] + LOGO_Y_OFFSET, LOGO[tmp + 2], LOGO[tmp + 3]);
		}		
		
		//animate cursor
		++x;
		if (x == 0x06) { 
			x = 0;
			if (state == 0) {
				++y;
				if (y == 0x18) state = 1;
			} else {
				--y;
				if (y == 0x15) state = 0;
			}
		}
		
		if (state == 0) oam_spr(game.state == GAME_ST_MENU ? 0x5E : 0x3C, 0x77 + (menu_position * 0x10), y, 0b00000001);
			else oam_spr(game.state == GAME_ST_MENU ? 0x5E : 0x3C, 0x76 + (menu_position * 0x10), y, 0b10000001);
		
		//movement and actions
		
		if (game.state == GAME_ST_MENU) title_check_cheat_code();
		
		if (pad1_new & PAD_DOWN || pad1_new & PAD_UP) {
			sfx_play(SND_MENU_MOVE, 0);
			menu_items = game.state == GAME_ST_MENU ? 1 : 3;
			if (pad1_new & PAD_DOWN) menu_position = menu_position == menu_items ? 0 : ++menu_position;
			if (pad1_new & PAD_UP) menu_position = menu_position == 0 ? menu_items : --menu_position;
		}
		
		if (pad1_new & PAD_START) {
			switch (game.state) {
				case GAME_ST_MENU:
					switch (menu_position) {
						case 0:
							x = y = state = 0;
							music_stop();
							sfx_play(SND_MENU_START, 0);
							while(1) {
								ppu_wait_nmi();
								clear_vram_buffer();
								++x;
								if (x == 3) {
									multi_vram_buffer_horz(state == 0 ? "START" : "     ", 0x05, NTADR_A(0x0D,0x0F));
									state = state == 0 ? 1 : 0;
									x = 0;
									++y;
									if (y == 0x12) return;
								}
							}
							break;
						case 1:
							game.state = GAME_ST_OPTIONS;
							menu_position = 0;
							title_draw_menu();
							break;
					}
				case GAME_ST_OPTIONS:
					switch (menu_position) {
						case 0:
							game.difficulty = game.difficulty == 2 ? 0 : ++game.difficulty;
							title_draw_options();
							break;
						case 1:
							game.last_level = game.last_level == GAME_MAX_LEVEL ? 0 : GAME_MAX_LEVEL; 
							title_draw_options();
							break;	
						case 2:
							game.effects.lightning_enabled = game.effects.lightning_enabled == 0 ? 1 : 0; 
							title_draw_options();
							break;
						case 3:
							game.state = GAME_ST_MENU;
							menu_position = 0;
							title_draw_menu();
							break;
					}
					break;
			}
			
		}
	}
};

void title_draw_options(void) {
	multi_vram_buffer_horz(game.effects.lightning_enabled == 0 ? "OFF": "ON ", 0x03, NTADR_A(0x14, 0x13));
	multi_vram_buffer_horz(game.last_level == 0 ? "ON" : "OFF ", 0x03, NTADR_A(0x14, 0x11));
	switch(game.difficulty) {
		case 0: multi_vram_buffer_horz("EASY  ", 0x06, NTADR_A(0x14, 0x0F)); break;
		case 1: multi_vram_buffer_horz("NORMAL", 0x06, NTADR_A(0x14, 0x0F)); break;
		case 2: multi_vram_buffer_horz("HARD  ", 0x06, NTADR_A(0x14, 0x0F)); break;
	}
}

void title_draw_menu(void) {
	for (index = 0; index != 4; ++index)
		multi_vram_buffer_horz("                    ", 0x14, NTADR_A(0x09, 0x0F + index + index));
	nmi_wait_and_prepare();
	switch (game.state) {
		case GAME_ST_MENU:
			multi_vram_buffer_horz("START", 0x05, NTADR_A(0x0D, 0x0F));
			multi_vram_buffer_horz("OPTIONS", 0x07, NTADR_A(0x0D, 0x11));
			break;
		case GAME_ST_OPTIONS:
			multi_vram_buffer_horz("LEVEL   ", 0x08, NTADR_A(0x09, 0x0F));
			multi_vram_buffer_horz("ENDLESS ", 0x08, NTADR_A(0x09, 0x11));
			multi_vram_buffer_horz("FLASH   ", 0x08, NTADR_A(0x09, 0x13));
			multi_vram_buffer_horz("RETURN  ", 0x08, NTADR_A(0x09, 0x15));
			title_draw_options();
			break;
	}
	index = 0;
}

void title_draw_background(void) {
	ppu_off();
	pal_bg(title_palette_bg);
	pal_spr(title_palette_sp);
	clear_screen();

	vram_adr(PPU_PALLETE);
	for(x = 0x00; x != 0x40; ++x) vram_put(0);
	
	ppu_on_all();
	//draw logo
	delay(30);
	oam_clear();
	x = state = 0;
	y = 8;
	sfx_play(SND_MENU_HIT, 0);
	oam_spr(LOGO[0] + LOGO_X_OFFSET, LOGO[1] + LOGO_Y_OFFSET, LOGO[2], LOGO[3]);
	delay_with_state_skip(40);
	sfx_play(SND_MENU_HIT, 0);
	oam_spr(LOGO[4] + LOGO_X_OFFSET, LOGO[5] + LOGO_Y_OFFSET, LOGO[6], LOGO[7]);
	delay_with_state_skip(40);
	
	while(1) {
		nmi_wait_and_prepare();
		if (state == 1) break;
		if (pad1_new & PAD_START) state = 1;
		++x;
		if (x == 0x05) {
			x = 0;
			sfx_play(SND_MENU_HIT, 0);
			oam_spr(LOGO[y] + LOGO_X_OFFSET, LOGO[y + 1] + LOGO_Y_OFFSET, LOGO[y + 2], LOGO[y + 3]);
			y += 4;
			if (y == LOGO_TILE_COUNT * 4) break;
		}
	}
	
	//draw title and menu
	delay_with_state_skip(60);
	if (state == 0) sfx_play(SND_GAME_LIGHTNING, 0);
	multi_vram_buffer_horz("ONE THOUSAND MAN", 0x10, NTADR_A(0x08, 0x0C));
	delay_with_state_skip(90);
}

void title_check_cheat_code(void) {
	if (marker < 10) {
		if (pad1_new & PAD_UP) {
			if (marker == 0 || marker == 1) ++marker; else marker = 0;
		}
		if (pad1_new & PAD_DOWN) {
			if (marker == 2 || marker == 3) ++marker; else marker = 0;
		}
		if (pad1_new & PAD_LEFT) {
			if (marker == 4 || marker == 6) ++marker; else marker = 0;
		}
		if (pad1_new & PAD_RIGHT) {
			if (marker == 5 || marker == 7) ++marker; else marker = 0;
		}
		if (pad1_new & PAD_B) {
			if (marker == 8) ++marker; else marker = 0;
		}
		if (pad1_new & PAD_A) {
			if (marker == 9) {
				++marker;
				sfx_play(SND_MENU_ERROR, 0);
				multi_vram_buffer_horz("KONAMI CODE NOT FOUND", 0x15, NTADR_A(0x06, 0x17));
			} else {
				marker = 0;
			}
		}
	} else if (marker < 249) {
		++marker;
		if ((marker - 10) % 20 == 0) {
			if (marker > 130) {
				one_vram_buffer(CHR_DOT, NTADR_A(0x08 + (marker - 120) / 10, 0x19));
			} else {
				one_vram_buffer(CHR_DOT, NTADR_A(0x08 + marker / 10, 0x19));
			}
		}
		if (marker == 140) {
			multi_vram_buffer_horz("SEARCHING FOR ALTERNATIVES", 0x1A, NTADR_A(0x03, 0x17));
			multi_vram_buffer_horz("                 ", 0x11, NTADR_A(0x0A, 0x19));
		}			
		if (marker == 249) {
			multi_vram_buffer_horz("   KOSARI CODE ACTIVATED  ", 0x1A, NTADR_A(0x03, 0x17));
			multi_vram_buffer_horz("                 ", 0x11, NTADR_A(0x0B, 0x19));
			sfx_play(SND_MENU_START, 0);
			points = 99;
		}
	}
}

//----MAIN GAME-----------------------------------------------

void game_initial_values_set(void) {
	//set initial rain drops
	for(x = 0x00; x != RAIN_COUNT; ++x) {
		rain_drop_x[x] = 0;
		rain_drop_y[x] = 0;
	}
	
	points = 1;
	score = 0;
	game.level = 1;
	game.lives = 3;
	player.move_delay = 0; 
	player.move_max_delay = 2;
	player.move_increase = 0;
	player.move_max_increase = 0;
	player.abilities.selection = 0;
	player.abilities.list[ABILITY_CUT_SPEED] = 0;
	player.abilities.list[ABILITY_CUT_POWER] = 0;
	player.abilities.list[ABILITY_CUT_SIZE] = 0;
	player.abilities.list[ABILITY_MOVE_SPEED] = 0;
	player.abilities.elevel = 0;
	
	set_vram_buffer(); 
	ppu_off();
	bank_spr(1);
	ppu_on_all();
}

void process_main_game(void) {
	while(1) {
		game_process_next_level();
		game_draw_background();
		game_draw_points();
		game_draw_time();
		while(1) {
			nmi_wait_and_prepare();
			if (game.paused == GAME_UNPAUSED) {
				game_small_scythe_processing();
				game_draw_sprites();
				game_enemy_processing();
				game_draw_rain();
				game_draw_effects();
				if (game.state == GAME_ST_LEVEL_LOSE) {
					game_grass_overgrow();
					if (game.state == GAME_ST_LEVEL_END_LOSE) break;
				} else {
					if (game.state == GAME_ST_LEVEL_WIN) {
						game_grass_fading();
						if (game.state == GAME_ST_LEVEL_END_WIN) break;
					} else {
						game_movement_and_actions();
						game_grass_grow();
						//Check if game time ends and activate win state
						//Otherwise process events
						++frame_tick;
						if (frame_tick == FRAME_COUNT_SEC) {
							frame_tick = 0;
							--game.play_time;
							if (game.play_time == 0) {
								sfx_play(SND_GAME_WIN, 0);
								game.state = GAME_ST_LEVEL_WIN;
								adr = 0;
							} else {
								if ((game.max_play_time - game.play_time) % 0x0A == 0) game.effects.lightning = 1;
								if ((game.max_play_time - game.play_time) % 0x0E == 0) game.effects.wind = 1;
								tmp = 1;
								for (x = 0; x < BIRDS_COUNT; ++x) {
									if (bird_state[x] == 0) {
										if (game.level > 8) tmp = rand8() % 0x01;
											else if(game.level > 5) tmp = rand8() % 0x02;
												else tmp = rand8() % 0x03;
										if (tmp == 0) game_create_enemy(x);
										break;
									}
								}
							}
							game_draw_time();
						}
						if (pad1_new & PAD_START) game.paused = GAME_PAUSED;
					}
				} 
			} else {
				if (pad1_new & PAD_START) game.paused = GAME_UNPAUSED;
			}
		}
		clear_vram_buffer();
		if (game.lives == 0x00 || game.level == game.last_level) break;
			else process_upgrades();
	}
}

void game_process_next_level(void) {
	fade_in();
	ppu_off();
	pal_bg(game_palette_bg);
	pal_spr(game_palette_sp);
	clear_vram_buffer();

	if (game.state == GAME_ST_LEVEL_END_WIN) {
		++game.level;
		switch(game.difficulty) {
			case GAME_DF_EASY:
				if (grass.delay_max != 10) --grass.delay_max;
				if (game.level % 3 == 0 && grass.instant_count != 10) ++grass.instant_count;
				if (game.max_play_time != 60) game.max_play_time += 2;
				break;
			case GAME_DF_MEDIUM:
				if (grass.delay_max != 8) --grass.delay_max;
				if (game.level % 2 == 0 && grass.instant_count != 10) ++grass.instant_count;
				if (game.max_play_time != 60) game.max_play_time += 4;
				break;
			case GAME_DF_HARD:
				if (grass.delay_max != 6) --grass.delay_max;
				if (grass.instant_count != 10) ++grass.instant_count;
				if (game.max_play_time != 60) game.max_play_time += 6;
				break;
		}
	}
	
	game.play_time = game.max_play_time;
	game.state = GAME_ST_LEVEL_PLAYING;
	grass.delay = grass.delay_max;
	
	//draw blank tiles
	vram_adr(NAMETABLE_A);
	for(x = 0x00; x != 0x20; ++x) 
		for(y = 0x00; y != 0x1D; ++y) 
			vram_put(CHR_BG_BLANK);
	
	//reset birds
	for(x = 0x00; x != BIRDS_COUNT; ++x) 
		bird_state[x] = 0;
	
	for (x = 0x00; x < RAIN_COUNT; ++x) {
		rain_drop_x[x] = rand8();
		rain_drop_y[x] = rand8();
	}
	//output score
	put_str(NTADR_A(0x0D, 0x09), "SCORE");
	put_number(NTADR_A(0x12, 0x09), score, 2);
	
	//output level num and lives count
	put_str(NTADR_A(0x0D, 0x0B), "LEVEL");
	
	vram_adr(NTADR_A(0x13, 0x0B));
	if (game.level >= 10) {
		vram_put(CHR_ZERO + game.level / 10);
		vram_put(CHR_ZERO + game.level % 10);
	} else {
		vram_put(CHR_ZERO + game.level);
	}
	
	
	vram_adr(NTADR_A(0x0F, 0x0D));
	vram_put(CHR_LIVE);
	vram_put(CHR_CROSS);
	vram_put(CHR_ZERO + game.lives);
	
	oam_clear();
	ppu_on_all();
	fade_out();
	
	delay(120);
}


void game_draw_background(void) {
//	$23c0-$23ff
	set_rand(get_frame_count());
	fade_in();
	
	ppu_off();
	
	clear_vram_buffer();
	clear_screen();

	//set pallete for all bg tiles
	set_bg_pallete(initial_pallete);
	
	//stars;
	for(x = 0x00; x != STARS_COUNT; ++x) {
		vram_adr(NAMETABLE_A + rand8() * 3 - SCREEN_TILES_X * 5);
		vram_put(0x2B);
		vram_adr(NAMETABLE_A + rand8() * 3 - SCREEN_TILES_X * 5);
		vram_put(0x2D);
	}
	
	//moon
	index = 0x10;
	for(y = MOON_Y; y != MOON_Y + 0x04; ++y) {
		vram_adr(NAMETABLE_A + y * SCREEN_TILES_X + MOON_X);
		for(x = 0x00; x != 0x04; ++x) {
			vram_put(index);
			++index;
		}
	}

	//initial grass height
	vram_adr(NAMETABLE_A + 0x20 * GRASS_BORDER);
	for(x = 0x00; x != GRASS_WIDTH; ++x) {
		grass.height[x] = rand8() % 0x03 + 1;
		vram_put(grass.height[x]);
	}

	//ground
	for(x = 0x00; x != GRASS_WIDTH; ++x) vram_put(0x06 + (rand8() % 4));
	for(x = 0x00; x != GRASS_WIDTH * 2; ++x) vram_put(0x0A + (rand8() % 4));
	for(x = 0x00; x != GRASS_WIDTH; ++x) {
		state = state == 0x0E ? 0x0F : 0x0E;
		vram_put(state);
	}
	
	//status bar
	vram_adr(NAMETABLE_A + SCREEN_TILES_X * 0x1C);
	for (x = 0x00; x != 0x20; ++x) 
		vram_put(STATUS_BAR[x] == 0 ? CHR_BG_BLANK : STATUS_BAR[x]);
	vram_adr(NAMETABLE_A + SCREEN_TILES_X * 0x1C + 0x03);
	vram_put(CHR_ZERO + game.lives);

	ppu_on_all();
	fade_out();
}

void game_draw_points(void) {
	one_vram_buffer(CHR_ZERO + points / 10, NTADR_A(0x0B, Y_EDGE));
	one_vram_buffer(CHR_ZERO + points % 10, NTADR_A(0x0C, Y_EDGE));
}

void game_draw_time(void) {
	one_vram_buffer(CHR_ZERO + game.play_time / 10, NTADR_A(0x15, Y_EDGE));
	one_vram_buffer(CHR_ZERO + game.play_time % 10, NTADR_A(0x16, Y_EDGE));
}

void game_draw_sprites(void) {
	oam_clear();

	if (player.cut.state == 0 || player.cut.state == CUT_ST_RECOVER) {
		if (player.direction == DIR_LEFT) oam_meta_spr(player.x, player.y, OTM_L);
			else oam_meta_spr(player.x, player.y, OTM_R);
	} else {
		--player.cut.delay;
		if (player.cut.delay == 0) {
			player.cut.delay = player.cut.max_delay;
			if (player.cut.state == CUT_ST_C3) {
				player.cut.state = CUT_ST_RECOVER;
				game_grass_cut();
			} else {
				++player.cut.state;
			}
		};
		if (player.cut.state == CUT_ST_C1 || player.cut.state == CUT_ST_RECOVER) oam_meta_spr(player.x - 8, player.y, OTM_RC_1);
		if (player.cut.state == CUT_ST_C2) oam_meta_spr(player.x, player.y, OTM_RC_2);
		if (player.cut.state == CUT_ST_C3) oam_meta_spr(player.x + 8, player.y, OTM_RC_3);
	}
	
	if (small_scythe.state != 0) {
		oam_meta_spr(small_scythe.x, small_scythe.y, SCYTHE_STATE[small_scythe.sprite]);
	}	
}

void game_draw_effects(void) {
	if (game.effects.lightning != 0 && game.effects.lightning_enabled) {
		++game.effects.lightning;
		switch (game.effects.lightning) {
			case 2: sfx_play(SND_GAME_LIGHTNING, 0);
					break;
			case 8: pal_col(0, 0x30);
					break;
			case 16: pal_col(0, 0x0F);
					break;
			case 24: pal_col(0, 0x30);
					break;
			case 32: pal_col(0, GAME_BG_COLOR);
					break;
			case 60: game.effects.lightning = 0;
					break;
		}
	}
	
	if (game.effects.wind != 0) {
		++game.effects.wind;
		switch (game.effects.wind) {
			case 2	: sfx_play(SND_GAME_WIND, 0);
					break;
			case 90: game.effects.wind = 0;
					 break;
		}
	}
}


void game_draw_rain(void) {
	--rain_delay;
	//move rain drop sprites if max delay reached
	if (rain_delay == 0) {
		rain_delay = DEFAULT_RAIN_DELAY;
		for (index = 0x00; index != RAIN_COUNT; ++index) {
			//x = rand8();
			rain_drop_x[index] += rain_drop_y[index];
			rain_drop_y[index] += 0x10;
			if (rain_drop_y[index] > 200) rain_drop_y[index] = 10;
		}
	}
	//using library forces to redraw sprites
	for (index = 0x00; index != RAIN_COUNT; ++index) {
		oam_spr(rain_drop_x[index], rain_drop_y[index], 0x04, 0b00000010);
	}
}

//Two functions to reuse same code in grass cut and fade
void game_grass_cf_prepare(void) {
	if (single_sprout > 8) index = 2; 
		else if (single_sprout > 4) index = 1; 
			else index = 0;
}

void game_grass_cf_finalize(void) {
	if (index == 2) {
		if (single_sprout < 9) one_vram_buffer(0, NTADR_A(x, GRASS_BORDER - 2));
	}
	if (index == 1 || index == 2) {
		if (single_sprout < 5) one_vram_buffer(0, NTADR_A(x, GRASS_BORDER - 1));
	}
	if (single_sprout > 8) index = 2; 
		else if (single_sprout > 4) index = 1; 
			else index = 0;
	one_vram_buffer(single_sprout - index * 4, NTADR_A(x, GRASS_BORDER - index));		

	grass.height[x] = single_sprout;
}
//-------------------------------------

void game_grass_grow(void) {
	if (game_calc_state == GCS_CHECK) {
		//check for critical grass amount and refresh status 
		game_calc_state = GCS_NORMAL;
		total_height = 0;
		
		for(x = 0x00; x != 0x20; ++x) total_height += grass.height[x];

		if (total_height > GRASS_SBLOCK * 6) {
			one_vram_buffer(0xA6, NTADR_A(0x1D, Y_EDGE));
			one_vram_buffer(0x9E + total_height / GRASS_SBLOCK, NTADR_A(0x1E, Y_EDGE));
			//lose game if total height is critical
			if (total_height > GRASS_SBLOCK * 8) {
				sfx_play(SND_GAME_LOSE, 0);
				game.state = GAME_ST_LEVEL_LOSE;
				player.cut.state = 0;
				--game.lives;
			}
		} else {
			if (total_height > GRASS_SBLOCK * 4) {
				one_vram_buffer(0xA3, NTADR_A(0x1D, Y_EDGE));
				one_vram_buffer(total_height > GRASS_SBLOCK * 5 ? 0xA0 : 0x7F, NTADR_A(0x1E, Y_EDGE));
			} else {
				one_vram_buffer(0x9F + total_height / GRASS_SBLOCK, NTADR_A(0x1D, Y_EDGE));
				one_vram_buffer(0x7F, NTADR_A(0x1E, Y_EDGE));
			}
		}
	} else {
		//operate grass grow
		--grass.delay;
		if (grass.delay == 0x00) {
			game_calc_state = GCS_CHECK;
			grass.delay = grass.delay_max;
			for (y = 0x00; ; ++y) {
				x = (rand8() >> 3);
				single_sprout = grass.height[x];
				if (single_sprout != 12) {
					++single_sprout;
					if (single_sprout == 5) one_vram_buffer(5, NTADR_A(x, GRASS_BORDER));
					if (single_sprout == 9) one_vram_buffer(5, NTADR_A(x, GRASS_BORDER - 1));
					if (single_sprout > 8) index = 2; 
						else if (single_sprout > 4) index = 1; 
							else index = 0;
					grass.height[x] = single_sprout;
					one_vram_buffer(single_sprout - index * 4, NTADR_A(x, GRASS_BORDER - index));
				}
				if (y == grass.instant_count) break;
			}
		}
	} 
}

//single_sprout = tmp
//corr_power = marker
void game_grass_cut(void) {
	x = (player.x / 8) + 2;
	state = 0;
	corr_power = player.cut.power;
	
	if (game.effects.lightning == 0 && game.effects.wind == 0) sfx_play(SND_GAME_CUT, 1);
	for (y = 0; y != player.cut.size; ++y) {
		//state used for cutting set - one to left, one to right, etc
		if (state == 0) {
			x += y;
			state = 1;	
			if (player.abilities.list[ABILITY_CUT_POWER] < ABILITY_MAX_LEVEL) {
				if (corr_power > (player.abilities.list[ABILITY_CUT_POWER] < ABILITY_EX_LEVEL ? 1 : 2)) --corr_power;
			}
		} else {
			x -= y;
			state = 0;
		}
		single_sprout = grass.height[x];
		
		//index used in subfunction - marker of grass height - 1, 2 or 3 tiles
		game_grass_cf_prepare();
		if (corr_power >= single_sprout) {
			single_sprout = 1;
			score += (corr_power - single_sprout);
			player.cut.count += (corr_power - single_sprout);
		} else {
			single_sprout -= corr_power;
			score += corr_power;
			player.cut.count += corr_power;
		}
		game_check_for_extra_point();
		game_grass_cf_finalize();
	}
	
	if (player.specials.active[SP_SLASH] != 0) {
		--player.specials.active[SP_SLASH];
		if (player.specials.active[SP_SLASH] == 0) {
			player.cut.max_delay = player.cut.temp_store;
		}
	}
}	

//every new point takes more grass then previous
void game_check_for_extra_point(void) {
	if (points < GAME_MAX_POINTS && player.cut.count > grass.col_for_point) {
		player.cut.count = 0;
		switch (game.difficulty) {
			case GAME_DF_EASY :   grass.col_for_point += 1; break;
			case GAME_DF_MEDIUM : grass.col_for_point += 3; break;
			case GAME_DF_HARD :   grass.col_for_point += 5; break;
		}
		++points;
		game_draw_points();
	}
}

//when winning level continuosly fade all grass
void game_grass_fading(void) {
	x = (rand8() >> 3);
	single_sprout = grass.height[x];
	if (single_sprout > 2) {
		game_grass_cf_prepare();
		--single_sprout;
		game_grass_cf_finalize();
	}
	++adr;
	if (adr == GAME_WIN_TIME) game.state = GAME_ST_LEVEL_END_WIN;
}

//when losing level hide player with grass
void game_grass_overgrow(void) {
	--grass.delay;
	if (grass.delay == 0x00) {
		game_calc_state = GCS_CHECK;
		grass.delay = 0x14;
		state = (player.x & 0xF8) / 8;
		index = 0;
		for (x = state; x != state + 6; ++x) {
			single_sprout = grass.height[x];
			for (y = 0x01; y != 0x07; ++y) {
				if (single_sprout <= y * 4) {
					one_vram_buffer(0xA3, NTADR_A(x, GRASS_BORDER - y + 1));
					one_vram_buffer(0x01, NTADR_A(x, GRASS_BORDER - y));
					break;
				}
			}
			single_sprout += 4;
			grass.height[x] = single_sprout;
			if (y == 0x07) ++index;
		}
		if (index == 6) game.state = GAME_ST_LEVEL_END_LOSE;
	}
}


void game_movement_and_actions(void) {
	if (player.specials.active[SP_DASH] != 0) {
		if (ability_max) x = SP_DASH_SPEED_MAX;
			else if (ability_ex) x = SP_DASH_SPEED_MED;
			else x = SP_DASH_SPEED_LOW;
		if (player.specials.state[SP_DASH] == SP_DASH_LEFT) {
			if (player.x < x + 3) player.x = 3; else player.x -= x;
		} else {
			player.x += x;
			if (player.x > RIGHT_EDGE - PLAYER_WIDTH) player.x = RIGHT_EDGE - PLAYER_WIDTH;
		}
		--player.specials.active[SP_DASH];
		player.y += player.specials.active[SP_DASH] > SP_DASH_JUMP ? -1 : 1;
		if (player.specials.active[SP_DASH] == 0) player.specials.state[SP_DASH] = SP_IDLE;
	} else {
		if (pad1 & PAD_LEFT || pad1 & PAD_RIGHT) {
			if ((ability_max && player.abilities.list[ABILITY_MOVE_SPEED] == ABILITY_MAX_LEVEL) || player.cut.state == CUT_ST_IDLE) {
				if (player.move_delay == 0) {
					if (player.move_max_delay != 0) player.move_delay = player.move_max_delay;
					x = 1;
					if (player.cut.state == CUT_ST_IDLE) {
						if (player.move_max_increase != 0) {
							--player.move_increase;
							if (player.move_increase == 0) {
								player.move_increase = player.move_max_increase;
								++x;
							}
						}
					}
					if (pad1 & PAD_LEFT) {
						player.x = (player.x - 3) < x ? 3 : player.x - x;
						player.direction = DIR_LEFT;
					} else {
						player.x += x;
						if (player.x > RIGHT_EDGE - PLAYER_WIDTH) player.x = RIGHT_EDGE - PLAYER_WIDTH;
						player.direction = DIR_RIGHT;
					}
				} else {
					--player.move_delay;
				}
			}
		}
	}
	if (game.state == GAME_ST_LEVEL_PLAYING ) {
		if (pad1 & PAD_B || player.specials.active[SP_SLASH] != 0) {
			if (player.cut.state == CUT_ST_IDLE || (player.specials.active[SP_SLASH] != 0 && player.cut.state == CUT_ST_RECOVER)) {
				player.cut.state = CUT_ST_C1;
				if (player.direction == DIR_LEFT) player.direction = DIR_RIGHT;
			}
		} else {
			if (player.cut.state == CUT_ST_RECOVER) player.cut.state = CUT_ST_IDLE;
		}
	}
	game_check_specials_keys();
}	

//operate abilities activation
void game_check_specials_keys(void) {
	if (player.specials.delayed > 0) {
		--player.specials.delayed;
		if (player.specials.delayed == 0) {
			player.specials.state[SP_DASH] = SP_IDLE;
			player.specials.state[SP_SLASH] = SP_IDLE;
		}
	}
	
	if (player.abilities.elevel >= SP_SSCYTHE_ALEVEL) {
		if (pad1 & PAD_A && pad1 & PAD_UP && small_scythe.state == 0) {
			tmp = ability_max ? SP_SSCYTHE_SPEED_EX : SP_SSCYTHE_SPEED;
			small_scythe.x = player.x + 0x0A;
			small_scythe.y = player.y;
			small_scythe.y_speed = tmp;
			if (pad1 & PAD_LEFT) small_scythe.x_speed = -tmp;
				else if (pad1 & PAD_RIGHT) small_scythe.x_speed = tmp;
					else small_scythe.x_speed = 0;
			small_scythe.state = small_scythe.sprite = 1;
		}
	}
	
	if (player.abilities.elevel >= SP_DASH_ALEVEL && player.specials.active[SP_DASH] == 0) {
		switch (player.specials.state[SP_DASH]) {
			case SP_IDLE:
				if (pad1_new & PAD_RIGHT) {
					player.specials.state[SP_DASH] = SP_DASH_RIGHT;
					player.specials.delayed = SP_MAX_DELAY;
				}
				if (pad1_new & PAD_LEFT) {
					player.specials.state[SP_DASH] = SP_DASH_LEFT;
					player.specials.delayed = SP_MAX_DELAY;
				}
				break;
			case SP_DASH_LEFT:
				if (pad1_new & PAD_LEFT) {
					player.specials.active[SP_DASH] = SP_DASH_LENGTH;
					player.specials.delayed = 0;
				} 
				break;
			case SP_DASH_RIGHT:
				if (pad1_new & PAD_RIGHT) {
					player.specials.active[SP_DASH] = SP_DASH_LENGTH;
					player.specials.delayed = 0;
				} 
				break;
		}
	}
	if (pad1_new & PAD_DOWN) {
		if (player.abilities.elevel >= SP_SLASH_ALEVEL && player.specials.active[SP_SLASH] == 0) {
			++player.specials.state[SP_SLASH];
			if (player.specials.state[SP_SLASH] == SP_SLASH_TRIPLE_DOWN) {
				player.specials.active[SP_SLASH] = ability_max ? SP_SLASH_COUNT_EX : SP_SLASH_COUNT;
				player.specials.state[SP_SLASH] = SP_IDLE;
				player.cut.temp_store = player.cut.max_delay;
				player.cut.max_delay = ability_max ? SP_SLASH_FAST_DELAY_EX : SP_SLASH_FAST_DELAY;
			} else {
				player.specials.delayed = SP_MAX_DELAY;
			}
		}
	}
}

//processing of throwable weapon
void game_small_scythe_processing(void) {
	if (small_scythe.state != 0) {
		x = small_scythe.x + small_scythe.x_speed;
		y = small_scythe.y - small_scythe.y_speed;
		if (x > RIGHT_EDGE || y > BOTTOM_EDGE) {
			small_scythe.state = 0;
		} else {
			++small_scythe.state;
			small_scythe.x = x;
			small_scythe.y = y;
			if (small_scythe.state == CHR_SC_ANIM_DELAY) {
				small_scythe.state = 1;
				small_scythe.sprite = small_scythe.sprite == 7 ? 0 : small_scythe.sprite + 1;
			}	
		}
	}
}

//create bird on random place and stats depends on pallete
void game_create_enemy(char bird_num) {
	if (bird_state[bird_num] == 0) {
		bird_state[bird_num] = 1;
		if (rand8() % 2 == 0) {
			bird_x[bird_num] = 2;
			bird_x_speed[bird_num] = 1;
		} else {
			bird_x[bird_num] = 235;
			bird_x_speed[bird_num] = -1;
		}
		bird_tag[bird_num] = 0;
		bird_y[bird_num] = 30 + (rand8() >> 3);
		bird_pallete[bird_num] = rand8() % 3 + 1;
		bird_health[bird_num] = bird_pallete[bird_num] == 3 ? 4 : bird_pallete[bird_num];
		if (game.level >= 5) ++bird_health[bird_num];
		if (game.level >= 10) ++bird_health[bird_num];
		bird_speed_delay[bird_num] = bird_health[bird_num];
	}
}

//check if birds have collision with small scythe 
//process hit and death of birds
void game_enemy_processing(void) {
	for (x = 0; x < BIRDS_COUNT; ++x) {
		if (bird_state[x] != 0) {
			++bird_state[x];
			--bird_speed_delay[x];
			if (bird_speed_delay[x] == 0) {
				bird_x[x] += bird_x_speed[x];
				bird_speed_delay[x] = bird_health[x];
			}
			if (bird_state[x] == 0x0A) {
				bird_state[x] = 1;
				bird_sprite[x] = bird_sprite[x] == 1 ? 0 : 1;
			}
			if (bird_x[x] <= 1 || bird_x[x] > 235) bird_state[x] = 0;
			if (small_scythe.state != 0 && bird_tag[x] == 0) {
				if (small_scythe.x <= bird_x[x] + BIRDS_WIDTH && 
				    small_scythe.x + SP_SSCYTHE_SIZE >= bird_x[x] &&
				    small_scythe.y <= bird_y[x] + BIRDS_HEIGHT && 
					small_scythe.y + SP_SSCYTHE_SIZE >= bird_y[x]) {
					tmp = ability_ex ? SP_SSCYTHE_DMG_EX : SP_SSCYTHE_DMG;
					small_scythe.state = 0;
					if (bird_health[x] <= tmp) {
						bird_tag[x] = BIRDS_TAG_DIE;
						sfx_play(SND_BIRD_DEATH, 1);
						score += bird_pallete[x] * 10;
						if (points < GAME_MAX_POINTS) {
							switch (bird_pallete[x]) {
								case 1: player.cut.count += grass.col_for_point >> 1; 
										game_check_for_extra_point();
										break;
								case 2: ++points; 
										game_draw_points();
										break;
								case 3: points += 2; 
										game_draw_points();
										break;
							}
						}
					} else {
						bird_health[x] -= tmp;
						bird_tag[x] = BIRDS_TAG_HIT;
					}
				}
			}
			tmp = 0;
			if (bird_tag[x] > 0) --bird_tag[x];
			if (bird_tag[x] > BIRDS_TAG_HIT) {
				tmp = BIRDS_TAG_DIE - bird_tag[x];
				if (bird_tag[x] == BIRDS_TAG_HIT + 1) {
					bird_state[x] = 0;
				}
			} 
	
			if (bird_tag[x] == 0 || bird_tag[x] > BIRDS_TAG_HIT)
			{
				if (bird_x_speed[x] == 1) {
					oam_spr(bird_x[x] + 0x08, bird_y[x] - tmp, 0x19, bird_pallete[x] - 1 | OAM_FLIP_H);
					oam_spr(bird_x[x], bird_y[x] + tmp, 0x1A + bird_sprite[x], bird_pallete[x] - 1 | OAM_FLIP_H	);
				} else {
					oam_spr(bird_x[x], bird_y[x] - tmp, 0x19, bird_pallete[x] - 1);
					oam_spr(bird_x[x] + 0x08, bird_y[x] + tmp, 0x1A + bird_sprite[x], bird_pallete[x] - 1);
				}
			}
		}
	}
}


//----UPGRADES-----------------------------------------------

void process_upgrades(void) {
	upgrade_draw_background();
	upgrade_show_ablity_desc();
	while (1) {
		nmi_wait_and_prepare();
		upgrade_draw_sprites();
		upgrade_movement_and_actions();
		if (state == 1) break;
	}
}

void upgrade_draw_background(void) {
	fade_in();
	ppu_off();
	clear_screen();
	
	//bg pallete
	pal_bg(upgrade_palette_bg);
	pal_spr(upgrade_palette_sp);

	vram_adr(PPU_PALLETE);
	for (x = 0x00; x != 0x40; ++x) {
		vram_put(0x00);
	}
	//border
	for (y = 0x02; y != 0x1B; ++y) {
		vram_adr(NAMETABLE_A + SCREEN_TILES_X * y + 0x02);
		if (y == 0x02 || y == 0x18 || y == 0x1A) {
			vram_put(CHR_BORDER_CORNER);
			for(x = 0x00; x != 0x1A; ++x) vram_put(CHR_BORDER_HORZ);
			vram_put(CHR_BORDER_CORNER);
		} else {
			vram_put(CHR_BORDER_VERT);
			vram_adr(NAMETABLE_A + SCREEN_TILES_X * y + 0x1D);
			vram_put(CHR_BORDER_VERT);
		}
	}
	
	//abilities icons and cell-values
	index = 0x00;
	adr = NAMETABLE_A + SCREEN_TILES_X * 0x04 + 0x04;
	for(x = 0x00; x != ABILITIES_COUNT; ++x) {
		tmp = player.abilities.list[x];
		upgrade_draw_icon();
		for (y = 0x00; y != ABILITY_MAX_LEVEL; ++y)
			vram_put(y < tmp ? CHR_FCELL_TOP : CHR_CELL_TOP);
		
		adr += SCREEN_TILES_X;
			
		upgrade_draw_icon();
		for (y = 0x00; y != ABILITY_MAX_LEVEL; ++y)
			vram_put(y < tmp ? CHR_FCELL_BOTTOM : CHR_CELL_BOTTOM);
			
		adr += SCREEN_TILES_X + SCREEN_TILES_X;
	}
	put_str(NTADR_A(0x03, 0x19), "                POINTS    ");
	
	for (x = 0x01; x != ABILITY_MAX_LEVEL; ++x) {
		if (player.abilities.elevel >= x) upgrade_draw_desc(x);
		else break;
	}
	
	upgrade_show_ablity_desc();
	upgrade_show_current_points();
	ppu_on_all();
	
	for (x = 0x01; x <= ABILITY_MAX_LEVEL; ++x) {
		ppu_wait_nmi();
		clear_vram_buffer();
		if (player.abilities.elevel >= x) upgrade_draw_desc(x); else break;
	}
	
	fade_out();
}

void upgrade_draw_desc(char abil_num) {
	switch (abil_num) {
		case 1: multi_vram_buffer_horz(" SMALL SCYTHE ", 0x0E, NTADR_A(0x0E, 0x04));
				multi_vram_buffer_horz(" \\ [ A        ", 0x0E, NTADR_A(0x0E, 0x05));
				break;
		case 2: multi_vram_buffer_horz(" DASH         ", 0x0E, NTADR_A(0x0E, 0x07));
				multi_vram_buffer_horz(" ^ ^ OR _ _   ", 0x0E, NTADR_A(0x0E, 0x08));
				break;
		case 3: multi_vram_buffer_horz(" ABILITIES    ", 0x0E, NTADR_A(0x0E, 0x10));
				multi_vram_buffer_horz(" POWER[       ", 0x0E,  NTADR_A(0x0E, 0x11));
				break;
		case 4: multi_vram_buffer_horz(" POWER SLASH  ", 0x0E, NTADR_A(0x0E, 0x0A));
				multi_vram_buffer_horz(" ] ] ]        ", 0x0E,  NTADR_A(0x0E, 0x0B));
				break;
		case 5: multi_vram_buffer_horz("[", 0x01,  NTADR_A(0x15, 0x11));
				if (player.abilities.list[ABILITY_MOVE_SPEED] == ABILITY_MAX_LEVEL) {
					multi_vram_buffer_horz(" MOVE AND CUT ", 0x0E,  NTADR_A(0x0E, 0x0D));
					multi_vram_buffer_horz(" ACTIVATED    ", 0x0E	,  NTADR_A(0x0E, 0x0E));
				}
				break;
	}
}

void upgrade_draw_icon(void) {
	vram_adr(adr);
	vram_put(abilities_icons[index]);
	++index;
	vram_put(abilities_icons[index]);
	++index;
	vram_put(0x00);
}



void upgrade_draw_sprites(void) {
	oam_clear();
	oam_meta_spr(0x1E, 0x1D + player.abilities.selection * 0x18, CURSOR);
}

void upgrade_movement_and_actions(void) {
	state = 0;
	if (pad1_new & PAD_DOWN) {
		sfx_play(SND_MENU_MOVE, 0);
		if (player.abilities.selection == (ABILITIES_COUNT - 1)) player.abilities.selection = 0;
			else ++player.abilities.selection;
		upgrade_show_ablity_desc();
	}
	if (pad1_new & PAD_UP) {
		sfx_play(SND_MENU_MOVE, 0);
		if (player.abilities.selection == 0) player.abilities.selection = ABILITIES_COUNT - 1;
			else --player.abilities.selection;
		upgrade_show_ablity_desc();
	}
	if (pad1_new & PAD_B) {
		if (tmp != ABILITY_MAX_LEVEL && points >= index) {
			sfx_play(SND_MENU_UPGRADE, 0);
			++tmp;
			points -= index;
			upgrade_recalc_ability_cost();
			upgrade_show_current_points();
			one_vram_buffer(CHR_FCELL_TOP, NTADR_A(0x06 + tmp, 0x04 + player.abilities.selection * 3));
			one_vram_buffer(CHR_FCELL_BOTTOM, NTADR_A(0x06 + tmp, 0x05 + player.abilities.selection * 3));
			player.abilities.list[player.abilities.selection] = tmp;
			switch(player.abilities.selection) {
				case ABILITY_CUT_SIZE: 
					++player.cut.size; 
					break;
				case ABILITY_CUT_POWER: 
					if (tmp == 1 || tmp == 3 || tmp == 5) ++player.cut.power; 
					break;
				case ABILITY_CUT_SPEED: 
					--player.cut.max_delay; 
					break;
				case ABILITY_MOVE_SPEED: 
					if (player.move_max_delay == 0) {
						if (player.move_max_increase == 0) player.move_max_increase = 3;
							else --player.move_max_increase;
						player.move_increase = player.move_max_increase;
					} else {
						--player.move_max_delay; 
					}
					if (tmp == ABILITY_MAX_LEVEL) upgrade_draw_desc(player.abilities.elevel);
					break;
				case ABILITY_SPECIAL: 
					++player.abilities.elevel;
					upgrade_draw_desc(player.abilities.elevel);
					break;
			}
		} else {
			sfx_play(SND_MENU_ERROR, 0);
		}
	}
	if (pad1_new & PAD_START) state = 1;
}	

void upgrade_show_current_points(void) {
	one_vram_buffer(CHR_ZERO + points / 10, NTADR_A(0x1A, 0x19));
	one_vram_buffer(CHR_ZERO + points % 10, NTADR_A(0x1B, 0x19));
}

void upgrade_show_ablity_desc(void) {
	adr = NTADR_A(0x04, 0x19);
	switch(player.abilities.selection) {
		case ABILITY_CUT_SIZE: multi_vram_buffer_horz("CUT WIDTH ", 0x0A, adr); break;
		case ABILITY_CUT_POWER: multi_vram_buffer_horz("CUT POWER ", 0x0A, adr); break;
		case ABILITY_CUT_SPEED: multi_vram_buffer_horz("CUT SPEED ", 0x0A, adr); break;
		case ABILITY_MOVE_SPEED: multi_vram_buffer_horz("MOVE SPEED", 0x0A, adr); break;
		case ABILITY_SPECIAL: multi_vram_buffer_horz("SPECIAL   ", 0x0A, adr); break;
	}
	tmp = player.abilities.list[player.abilities.selection];
	upgrade_recalc_ability_cost();
}

void upgrade_recalc_ability_cost(void) {
	if (tmp < 5) {
		index = upgrade_cost[player.abilities.selection * ABILITY_MAX_LEVEL + tmp];
		one_vram_buffer(CHR_ZERO + index, NTADR_A(0x0F, 0x19));
	} else {
		one_vram_buffer(CHR_BG_BLANK, NTADR_A(0x0F, 0x19));
	}
}

//-----------------------------------
//print custom string in game end credits
void process_end_game_string_print(unsigned char xpos, unsigned char ypos, const char *str, unsigned char extra_blank) {
	unsigned char npos = 0;
	if (tmp == 0) {
		if (index == 0) {
			multi_vram_buffer_horz("                                ", 0x20, NTADR_A(0, ypos));
			if (extra_blank != 0)
				multi_vram_buffer_horz("                                ", 0x20, NTADR_A(0, extra_blank));
		}
		
		if (state == EG_PRINT_DELAY) {
			state = 0;
			for (y = 0; y < index; ++y) {
				*str++;
				++xpos;
			}
			if (!*str) {
					++marker;
					index = 0;
					tmp = 60;
			} else {
				++index;
				one_vram_buffer(*str, NTADR_A(xpos, ypos));
			}
		} else {
			++state;
		}
	} else {
		--tmp;
	}
}

void process_game_win(void) {
	fade_in();
	ppu_off();

	vram_adr(NAMETABLE_A + 0x20 * 0x1C);
	for (x = 0x00; x != 0x20; ++x) vram_put(CHR_BG_BLANK);
	
	ppu_on_all();
	music_play(MSC_GAME_WIN);
	fade_out();
	x = 1;
	player.x = 50;
	player.y = PLAYER_DEF_Y;
	
	index = 0;
	state = 0;
	marker = 0;
	tmp = 200;
	while(1) {
		
		nmi_wait_and_prepare();
	
		oam_clear();
		oam_meta_spr(player.x, player.y, x == 1 ? OTM_R : OTM_L);
		player.x += x;
		if (player.x > 180) x = -1;
		if (player.x < 50) x = 1;
		if (marker < 0x0A) {
			if (tmp < 5 && marker == 0) {
				multi_vram_buffer_horz("                                ", 0x20, NTADR_A(0, 0x09 + tmp));
			}
			switch (marker) {
				case 0: process_end_game_string_print(0x09, 0x0B, "ONE THOUSAND MAN", 0); break;
				case 1: process_end_game_string_print(0x09, 0x0A, "GAME CREATED BY", 0x0B); break;
				case 2: process_end_game_string_print(0x0A, 0x0C, "SPERLINGSKAUZ", 0); break;
				case 3: process_end_game_string_print(0x08, 0x0A, "ORIGINAL COMICS BY", 0x0C); break;
				case 4: process_end_game_string_print(0x0A, 0x0C, "CYNIC MANSION", 0); break;
				case 5: process_end_game_string_print(0x08, 0x0A, "SPECIAL THANKS TO", 0x0C); break;
				case 6: process_end_game_string_print(0x0E, 0x0C, "SHIRU", 0); break;
				case 7: process_end_game_string_print(0x0D, 0x0C, "NESDOUG", 0); break;
				case 8: process_end_game_string_print(0x06, 0x0A, " ", 0x0C); break;
				case 9: process_end_game_string_print(0x06, 0x0B, "THANK YOU FOR PLAYING!", 0); break;
			}
		}
		if (pad1_new & PAD_START) break;
	}
	music_stop();
}

void process_game_end(void) {
	if (game.state == GAME_ST_LEVEL_END_WIN) process_game_win();

	pal_bg(game_palette_bg);
	pal_spr(game_palette_sp);
	fade_in();
	ppu_off();
	vram_adr(PPU_PALLETE);
	for (x = 0x00; x != 0x40; ++x) vram_put(0);
	vram_adr(NAMETABLE_A);
	for (x = 0x00; x != 0x20; ++x) 
		for(y = 0x00; y != 0x1D; ++y) 
			vram_put(CHR_BG_BLANK);
	put_str(NTADR_A(0x0B, 0x0B), "GAME OVER");
	
	put_str(NTADR_A(0x05, 0x10), "PRESS START TO RETURN");
	put_str(NTADR_A(0x09, 0x11), "TO MAIN MENU");
	
	put_str(NTADR_A(0x06, 0x0D), "FINAL SCORE");
	put_number(NTADR_A(0x11, 0x0D), score, 2);
	
	ppu_on_all();
	
	oam_clear();
	fade_out();
	
	while(1) {
		nmi_wait_and_prepare();
		if (pad1_new & PAD_START) break;
	}
	
	game.state = GAME_ST_MENU;
	
	fade_in();
	ppu_off();
	vram_adr(NAMETABLE_A + SCREEN_TILES_X * 0x0B + 0x0B);
	for(x = 0x00; x != 0x09; ++x) vram_put(CHR_BG_BLANK);
	ppu_on_all();
	fade_out();
}

//---------------------------------------------

void main (void) {
	
	while (1) {
		game_initial_values_set();
		process_title();
		if (points == 99) process_upgrades();
		process_main_game();
		process_game_end();
	}
}
