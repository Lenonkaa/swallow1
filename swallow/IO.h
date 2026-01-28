#pragma once

#include "dataTypes.h"



#define QUIT_BUTTON 'q'
#define VELOCITY_INCREASE_BUTTON 'p'
#define VELOCITY_DECREASE_BUTTON 'o'


//----------------------------------------------------------------------------------------------------------------------------------------------------------------------



// Input 
void load_config(const char* filename, gameConfig* cfg);
void input(swallow* player, gameContext* ctx);

void load_level_config(const char* filename, gameConfig* cfg);
void load_config_score(FILE* file, gameConfig* cfg, int n_parameters_toread);
void load_config_actors(FILE* file, gameConfig* cfg, int n_parameters_toread);
void load_config_stars(FILE* file, gameConfig* cfg, int n);
void load_config_swallow(FILE* file, gameConfig* cfg, int n);
void load_config_hunters(FILE* file, gameConfig* cfg, int n);

// Output