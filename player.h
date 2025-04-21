#ifndef PLAYER_H
#define PLAYER_H

#include "contestant.h"
#include "revolver.h"

class player : public contestant {
    public:
    bool move(contestant& enemy, revolver& gun);
};



#endif //PLAYER_H
