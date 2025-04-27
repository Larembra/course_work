#include "game.h"
#include "revolver.h"
#include "player.h"
#include "deepseek.h"
#include <iostream>
#include <string>
#include <vector>
#include <curl/curl.h>
#include "json.hpp"
#include <unistd.h>
#include <thread>
#include <chrono>
#include "telegram.h"

std::string rules =
    "<b>Magic Revolver Duel</b> - variation of russian roulette with magical rules.\n"
    "You and your opponent have 3 HP each (maximum 3) and 3 MP each (maximum 10, recovers 1 MP per turn).\n"
    "A <b>6-chamber</b> revolver is used with a random number of bullets (from 1 to 5).\n"
    "<b>Available spells:</b>\n"
    "<b>Power up</b> (3 MP, next shot deals double damage - 2 HP),\n"
    "<b>Stun</b> (5 MP, opponent skips a turn),\n"
    "<b>Heal</b> (3 MP, +1 HP to your health, but not above maximum),\n"
    "<b>Swap bullet</b> (3 MP, if there was a bullet in chamber, bullet disappears, if wasn't, bullet appears),\n"
    "<b>Fireball</b> (5 MP, deals 1 HP damage to the enemy),\n"
    "<b>Blood ritual</b> (0 MP, you lose 1 HP, but instantly gain 2 MP that can be used this turn).\n"
    "If HP drops to 0 - game over. Let's begin!";

void game(int64_t chat_id) {
    std::string chosen;
    //sendTelegramMessageWithInlineKeyboard(TELEGRAM_TOKEN, CHAT_ID, "Welcome to Magic Revolver Duel!", {{"Play!", "play"}, {"Rules", "rules"}, {"Telegram", "telegram"}, {"Telegram", "telegram"}, {"Telegram", "telegram"}, {"Telegram", "telegram"}, {"Telegram", "telegram"}});
    sendTelegramMessageWithKeyboard(chat_id, "🎩Welcome to Magic Revolver Duel!🎩", {{"Play!", "Rules"}});
    chosen = waitForTelegramInput(chat_id);
    while (true) {
        if (chosen == "Rules") {
            sendTelegramMessageWithKeyboard(chat_id, rules, {{"Play!"}});
            chosen = waitForTelegramInput(chat_id);
        }
        if (chosen == "Play!") {
            std::vector <std::string> enemies = {"DeepSeek"};
            // Отправляем клавиатуру с кнопкой "DeepSeek"
            sendTelegramMessageWithKeyboard(chat_id, "⚔️Choose your enemy:⚔️", {enemies});

            // Ждём нажатия кнопки (пользователь пришлёт сообщение "DeepSeek")
            chosen = waitForTelegramInput(chat_id);

            if (chosen == "DeepSeek") {
                revolver gun;
                gun.new_magazine();
                sendTelegramMessage(chat_id, "6-chamber revolver randomly loaded with ❗️"+ std::to_string(gun.num_bullets())+"❗️ bullets!");
                sendTelegramMessage(chat_id, "🍀 Good luck! 🍀");

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
                            sendTelegramMessage(chat_id, "💖 Your turn! 💖");
                            // Ждём хода игрока через Telegram:
                            //std::string command = waitForTelegramInput(TELEGRAM_TOKEN, CHAT_ID, last_update_id);
                            // Передайте команду в move, если move умеет принимать строку команды:
                            result = p1.move(chat_id, d, gun);
                        }
                        else {
                            sendTelegramMessage(chat_id, "💀 Enemy turn! 💀");
                            result = d.move(chat_id, p1, gun);
                        }
                    }
                    else if (result == 1) {
                        if (!d.is_stunned()) {
                            sendTelegramMessage(chat_id, "💀 Enemy turn! 💀");
                            result = d.move(chat_id, p1, gun);
                        }
                        else {
                            sendTelegramMessage(chat_id, "💖 Your turn! 💖");
                            //std::string command = waitForTelegramInput(TELEGRAM_TOKEN, CHAT_ID, last_update_id);
                            result = p1.move(chat_id, d, gun);
                        }
                    }
                }
                if (p1.get_hp() <= 0) {
                    sendTelegramMessage(chat_id, "💀 You lose! 💀");
                }
                else {
                    sendTelegramMessage(chat_id, "🎉 You win! 🎉");
                }
            }
        }
        sendTelegramMessageWithKeyboard(chat_id, "Game over!", {{"Play!", "Rules"}});
        chosen = waitForTelegramInput(chat_id);
    }
}