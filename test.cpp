#include "revolver.h"
#include "player.h"
#include "deepseek.h"
#include <iostream>

using namespace std;


int main() {
    string chosen;
    bool result = 0; //определяет, чей ход будет следующим
    vector <string> enemies = {"DeepSeek"};
    cout << "Choose your enemy:" << endl;
    for (auto i : enemies) {
        cout << i << endl;
    }
    cin >> chosen;
    if (chosen == "DeepSeek") {
        revolver gun;
        gun.new_magazine();
        gun.printmagazine();

        player p1;
        p1.set_max_hp(3);
        p1.set_mp(3,10);
        deepseek d;
        d.set_max_hp(3);
        d.set_mp(3,10);
        while (p1.get_hp() != 0 and d.get_hp() != 0) {
            if (result == 0) {
                if (!p1.is_stunned()) {
                    cout << "p1, your turn!" << endl;
                    result = p1.move(d, gun);
                }
                else {
                    cout << "enemy turn!" << endl;
                    result = d.move(p1, gun);
                }
            }
            if (result == 1) {
                if (!d.is_stunned()) {
                    cout << "enemy turn!" << endl;
                    result = d.move(p1, gun);
                }
                else {
                    cout << "p1, your turn!" << endl;
                    result = p1.move(d, gun);
                }
            }
        }
        if (p1.get_hp() == 0) {
            cout << "You lose!" << endl;
        }
        else {
            cout << "You win!" << endl;
        }

    }
    // revolver gun;
    // gun.new_magazine();
    // gun.printmagazine();
    // gun.num_bullets();
    // for (int i = 0; i < 6; i++) {
    //     cout << gun.shoot() << endl;
    //     gun.printmagazine();
    // }

    // player p1, p2;
    // p1.set_hp(3);
    // p2.set_hp(3);
    // p1.set_mp(3,10);
    // p2.set_mp(3,10);
    // p1.move(p2,gun);

    // player p1;
    // p1.set_hp(3);
    // p1.set_mp(3,10);
    //
    // cout << "Choose your enemy:" << endl;
    // deepseek d;
    // d.set_hp(3);
    // d.set_mp(3,10);
    // vector <string> acts = d.get_response(p1, gun);
    // // for (auto a : acts) {
    // //     cout << a << endl;
    // // }
    // cout << d.move(p1, gun);
    return 0;
}