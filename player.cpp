#include "player.h"
#include "revolver.h"
#include <iostream>

using namespace std;

bool player::move(contestant& enemy, revolver& gun) {
    if (mp < max_mp) {
        mp++;
    }
    cout << "your hp: " << hp << endl;
    cout << "your mp: " << mp << endl;
    cout << endl;
    cout << "enemy hp: " << enemy.get_hp() << endl;
    cout << "enemy mp: " << enemy.get_mp() << endl;
    cout << endl;
    std::cout << "bullets: " << gun.num_bullets() << std::endl;
    std::cout << "chambers: " << gun.num_chambers() << std::endl;
    int damage = 1;
    string act = "";
    cin >> act;
    while (act != "shootenemy" and act != "shootmyself") {
        if (act == "enhancedshot") {
            if (mp >= 3) {
                mp -= 3;
                damage = 2;
                cout << "spell was casted" << endl;
                cout << "your mp: " << mp << endl;
            }
            else {
                cout << "not enough mana" << endl;
                cout << "your mp: " << mp << ", need 3" << endl;
            }
        }
        else if (act == "stun") {
            if (mp >= 5) {
                mp -= 5;
                enemy.stun();
                cout << "spell was casted" << endl;
                cout << "your mp: " << mp << endl;
            }
            else {
                cout << "not enough mana" << endl;
                cout << "your mp: " << mp << ", need 5" << endl;
            }
        }
        else if (act == "heal") {
            if (mp >= 3 and hp < max_hp) {
                mp -= 3;
                hp++;
                cout << "spell was casted" << endl;
                cout << "your mp: " << mp << endl;
                cout << "your hp: " << hp << endl;
            }
            else if (hp == max_hp) {
                cout << "you already have max health" << endl;
            }
            else {
                cout << "not enough mana" << endl;
                cout << "your mp: " << mp << ", need 3" << endl;
            }
        }
        else if (act == "swapbullet") {
            if (mp >= 3) {
                mp -= 3;
                gun.swap_bullet();
                cout << "spell was casted" << endl;
                cout << "your mp: " << mp << endl;
            }
            else {
                cout << "not enough mana" << endl;
                cout << "your mp: " << mp << ", need 3" << endl;
            }
        }
        else if (act == "fireball") {
            if (mp >= 5) {
                mp -= 5;
                enemy.set_hp(enemy.get_hp() - 1);
                if (enemy.get_hp() == 0) {
                    return 0;
                }
                cout << "spell was casted" << endl;
                cout << "your mp: " << mp << endl;
                cout << "enemy hp: " << enemy.get_hp() << endl;
            }
            else {
                cout << "not enough mana" << endl;
                cout << "your mp: " << mp << ", need 5" << endl;
            }
        }
        else if (act == "bloodritual") {
            if (hp > 1) {
                hp--;
                if (mp+2 > max_mp) {
                    mp = max_mp;
                }
                else {
                    mp+=2;
                }
                cout << "spell was casted" << endl;
                cout << "your hp: " << hp << endl;
                cout << "your mp: " << mp << endl;
            }
            else {
                cout << "not enough health" << endl;
                cout << "your hp: " << hp << endl;
            }
        }
        else {
            cout << "unknown action" << endl;
        }
        cin >> act;
    }
    if (act == "shootenemy") {
        if (gun.shoot()) {
            enemy.set_hp(enemy.get_hp() - damage);
        }
        cout << "enemy hp: " << enemy.get_hp() << endl;
        return 1;
    }
    else if (act == "shootmyself") {
        if (!gun.shoot()) {
            return 0;
        }
        hp-=damage;
        cout << "your hp: " << hp << endl;
        return 1;
    }
}