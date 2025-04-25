#include "player.h"
#include "revolver.h"
#include <iostream>
#include "telegram.h"

using namespace std;

const std::string TELEGRAM_TOKEN = "7928899642:AAGmfMYq3LmjwgySM9lx0heY51Ov6sbscko";
const int64_t CHAT_ID = 1508659574;
const std::vector<std::vector<std::string>> acts = {{"Enhanced shot", "Stun", "Heal"}, {"Swap bullet", "Fireball", "Blood ritual"}, {"Shoot myself", "Shoot enemy"}};


std::string player::format_status(contestant& enemy, revolver& gun) {
    //std::ostringstream oss;
    string msg = R"(Your HP: {MY_HP} | MP: {MY_MP}
Opponent HP: {ENEMY_HP} | MP: {ENEMY_MP}
Remaining chambers: {CHAMBERS}
Remaining bullets: {BULLETS})";

    // Заменяем плейсхолдеры на фактические значения
    size_t pos;
    while ((pos = msg.find("{MY_HP}")) != std::string::npos)
        msg.replace(pos, 7, std::to_string(hp));
    while ((pos = msg.find("{MY_MP}")) != std::string::npos)
        msg.replace(pos, 7, std::to_string(mp));
    while ((pos = msg.find("{ENEMY_HP}")) != std::string::npos)
        msg.replace(pos, 10, std::to_string(enemy.get_hp()));
    while ((pos = msg.find("{ENEMY_MP}")) != std::string::npos)
        msg.replace(pos, 10, std::to_string(enemy.get_mp()));
    while ((pos = msg.find("{CHAMBERS}")) != std::string::npos)
        msg.replace(pos, 10, std::to_string(gun.num_chambers()));
    while ((pos = msg.find("{BULLETS}")) != std::string::npos)
        msg.replace(pos, 9, std::to_string(gun.num_bullets()));

    return msg;
}

