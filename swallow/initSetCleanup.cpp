
#include "initSetCleanup.h"
#include "IO.h"
#include "drawOutput.h"



//----------------------------------------------------------------------------------------------------------------------------------------------------------------------



void init_game(swallow* player, star stars[], hunter hunters[], gameContext* context, gameConfig* cfg) {
    init_screen_pdcurses();
    load_config("config.txt", cfg);

    context->level = INITIAL_LEVEL;
    set_level(player, hunters, stars, context, cfg); //level 0, init actors
    init_game_data(context, cfg);
    //init_levels(context, cfg);
    init_swallow_look(player);
}



// START GAME

void init_screen_pdcurses() {
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);

	set_colors();
}




void init_game_data(gameContext* context, gameConfig* cfg) {
    getmaxyx(stdscr, context->screen_h, context->screen_w);
    context->running = true;
    context->frame_count = 0;
    context->win = false;
    
}


void cleanup_game() {
    endwin(); //terminal back
}



//----------------------------------------------------------------------------------------------------------------------------------------------------------------------


// LEVELS


bool next_level_check(swallow* player, gameContext* context, const gameConfig* cfg) {
    if (player->stars_collected >= cfg->star_win){
        player->score += (context->level) * LEVEL_BONUS_MULTIPLIER; //level bonus, level alredy increased, but levels start from 0
        player->score += player->life; //bonus for life left
        return true;
    }
    else
        return false;
}

void set_level(swallow* player, hunter hunters[], star stars[], gameContext* context, gameConfig* cfg) {

    int level = context->level + 1;
    char filename[50];

    sprintf(filename, "level%d.txt", level);

    player->stars_collected = 0; //reset for next level
	
    context->frame_count = 0;
    
    load_level_config(filename, cfg);

    context->gameAreaSize.height = cfg->map.height;
    context->gameAreaSize.width = cfg->map.width;

    init_hunters(hunters, cfg); //reset hunters
    init_stars(stars);
	init_swallow(player, context); // now life=100, stars_collected=0, initial position


}





//----------------------------------------------------------------------------------------------------------------------------------------------------------------------



// Actors' initialization

void init_swallow(swallow* player, const gameContext* context) {

    player->pos.x = 10.0f;
    player->pos.y = context->screen_h / 2.0f;
    player->pos.vx = SWALLOW_INITIAL_MOMENTUM_X;
    player->pos.vy = SWALLOW_INITIAL_MOMENTUM_Y;
    player->life = 100; //100%
    player->stars_collected = 0;


    if (context->level == 0) {
        player->speed_lvl = DEFAULT_SWALLOW_SPEED_LEVEL;
        player->score = 0;
    }


}

void init_swallow_look(swallow* player) {
    // symbol
    player->size.width = SWALLOW_SHAPE_WIDTH;
    player->size.height = SWALLOW_SHAPE_LENGTH;

    // symbol into array
    //strcpy(player->shape[0], "SWALLOW_SHAPE");
    strcpy(player->shape[0], SWALLOW_SHAPE);


}

void init_stars(star* stars) {
    for (int i = 0; i < LIMIT_STARS; i++) {
        stars[i].active = false;
    }
}


void reset_hunter(int i, hunter hunters[], const gameConfig* cfg) {
    hunters[i].bounces_left = cfg->hunter_bounces;
    hunters[i].damage = cfg->damage_value;
    hunters[i].speed = cfg->hunter_speed;
}

void init_hunters(hunter* hunters, const gameConfig* cfg) {
    for (int i = 0; i < LIMIT_HUNTERS; i++) {
        hunters[i].active = false;
        reset_hunter(i, hunters, cfg);
    }
}

