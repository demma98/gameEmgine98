#ifndef LIVING_GUARD
#include "living.h"
#endif

#define PLAYER_GUARD 1

class Player : public Living{
    public:
        Player(int x, int y, int id);
        void Specific_update(Table table);
};