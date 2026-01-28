#include "IO.h"




// Input / Output

void load_config(const char* filename, gameConfig* cfg) {
    FILE* file = fopen(filename, "r");
    if (!file) return;

    char key[50];
    float value;

    while (fscanf(file, "%s %f", key, &value) != EOF) {
        if (strcmp(key, "MAX_STARS") == 0) cfg->max_stars = (int)value;
        else if (strcmp(key, "GAME_SPEED_MS") == 0) cfg->game_speed = (int)value;
        else if (strcmp(key, "DAMAGE_VALUE") == 0) cfg->damage_value = (int)value;
        else if (strcmp(key, "DAMAGE_DIFFERENCE") == 0) cfg->damage_diff = (int)value;
        else if (strcmp(key, "HUNTERS_STANDARD_SPEED") == 0) cfg->hunter_speed = value;
        else if (strcmp(key, "HUNTERS_STANDARD_BOUNCES") == 0) cfg->hunter_bounces = (int)value;
        else if (strcmp(key, "HUNTERS_SPEED_DIFFERENCE") == 0) cfg->hunter_speed_diff = value;
        else if (strcmp(key, "HUNTERS_BOUNCES_DIFFERENCE") == 0) cfg->hunter_bounces_diff = (int)value;
        else if (strcmp(key, "HUNTERS_SPEED_LEVEL_DIFF") == 0) cfg->hunter_speed_lvl_diff = value;
        else if (strcmp(key, "STARS_TO_WIN") == 0) cfg->star_win = (int)value;
        else if (strcmp(key, "STARS_TO_WIN_LEVEL_DIFF") == 0) cfg->star_win_diff = (int)value;



    }
    fclose(file);
}




void input(swallow* player, gameContext* ctx) {
    int ch = getch();

    if (ch == ERR) return; // no key pressed


    switch (ch) {
		// q for quit
    case QUIT_BUTTON: ctx->running = false; break; 

        // change in velocity vector
    case 'w': player->pos.vy = UP; player->pos.vx = 0; break;
    case 's': player->pos.vy = DOWN; player->pos.vx = 0; break;
    case 'a': player->pos.vx = LEFT; player->pos.vy = 0; break;
    case 'd': player->pos.vx = RIGHT; player->pos.vy = 0; break;

        // o-p velocity change
    case VELOCITY_DECREASE_BUTTON: if (player->speed_lvl > 1) player->speed_lvl--; break;
    case VELOCITY_INCREASE_BUTTON: if (player->speed_lvl < NUMBER_SPEED_LEVELS) player->speed_lvl++; break;

    }
}

void load_level_config(const char* filename, gameConfig* cfg) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Blad: Nie mozna otworzyc pliku %s\n", filename);
        return;
    }

    char key[50];
    float value;

    while (fscanf(file, "%s %f", key, &value) != EOF) {
        if (strcmp(key, "MAP_WIDTH") == 0) cfg->map.width = (int)value;
        else if (strcmp(key, "MAP_HEIGHT") == 0) cfg->map.height = (int)value;
        else if (strcmp(key, "STARS_TO_COLLECT") == 0) cfg->star_win = (int)value;

        else if (strcmp(key, "TIME_LIMIT") == 0) cfg->time_limit = (int)value;


        else if (strcmp(key, "---SCORE---") == 0) {
            load_config_score(file, cfg, (int)value);
        }

        else if (strcmp(key, "---ACTORS---") == 0) {
            load_config_actors(file, cfg, (int)value);
        }




    }
    fclose(file);
}

void load_config_score(FILE* file, gameConfig* cfg, int n_parameters_toread) {
    char key[50];
    float value;
    if (!file) {
        printf("Blad: Nie mozna otworzyc pliku %s\n", file);
        return;
    }
    for (int i = 0; i < n_parameters_toread; i++) {
        if (fscanf(file, "%s %f", key, &value) != EOF) {
            if (strcmp(key, "SCORE_STAR") == 0) cfg->score.star = (int)value;
            else if (strcmp(key, "SCORE_LIFE") == 0) cfg->score.life = (int)value;
            else if (strcmp(key, "SCORE_TIME") == 0) cfg->score.time = (int)value;
        }
    }
}



