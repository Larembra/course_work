#include "revolver.h"
#include "player.h"
#include "deepseek.h"
#include <iostream>
#include <string>
#include <vector>
#include <curl/curl.h>
#include "json.hpp"
#include <thread>
#include <chrono>
#include "telegram.h"


using json = nlohmann::json;
using namespace std;

// const std::string TELEGRAM_TOKEN = "7928899642:AAGmfMYq3LmjwgySM9lx0heY51Ov6sbscko";
// const int64_t CHAT_ID = 1508659574;



int main() {
    //int64_t last_update_id = 0;
    int64_t last_update_id = getLastUpdateId();
    string chosen;

    //sendTelegramMessageWithInlineKeyboard(TELEGRAM_TOKEN, CHAT_ID, "Welcome to Magic Revolver Duel!", {{"Play!", "play"}, {"Rules", "rules"}, {"Telegram", "telegram"}, {"Telegram", "telegram"}, {"Telegram", "telegram"}, {"Telegram", "telegram"}, {"Telegram", "telegram"}});
    sendTelegramMessageWithKeyboard("Welcome to Magic Revolver Duel!", {{"Play!", "Rules"}});
    chosen = waitForTelegramInput();
    if (chosen == "Rules") {
        sendTelegramMessageWithKeyboard("It's a very cool game!", {{"Play!"}});
        chosen = waitForTelegramInput();
    }
    if (chosen == "Play!") {
        vector <string> enemies = {"DeepSeek", "ChatGPT"};
        // Отправляем клавиатуру с кнопкой "DeepSeek"
        sendTelegramMessageWithKeyboard("Choose your enemy:", {enemies});

        // Ждём нажатия кнопки (пользователь пришлёт сообщение "DeepSeek")
        chosen = waitForTelegramInput();

        if (chosen == "DeepSeek") {
            revolver gun;
            gun.new_magazine();

            player p1;
            p1.set_max_hp(3);
            p1.set_mp(3,10);
            deepseek d;
            d.set_max_hp(3);
            d.set_mp(3,10);

            bool result = 0;
            while (p1.get_hp() > 0 && d.get_hp() > 0) {
                if (result == 0) {
                    if (!p1.is_stunned()) {
                        sendTelegramMessage("p1, your turn!");
                        // Ждём хода игрока через Telegram:
                        //std::string command = waitForTelegramInput(TELEGRAM_TOKEN, CHAT_ID, last_update_id);
                        // Передайте команду в move, если move умеет принимать строку команды:
                        result = p1.move(d, gun);
                    }
                    else {
                        sendTelegramMessage("enemy turn!");
                        result = d.move(p1, gun);
                    }
                }
                else if (result == 1) {
                    if (!d.is_stunned()) {
                        sendTelegramMessage("enemy turn!");
                        result = d.move(p1, gun);
                    }
                    else {
                        sendTelegramMessage("p1, your turn!");
                        //std::string command = waitForTelegramInput(TELEGRAM_TOKEN, CHAT_ID, last_update_id);
                        result = p1.move(d, gun);
                    }
                }
            }
            if (p1.get_hp() <= 0) {
                sendTelegramMessage("You lose!");
            }
            else {
                sendTelegramMessage("You win!");
            }
        }
    }

    return 0;
}
