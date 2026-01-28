#include "collisions_hMovement.h"




// STAR - SWALLOW collisions

void check_star_collision(swallow* player, star stars[], const gameConfig* cfg) {

    for (int i = 0; i < cfg->max_stars; i++) {
        if (stars[i].active) {
            int sx = (int)stars[i].pos.x;
            int sy = (int)stars[i].pos.y;

            int px = (int)player->pos.x;
            int py = (int)player->pos.y;

            // star (s) inside swallow (p - player) - left/right and top/below
            if (sx >= px && sx <= px + player->size.width &&
                sy >= py && sy <= py + player->size.height) {

                player->stars_collected++;
				player->score += POINTS_FOR_STAR;
                stars[i].active = false;
            }

        }
    }
}


// HUNTER - SWALLOW collisons -- do ulepszenia


void check_hunter_collision(swallow* player, hunter hunters[], const gameConfig* cfg) {

    for (int i = 0; i < cfg->current_max_hunters; i++) {
        if (hunters[i].active) {

            // swallow (p - player) borders
            int pLeft = (int)player->pos.x;
            int pRight = pLeft + player->size.width;
            int pTop = (int)player->pos.y;
            int pBottom = pTop + player->size.height;

            // hunter borders
            int hLeft = (int)hunters[i].pos.x;
            int hRight = hLeft + hunters[i].size.width;
            int hTop = (int)hunters[i].pos.y;
            int hBottom = hTop + hunters[i].size.height;

            // collision - checking if there is distant between swallow and hunter borders - overlap
            if (pLeft < hRight &&
                pRight > hLeft &&
                pTop < hBottom &&
                pBottom > hTop) {

                player->life -= hunters[i].damage; // damage
                hunters[i].active = false; //disappear after collision

                if (player->life < 0) player->life = 0;
            }

        }
    }
}



// HUNTERS Movement --- do ulepszenia


void hunter_movement(int i, hunter hunters[], const swallow* player, const gameContext* ctx, const gameConfig* cfg) {

    hunter_dash_logic(i, hunters, player, ctx, cfg);
    // const vector
    hunters[i].pos.x += hunters[i].pos.vx;
    hunters[i].pos.y += hunters[i].pos.vy;

    hunter_bouncing(i, hunters, ctx);
    
}




void hunter_bouncing(int i, hunter hunters[], const gameContext* ctx) {


    int max_w = ctx->gameAreaSize.width;
    int max_h = ctx->gameAreaSize.height;
   
    bool bounced = false;


    //zamienic w funkcje z parametrem - bo to samo prawie dla x i y

    // left/right
    if (hunters[i].pos.x <= 0 || hunters[i].pos.x >= max_w - hunters[i].size.width) {
        hunters[i].pos.vx *= -1;

        //wall protection
        if (hunters[i].pos.x <= 0) hunters[i].pos.x = 1;
        if (hunters[i].pos.x >= max_w - hunters[i].size.width) hunters[i].pos.x = max_w - hunters[i].size.width - 1;
        bounced = true;
    }

    // top/bottom
    if (hunters[i].pos.y <= 0 || hunters[i].pos.y >= max_h - hunters[i].size.height) {
        hunters[i].pos.vy *= -1;

        //wall protection
        if (hunters[i].pos.y <= 0) hunters[i].pos.y = 1;
        if (hunters[i].pos.y >= max_h - hunters[i].size.height) hunters[i].pos.y = max_h - hunters[i].size.height - 1;
        bounced = true;
    }


    if (bounced) {
        hunters[i].bounces_left--;
        // disappear after bounces
        if (hunters[i].bounces_left <= 0) hunters[i].active = false;

    }


}



// HUNTER - DASH

void hunter_dash_logic(int i, hunter hunters[], const swallow* player, const gameContext* ctx, const gameConfig* cfg) {


    if (hunters[i].wait_before_dash > 0) {
        hunters[i].wait_before_dash--;

        if (hunters[i].wait_before_dash == 0) {
            hunter_perform_dash(&hunters[i], player);
        }

        return;
    }



    if (!hunters[i].has_dashed) {

        if (hunter_check_if_miss_swallow(&hunters[i], player)) {

            hunters[i].wait_before_dash = WAIT_TIME_BEFOR_DASH * 1000 / cfg->game_speed;
            hunters[i].pos.vx = 0;
            hunters[i].pos.vy = 0;
            return;
        }
    }
}



void hunter_perform_dash(hunter* h, const swallow* p) {

    float vector_position_difference_x = p->pos.x - h->pos.x;
    float vector_position_difference_y = p->pos.y - h->pos.y;

    float distance = sqrtf(vector_position_difference_x * vector_position_difference_x + vector_position_difference_y * vector_position_difference_y);

    if (distance > 0) {

        float direction_vector_x = vector_position_difference_x / distance;
        float direction_vector_y = vector_position_difference_y / distance;

        h->pos.vx = direction_vector_x * h->dash_speed;
        h->pos.vy = direction_vector_y * h->dash_speed;
        h->has_dashed = true;
    }

}


bool hunter_check_if_miss_swallow(const hunter* h, const swallow* p) {
    //from hunter to swallow
    float vector_position_difference_x = p->pos.x - h->pos.x;
    float vector_position_difference_y = p->pos.y - h->pos.y;

    float hunter_speed_vector_magnitude = sqrtf(h->pos.vx * h->pos.vx + h->pos.vy * h->pos.vy);
    if (hunter_speed_vector_magnitude <= 0) return false;


    float area_parallelogram_on_vectors = fabsf(vector_position_difference_x * h->pos.vy - vector_position_difference_y * h->pos.vx); //cross product
    float swallow_shortest_distance_to_hunter_path = area_parallelogram_on_vectors / hunter_speed_vector_magnitude; //perpendicular distance = area / base

    if (swallow_shortest_distance_to_hunter_path > (float)p->size.width) {
        return true; // miss swallow
    }

    return false;

}