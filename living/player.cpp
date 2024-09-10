#include "player.h"

Player :: Player(int x, int y, int id){
    this->x = (double)x;
    this->y = (double)y;
    this->id = id;
    
    make();
}

void Player :: Specific_update(Table table){
    const Uint8* state = SDL_GetKeyboardState(nullptr);
        if(state[SDL_SCANCODE_RIGHT]){
            if(speed_x < 2.1)
                speed_x += 0.5;
        }
        else{
            if(speed_x > 0)
                if(speed_x > 0.5)
                    speed_x -= 0.5;
                else
                    speed_x = 0;
        }
        if(state[SDL_SCANCODE_LEFT]){
            if(speed_x > -2.1)
                speed_x -= 0.5;
        }
        else{
            if(speed_x < 0)
                if(speed_x < -0.5)
                    speed_x += 0.5;
                else
                    speed_x = 0;
        }
        if(state[SDL_SCANCODE_UP]){
            if(jumping == false && (distanceDown(table) == 0.0 || coyote_time > 0)){
                jump = true;
                speed_y -= 4.7;
            }
            if(jump){
                speed_y -= 0.5;
            }
            jumping = true;
        }
        else{
            jumping = false;
            jump = false;
        }
}