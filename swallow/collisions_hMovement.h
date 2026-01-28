#pragma once

#include "dataTypes.h"

#define POINTS_FOR_STAR 5
#define WAIT_TIME_BEFOR_DASH 3

// Star/hunter-swallow 


void check_star_collision(swallow* player, star stars[], const gameConfig* cfg);

void check_hunter_collision(swallow* player, hunter hunters[], const gameConfig* cfg);


// Hunters movement

void hunter_movement(int i, hunter hunters[], const swallow* player, const gameContext* ctx, const gameConfig* cfg);
void hunter_bouncing(int i, hunter hunters[], const gameContext* ctx);





void hunter_dash_logic(int i, hunter hunters[], const swallow* player, const gameContext* ctx, const gameConfig* cfg);
bool hunter_check_if_miss_swallow(const hunter* h, const swallow* p);
void hunter_perform_dash(hunter* h, const swallow* p);
