#pragma once

#include "dataTypes.h"

#define POINTS_FOR_STAR 5

// Star/hunter-swallow 


void check_star_collision(swallow* player, star stars[], const gameConfig* cfg);

void check_hunter_collision(swallow* player, hunter hunters[], const gameConfig* cfg);


// Hunters movement

void hunter_movement(int i, hunter hunters[], const swallow* player, const gameContext* ctx);
void hunter_bouncing(int i, hunter hunters[], const gameContext* ctx);