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

    for (int i = 0; i < cfg->max_hunters; i++) {
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


void hunter_movement(int i, hunter hunters[], const swallow* player, const gameContext* ctx) {


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