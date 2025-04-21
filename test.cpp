#include "revolver.h"
#include "player.h"
#include <iostream>

using namespace std;


int main() {
    revolver gun;
    gun.new_magazine();
    gun.printmagazine();
    // gun.num_bullets();
    // for (int i = 0; i < 6; i++) {
    //     cout << gun.shoot() << endl;
    //     gun.printmagazine();
    // }
    player p1, p2;
    p1.set_hp(3);
    p2.set_hp(3);
    p1.set_mp(3,10);
    p2.set_mp(3,10);
    p1.move(p2,gun);
    return 0;
}