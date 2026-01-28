#include "drawOutput.h"




void set_colors() {

    start_color();

    //(text,background)
    init_pair(C_DEFAULT, COLOR_WHITE, COLOR_BLACK);
    init_pair(C_STATUS_BAR, COLOR_BLACK, COLOR_WHITE);
    init_pair(C_ARENA_BORDERS, COLOR_WHITE, COLOR_BLACK);

    init_pair(C_SWALLOW_HIGH, COLOR_GREEN, COLOR_BLACK);
    init_pair(C_SWALLOW_MEDIUM, COLOR_YELLOW, COLOR_BLACK);
    init_pair(C_SWALLOW_LOW, COLOR_RED, COLOR_BLACK);

    init_pair(C_STAR, COLOR_YELLOW, COLOR_BLACK);

    init_pair(C_HUNTER_1, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(C_HUNTER_2, COLOR_RED, COLOR_BLACK);
    init_pair(C_HUNTER_3, COLOR_ORANGE, COLOR_BLACK);

    init_pair(C_TAXI, COLOR_CYAN, COLOR_BLACK);
}





void draw_game(const swallow* player, const star stars[], const hunter hunters[], const gameContext* ctx, const gameConfig* cfg) {
    erase();

    draw_arena_borders(cfg);
    draw_actors(player, stars, hunters, ctx, cfg);

    draw_status_bar(player, ctx, cfg);

    refresh();
}


// drawOutput.cpp

// Funkcja rysująca granice areny
void draw_arena_borders(const gameConfig* cfg) {
    int width = cfg->map.width+1;
    int height = cfg->map.height;

    attron(COLOR_PAIR(C_ARENA_BORDERS));
    //poziome
    //mvhline(0, 0, ACS_HLINE, width);          // top
    mvhline(height , 0, ACS_HLINE, width); // bottom

    //pionowe
    //mvvline(0, 0, ACS_VLINE, height);           // left
    mvvline(0, width - 1, ACS_VLINE, height);   // right

    // narozniki
   // mvaddch(0, 0, ACS_ULCORNER);                    // Upper left
   //mvaddch(0, width-1, ACS_URCORNER);            // Upper right
   //mvaddch(height, 0, ACS_LLCORNER);           // Lower left
    mvaddch(height, width - 1, ACS_LRCORNER);   // Lower right


    attroff(COLOR_PAIR(C_ARENA_BORDERS));
}


void draw_actors(const swallow* player, const star stars[], const hunter hunters[], const gameContext* ctx, const gameConfig* cfg) {
    draw_stars(stars, cfg);
    draw_swallow(player);
    draw_hunters(hunters, cfg);

}

void draw_status_bar(const swallow* player, const gameContext* ctx, const gameConfig* cfg) {
    
    
	// status bar - look


    int status_y = ctx->screen_h - STATUS_HEIGHT;

    // line
    mvhline(status_y, 0, ACS_HLINE, ctx->screen_w);

    // background
    attron(COLOR_PAIR(C_STATUS_BAR));
    for (int y = status_y + 1; y < ctx->screen_h; y++) {
        mvhline(y, 0, ' ', ctx->screen_w);
    }

    //status bar - game statistics
    mvprintw(status_y + 1, 2, "PLAYER: SWALLOW-1");
    mvprintw(status_y + 1, 30, "LIFE FORCE: %d%%", player->life);
    mvprintw(status_y + 1, 60, "SPEED LEVEL: %d/%d", player->speed_lvl, NUMBER_SPEED_LEVELS);
    mvprintw(status_y + 1, 90, "HUNTERS: %d", cfg->max_hunters);
    mvprintw(status_y + 2, 2, "LEVEL: %d/%d", ctx->level + 1, NUMBER_LEVELS);
    mvprintw(status_y + 2, 30, "STARS: %d/%d", player->stars_collected, cfg->star_win);
    mvprintw(status_y + 2, 60, "SCORE: %d", player->score);
    mvprintw(status_y + 2, 90, "TIME LEFT: %.1f s", ctx->time_left);
    attroff(COLOR_PAIR(C_STATUS_BAR));
}



void draw_stars(const star stars[], const gameConfig* cfg) {
    attron(COLOR_PAIR(C_STAR) | A_BOLD);
    for (int i = 0; i < cfg->max_stars; i++) {
        if (stars[i].active) {
            // int for cell
            mvaddch((int)stars[i].pos.y, (int)stars[i].pos.x, '*');
        }
    }
    attroff(COLOR_PAIR(C_STAR) | A_BOLD);
}

void draw_swallow(const swallow* player) {
    int current_swallow_color = C_SWALLOW_HIGH;
    if (player->life <= LOW_LIFE) current_swallow_color = C_SWALLOW_LOW;
    else if (player->life <= MEDIUM_LIFE) current_swallow_color = C_SWALLOW_MEDIUM;

    attron(COLOR_PAIR(current_swallow_color) | A_BOLD);
    for (int y = 0; y < player->size.height; y++) {
        for (int x = 0; x < player->size.width; x++) {

            if (player->shape[y][x] != '\0') {
                mvaddch((int)player->pos.y + y, (int)player->pos.x + x, player->shape[y][x]);
            }
        }
    }
    attroff(COLOR_PAIR(current_swallow_color) | A_BOLD);
}


void draw_hunters(const hunter hunters[], const gameConfig* cfg) {
    for (int i = 0; i < cfg->max_hunters; i++) {
        if (hunters[i].active) {

			attron(COLOR_PAIR(hunters[i].color_pair)); //| A_BOLD) - ale są jaśniejsze;

            for (int y = 0; y < hunters[i].size.height; y++) {
                for (int x = 0; x < hunters[i].size.width; x++) {
                    if (hunters[i].shape[y][x] != '\0') {
                        mvaddch((int)hunters[i].pos.y + y, (int)hunters[i].pos.x + x, hunters[i].shape[y][x]);
                    }
                }
            }
            mvprintw((int)hunters[i].pos.y + hunters[i].size.height, (int)hunters[i].pos.x, "%d", hunters[i].bounces_left); // show bounces below hunter

            attroff(COLOR_PAIR(hunters[i].color_pair) | A_BOLD);
        }
    }
}
