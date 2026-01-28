#pragma once

#include "dataTypes.h"


#define LOW_LIFE 25
#define MEDIUM_LIFE 50


#define COLOR_ORANGE 208


#define STATUS_HEIGHT 3
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------


void set_colors();

void draw_game(const swallow* player, const star stars[], const hunter hunters[], const gameContext* ctx, const gameConfig* cfg);

void draw_arena_borders(const gameConfig* cfg);
void draw_actors(const swallow* player, const star stars[], const hunter hunters[], const gameContext* ctx, const gameConfig* cfg);
void draw_status_bar(const swallow* player, const gameContext* ctx, const gameConfig* cfg);
void draw_swallow(const swallow* player);
void draw_stars(const star stars[], const gameConfig* cfg);
void draw_hunters(const hunter hunters[], const gameConfig* cfg);
