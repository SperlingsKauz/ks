struct StGrass {
	unsigned char delay;
	unsigned char delay_max;
	unsigned char instant_count;
	unsigned char col_for_point;
	unsigned char height[GRASS_WIDTH];
};

struct StEffects {
	unsigned char lightning;
	unsigned char lightning_enabled;
	unsigned char wind;
};

struct StGame {
	unsigned char level;
	unsigned char last_level;
	unsigned char paused;
	unsigned char state;
	unsigned char play_time;
	unsigned char max_play_time;
	unsigned char difficulty;
	unsigned char lives;
	struct StEffects effects;
};

struct StCut {
	unsigned char power;
	unsigned char size;
	unsigned char state;
	unsigned char delay;
	unsigned char max_delay;
	unsigned char temp_store;
	unsigned char count;
};

struct StAbilities {
	unsigned char selection;
	unsigned char list[ABILITIES_COUNT];
	unsigned char elevel;
};

struct StSmallScythe {
	unsigned char x;
	unsigned char y;
	unsigned char width;
	unsigned char height;
	unsigned char sprite;
	unsigned char state;
	unsigned char x_speed;
	unsigned char y_speed;
};

struct StSpecials {
	unsigned char delayed;
	unsigned char active[SP_COUNT];
	unsigned char state[SP_COUNT];
};

struct StPlayer {
	unsigned char x;
	unsigned char y;
	unsigned char width;
	unsigned char height;
	unsigned char move_delay;
	unsigned char move_max_delay;
	unsigned char move_increase;
	unsigned char move_max_increase;
	unsigned char direction;
	struct StCut cut;
	struct StAbilities abilities;
	struct StSpecials specials;
};

struct StPoint {
	unsigned char x;
	unsigned char y;
};

struct BoxUnit {
	unsigned char x;
	unsigned char y;
	unsigned char width;
	unsigned char height;
	unsigned char sprite;
	unsigned char state;
};


void nmi_wait_and_prepare(void);
void clear_screen(void);
void fade_in(void);
void fade_out(void);
void delay_with_state_skip(unsigned char delay_length);
void put_str(unsigned int adr, const char *str);
void put_char(unsigned int address, const char *str, unsigned char num);
void put_number(unsigned int address, unsigned int number, unsigned char zero_count);
	
void game_initial_values_set(void);
void game_process_next_level(void);
void game_grass_grow(void);
void game_grass_cf_prepare(void);
void game_grass_cf_finalize(void);
void game_grass_fading(void);
void game_grass_overgrow(void);
void game_grass_cut(void);
void game_check_for_extra_point(void);
void game_draw_rain(void);
void game_draw_sprites(void);
void game_draw_background(void);
void game_draw_time(void);
void game_draw_points(void);
void game_draw_effects(void);
void game_movement_and_actions(void);	
void game_check_specials_keys(void);
void game_small_scythe_processing(void);
void game_create_enemy(char bird_num);
void game_enemy_processing(void);

void upgrade_draw_sprites(void);
void upgrade_draw_background(void);
void upgrade_movement_and_actions(void);	
void upgrade_draw_desc(char abil_num);
void upgrade_draw_icon(void);
void upgrade_recalc_ability_cost(void);
void upgrade_show_ablity_desc(void);
void upgrade_show_current_points(void);

void title_draw_background(void);
void title_draw_options(void);
void title_draw_menu(void);
void title_movement_and_actions(void);
void title_check_cheat_code(void);

void process_title(void);
void process_upgrades(void);
void process_main_game(void);
void process_end_game_string_print(unsigned char xpos, unsigned char ypos, const char *str, unsigned char extra_blank) ;
void process_game_win(void);
void process_game_end(void);

void d();
void d2();
void tasm(void);
void set_breakpoint(void);

void operate_collisions(void);

void upgrade_draw_background(void);