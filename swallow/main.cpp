// swallow.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "dataTypes.h"
#include "drawOutput.h"
#include "initSetCleanup.h"
#include "IO.h"
#include "updateActors.h"
#include "collisions_hMovement.h"

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------

void game_running(swallow* player, hunter hunters[], star stars[], gameContext* context, gameConfig* cfg);

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------


// MAIN

int main() {

    srand((unsigned int)time(NULL));

   // gameConfig cfg;
    gameConfig cfg = { 0 };
    gameContext context;
    swallow player;
    star stars[LIMIT_STARS];
    hunter hunters[LIMIT_HUNTERS];



    

    init_game(&player, stars, hunters, &context, &cfg);
    
    
    game_running(&player, hunters, stars, &context, &cfg);

    cleanup_game();

	//display final score
    if (context.win)
        printf("Game Over! You won! Your level is: %d\n", context.level);
	else
        printf("Game Over! You lost! Your score is: %d\n", player.score);

    return 0;


}



//----------------------------------------------------------------------------------------------------------------------------------------------------------------------







void game_running(swallow* player, hunter hunters[], star stars[], gameContext* context, gameConfig* cfg) {

    while (context->running) {

        input(player, context);

        actors_movement_and_collisions(player, hunters, stars, context, cfg);

        context->frame_count++;

        context->world_time = context->frame_count * (cfg->game_speed / 1000.0f); //game speed in ms : 1000 = seconds

        context->time_left = cfg->time_limit - context->world_time;

        // checking life, time and score - when to end the game

        if (next_level_check(player, context, cfg)) {
            context->level++;
            if (context->level < NUMBER_LEVELS) {
                set_level(player, hunters, stars, context, cfg);
            }
            else {
                context->win = true;
                context->running = false;
            }
        }


        if (player->life <= 0 || context->time_left<=0) {
            context->running = false;

        }


        draw_game(player, stars, hunters, context, cfg);

        // time control
        napms(cfg->game_speed); // wait x miliseconds - bład, czeka np 20ms, ale minal tez czas na wykonanie ietarcji petli - POPRAWIC CZAS
    }

}