void load_config_actors(FILE* file, gameConfig* cfg, int n_parameters_toread) {
    char key[50];
    float value;
    for (int i = 0; i < n_parameters_toread; i++) {
        if (fscanf(file, "%s %f", key, &value) != EOF) {
            if (strcmp(key, "~~~STARS~~~") == 0) {
                load_config_stars(file, cfg, (int)value);
            }
            else if (strcmp(key, "~~~SWALLOW~~~") == 0) {
                load_config_swallow(file, cfg, (int)value);
            } 
            else if (strcmp(key, "~~~HUNTERS~~~") == 0) {
                load_config_hunters(file, cfg, (int)value);
            }
        }
    }


}


void load_config_stars(FILE* file, gameConfig* cfg, int n) {
    char key[50];
    float value;
    for (int i = 0; i < n; i++) {
        if (fscanf(file, "%s %f", key, &value) != EOF) {
            if (strcmp(key, "SPAWN_RATE_STARS") == 0) cfg->spawn_probability_stars = static_cast<float>(value);
        }
    }
}


void load_config_swallow(FILE* file, gameConfig* cfg, int n) {
    char key[50];
    float value;
    for (int i = 0; i < n; i++) {
        if (fscanf(file, "%s %f", key, &value) != EOF) {
            if (strcmp(key, "MIN_SPEED") == 0) cfg->swallow_speed_min = static_cast<float>(value);
            else if (strcmp(key, "MAX_SPEED") == 0) cfg->swallow_speed_max = static_cast<float>(value);
        }
    }
    cfg->swallow_speed_diff = (cfg->swallow_speed_max - cfg->swallow_speed_min) / NUMBER_SPEED_LEVELS;
}

void load_config_hunters(FILE* file, gameConfig* cfg, int n) {
    char key[50];
    float value;
    for (int i = 0; i < n; i++) {
        if (fscanf(file, "%s %f", key, &value) != EOF) {
            if (strcmp(key, "SPAWN_RATE_HUNTERS") == 0) {
                cfg->spawn_probability_hunters = static_cast<float>(value);
            }
            else if (strcmp(key, "MAX_HUNTERS") == 0) {
                cfg->max_hunters = (int)value;
            }
            else if (strcmp(key, "ESCALATION_TIME") == 0) cfg->escalation_interval_seconds = (int)value;
            else if (strcmp(key, "ESCALATION_HUNTERS") == 0) cfg->escalation_max_hunters = (int)value;
            else if (strcmp(key, "ESCALATION_BOUNCES") == 0) cfg->escalation_hunters_bounces = (int)value;

            else if (strcmp(key, "TYPE") == 0) {
                int type = (int)value; // Typ 0, 1 lub 2 (H_STANDARD, H_BIG, H_FAST)

                for (int j = 0; j < 6; j++) {
                    if (fscanf(file, "%s %f", key, &value) != EOF) {
                        if (strcmp(key, "ALLOWED") == 0)
                            cfg->hunter_types[type].type_allowed = (int)value;
                        else if (strcmp(key, "SPEED") == 0)
                            cfg->hunter_types[type].speed = static_cast<float>(value);
                        else if (strcmp(key, "BOUNCES_LIMIT") == 0)
                            cfg->hunter_types[type].hunter_bounces = (int)value;
                        else if (strcmp(key, "WIDTH") == 0)
                            cfg->hunter_types[type].size.width = (int)value;
                        else if (strcmp(key, "HEIGHT") == 0)
                            cfg->hunter_types[type].size.height = (int)value;
                        else if (strcmp(key, "COLOR") == 0)
                            cfg->hunter_types[type].color_pair = (int)value;
                    }
                }
            }
        }
    }
}