bool player::move(contestant& enemy, revolver& gun) {
    if (mp < max_mp) {
        mp++;
    }
    string msg = format_status(enemy,gun);




    // cout << "your hp: " << hp << endl;
    // cout << "your mp: " << mp << endl;
    // cout << endl;
    // cout << "enemy hp: " << enemy.get_hp() << endl;
    // cout << "enemy mp: " << enemy.get_mp() << endl;
    // cout << endl;
    // std::cout << "bullets: " << gun.num_bullets() << std::endl;
    // std::cout << "chambers: " << gun.num_chambers() << std::endl;
    int damage = 1;
    string act = "";
    //cin >> act;
    //int64_t last_update_id = getLastUpdateId();
    sendTelegramMessageWithKeyboard(msg, acts);
    act = waitForTelegramInput();
    while (act != "Shoot enemy" and act != "Shoot myself") {
        if (act == "Enhanced shot") {
            if (mp >= 3) {
                mp -= 3;
                damage = 2;
                sendTelegramMessage("spell was casted");
                sendTelegramMessage("your mp: "+std::to_string(mp));
                // cout << "spell was casted" << endl;
                // cout << "your mp: " << mp << endl;
            }
            else {
                sendTelegramMessage("not enough mana");
                sendTelegramMessage("your mp: "+std::to_string(mp)+", need 3");
                // cout << "not enough mana" << endl;
                // cout << "your mp: " << mp << ", need 3" << endl;
            }
        }
        else if (act == "Stun") {
            if (mp >= 5) {
                mp -= 5;
                enemy.stun();
                sendTelegramMessage("spell was casted");
                sendTelegramMessage("your mp: "+std::to_string(mp));
                // cout << "spell was casted" << endl;
                //cout << "your mp: " << mp << endl;
            }
            else {
                sendTelegramMessage("not enough mana");
                sendTelegramMessage("your mp: "+std::to_string(mp)+", need 5");
                // cout << "not enough mana" << endl;
                // cout << "your mp: " << mp << ", need 5" << endl;
            }
        }
        else if (act == "Heal") {
            if (mp >= 3 and hp < max_hp) {
                mp -= 3;
                hp++;
                sendTelegramMessage("spell was casted");
                sendTelegramMessage("your mp: "+std::to_string(mp));
                sendTelegramMessage("your hp: "+std::to_string(hp));

                // cout << "spell was casted" << endl;
                // cout << "your mp: " << mp << endl;
                // cout << "your hp: " << hp << endl;
            }
            else if (hp == max_hp) {
                sendTelegramMessage("you already have max health");
                //cout << "you already have max health" << endl;
            }
            else {
                sendTelegramMessage("not enough mana");
                sendTelegramMessage("your mp: "+std::to_string(mp)+", need 3");
                // cout << "not enough mana" << endl;
                // cout << "your mp: " << mp << ", need 3" << endl;
            }
        }
        else if (act == "Swap bullet") {
            if (mp >= 3) {
                mp -= 3;
                gun.swap_bullet();
                sendTelegramMessage("spell was casted");
                sendTelegramMessage("your mp: "+std::to_string(mp));
                // cout << "spell was casted" << endl;
                // cout << "your mp: " << mp << endl;
            }
            else {
                sendTelegramMessage("not enough mana");
                sendTelegramMessage("your mp: "+std::to_string(mp)+", need 3");
                // cout << "not enough mana" << endl;
                // cout << "your mp: " << mp << ", need 3" << endl;
            }
        }
        else if (act == "Fireball") {
            if (mp >= 5) {
                mp -= 5;
                enemy.set_hp(enemy.get_hp() - 1);
                if (enemy.get_hp() == 0) {
                    return 0;
                }
                sendTelegramMessage("spell was casted");
                sendTelegramMessage("your mp: "+std::to_string(mp));
                sendTelegramMessage("enemy hp: "+std::to_string(enemy.get_hp()));
                // cout << "spell was casted" << endl;
                // cout << "your mp: " << mp << endl;
                // cout << "enemy hp: " << enemy.get_hp() << endl;
            }
            else {
                sendTelegramMessage("not enough mana");
                sendTelegramMessage("your mp: "+std::to_string(mp)+", need 5");
                // cout << "not enough mana" << endl;
                // cout << "your mp: " << mp << ", need 5" << endl;
            }
        }
        else if (act == "Blood ritual") {
            if (hp > 1) {
                hp--;
                if (mp+2 > max_mp) {
                    mp = max_mp;
                }
                else {
                    mp+=2;
                }
                sendTelegramMessage("spell was casted");
                sendTelegramMessage("your mp: "+std::to_string(mp));
                sendTelegramMessage("your hp: "+std::to_string(hp));
                // cout << "spell was casted" << endl;
                // cout << "your mp: " << mp << endl;
                // cout << "your hp: " << hp << endl;
            }
            else {
                sendTelegramMessage(R"(not enough health
your hp: 1)"
                );

                // cout << "not enough health" << endl;
                // cout << "your hp: 1" << endl;
            }
        }
        else {
            sendTelegramMessage("unknown action");
            //cout << "unknown action" << endl;
        }
        //last_update_id = getLastUpdateId();
        msg = format_status(enemy,gun);
        sendTelegramMessageWithKeyboard(msg, acts);
        act = waitForTelegramInput();
        //cin >> act;
    }
    if (act == "Shoot enemy") {
        if (gun.shoot()) {
            enemy.set_hp(enemy.get_hp() - damage);
        }
        sendTelegramMessage("enemy hp: "+std::to_string(enemy.get_hp()));
        //cout << "enemy hp: " << enemy.get_hp() << endl;
        return 1;
    }
    else if (act == "Shoot myself") {
        if (!gun.shoot()) {
            return 0;
        }
        hp-=damage;
        sendTelegramMessage("your hp: "+std::to_string(hp));
        //cout << "your hp: " << hp << endl;
        return 1;
    }
}