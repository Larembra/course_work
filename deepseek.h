

#ifndef DEEPSEEK_H
#define DEEPSEEK_H

#include "contestant.h"
#include "revolver.h"
#include <string>

class deepseek : public contestant {
public:
    bool move(contestant& enemy, revolver& gun);
    std::vector <std::string> get_response(contestant& enemy, revolver& gun);
    std::string format_prompt(contestant& enemy, revolver& gun);
    std::string format_status(contestant& enemy, revolver& gun);
};

#endif //DEEPSEEK_H
