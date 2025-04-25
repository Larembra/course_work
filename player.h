#ifndef PLAYER_H
#define PLAYER_H

#include "contestant.h"
#include "revolver.h"
#include <string>

class player : public contestant {
    public:
    bool move(contestant& enemy, revolver& gun);
    std::string format_status(contestant& enemy, revolver& gun);
};



#endif //PLAYER_H
