#include "contestant.h"

void contestant::set_hp(int Hp) {
    hp = Hp;
    max_hp = Hp;
}
void contestant::set_mp(int Mp, int max_Mp) {
    mp = Mp;
    max_mp = max_Mp;
}
int contestant::get_hp() {
    return hp;
}

int contestant::get_mp() {
    return mp;
}
void contestant::stun() {
    stunned = true;
}
void contestant::unstun() {
    stunned = false;
}