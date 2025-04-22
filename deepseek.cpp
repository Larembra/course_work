#include "deepseek.h"
#include "revolver.h"
#include <iostream>

#include <iostream>
#include <vector>
#include <string>
#include "curl/curl.h"
#include "json.hpp"


using json = nlohmann::json;

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t total_size = size * nmemb;
    output->append((char*)contents, total_size);
    return total_size;
}

bool deepseek::move(contestant& enemy, revolver& gun) {
    if (mp < max_mp) {
        mp++;
    }
    std::cout << "your hp: " << enemy.get_hp() << std::endl;
    std::cout << "your mp: " << enemy.get_mp() << std::endl;
    std::cout << std::endl;
    std::cout << "enemy hp: " << hp << std::endl;
    std::cout << "enemy mp: " << mp << std::endl;
    std::cout << std::endl;
    std::cout << "bullets: " << gun.num_bullets() << std::endl;
    std::cout << "chambers: " << gun.num_chambers() << std::endl;
    std::vector <std::string> acts = get_response(enemy, gun);
    for (auto a : acts) {
        std::cout << a << std::endl;
    }
    std::cout << std::endl;
    int damage = 1;
    // std::string act = "";
    // std::cin >> act;


    // acts = {"heal"};//!!!!!!!!!!!!!!!!!!!!!!!!
    // hp = 2;

    for (auto act : acts) {
        if (act == "enhancedshot") {
            if (mp >= 3) {
                mp -= 3;
                damage = 2;
                std::cout << "enemy used enhanced shot" << std::endl;
                std::cout << "enemy mp: " << mp << std::endl;
            }
            else {
                // std::cout << "not enough mana" << std::endl;
                // std::cout << "your mp: " << mp << ", need 3" << std::endl;
            }
        }
        else if (act == "stun") {
            if (mp >= 5) {
                mp -= 5;
                enemy.stun();
                std::cout << "enemy used stun" << std::endl;
                std::cout << "enemy mp: " << mp << std::endl;
            }
            else {
                // std::cout << "not enough mana" << std::endl;
                // std::cout << "your mp: " << mp << ", need 5" << std::endl;
            }
        }
        else if (act == "heal") {
            if (mp >= 3 and hp < max_hp) {
                mp -= 3;
                hp++;
                std::cout << "enemy used heal" << std::endl;
                std::cout << "enemy mp: " << mp << std::endl;
                std::cout << "enemy hp: " << hp << std::endl;
            }
            else if (hp == max_hp) {
                // std::cout << "you already have max health" << std::endl;
            }
            else {
                // std::cout << "not enough mana" << std::endl;
                // std::cout << "your mp: " << mp << ", need 3" << std::endl;
            }
        }
        else if (act == "swapbullet") {
            if (mp >= 3) {
                mp -= 3;
                gun.swap_bullet();
                std::cout << "enemy used swapbullet" << std::endl;
                std::cout << "enemy mp: " << mp << std::endl;
            }
            else {
                // std::cout << "not enough mana" << std::endl;
                // std::cout << "your mp: " << mp << ", need 3" << std::endl;
            }
        }
        else if (act == "fireball") {
            if (mp >= 5) {
                mp -= 5;
                enemy.set_hp(enemy.get_hp() - 1);
                if (enemy.get_hp() == 0) {
                    return 1;
                }
                std::cout << "enemy used fireball" << std::endl;
                std::cout << "enemy mp: " << mp << std::endl;
                std::cout << "your hp: " << enemy.get_hp() << std::endl;
            }
            else {
                // std::cout << "not enough mana" << std::endl;
                // std::cout << "your mp: " << mp << ", need 5" << std::endl;
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
                std::cout << "enemy used bloodritual" << std::endl;
                std::cout << "enemy mp: " << mp << std::endl;
                std::cout << "enemy hp: " << hp << std::endl;
            }
            else {
                // std::cout << "not enough health" << std::endl;
                // std::cout << "your hp: " << hp << std::endl;
            }
        }
        else if (act == "shootenemy") {
            std::cout << "enemy shot you! " << std::endl;
            if (gun.shoot()) {
                enemy.set_hp(enemy.get_hp() - damage);
            }

            std::cout << "your hp: " << enemy.get_hp() << std::endl;
            return 0;
        }
        else if (act == "shootmyself") {
            std::cout << "enemy shot himself! " << std::endl;
            if (!gun.shoot()) {
                return 1;
            }
            hp-=damage;
            std::cout << "enemy hp: " << hp << std::endl;
            return 0;
        }
        else {
            std::cout << "unknown action" << std::endl;
        }
        //std::cin >> act;
    }

}

const std::string DEEPSEEK_PROMPT_FMT = R"(You are participating in Magic Revolver Duel - a variation of russian roulette with magical rules.
Here are the conditions: you and your opponent have 3 HP each (maximum 3) and 3 MP each (maximum 10, recovers 1 MP per turn).
A 6-chamber revolver is used with a random number of bullets (from 1 to 5).
Available spells: 'enhancedshot' (3 MP, next shot deals double damage - 2 HP), 'stun' (5 MP, opponent skips a turn), 'heal' (3 MP, +1 HP to your health, but not above maximum),
'swapbullet' (3 MP, if there was a bullet in chamber, bullet disappears, if wasn't, bullet appears), 'fireball' (5 MP, deals 1 HP damage to the enemy), 'bloodritual' (0 MP, you lose 1 HP,
but instantly gain 2 MP that can be used this turn).
All your response MUST BE STRICTLY FORMATTED: first list the used spells, EACH ON A NEW LINE (if you have enough mana, you can use one or more unique or identical spells, as well as no spells per turn),
then ON A NEW LINE always write 'shootmyself' (gives an extra turn if there wasn't a bullet in the chamber) or 'shootenemy' (deals 1 HP damage if bullet).
You cannot use spells you don't have enough mana for.

Example of your response:
enhancedshot
shootenemy

Current status: Your HP: {MY_HP} | MP: {MY_MP}
Opponent HP: {ENEMY_HP} | MP: {ENEMY_MP}
Remaining chambers: {CHAMBERS}
Remaining bullets: {BULLETS}
FOLLOW THE FORMAT, DON'T ADD EXPLANATIONS. DON'T REASON LONG, YOU HAVE TO GIVE ANSWER ALMOST INSTANTLY. If HP drops to 0 - game over. Let's begin!)";

std::string deepseek::format_prompt(contestant& enemy, revolver& gun) {
    std::ostringstream oss;
    std::string prompt = DEEPSEEK_PROMPT_FMT;

    // Заменяем плейсхолдеры на фактические значения
    size_t pos;
    while ((pos = prompt.find("{MY_HP}")) != std::string::npos)
        prompt.replace(pos, 7, std::to_string(hp));
    while ((pos = prompt.find("{MY_MP}")) != std::string::npos)
        prompt.replace(pos, 7, std::to_string(mp));
    while ((pos = prompt.find("{ENEMY_HP}")) != std::string::npos)
        prompt.replace(pos, 10, std::to_string(enemy.get_hp()));
    while ((pos = prompt.find("{ENEMY_MP}")) != std::string::npos)
        prompt.replace(pos, 10, std::to_string(enemy.get_mp()));
    while ((pos = prompt.find("{CHAMBERS}")) != std::string::npos)
        prompt.replace(pos, 10, std::to_string(gun.num_chambers()));
    while ((pos = prompt.find("{BULLETS}")) != std::string::npos)
        prompt.replace(pos, 9, std::to_string(gun.num_bullets()));

    return prompt;
}

std::vector<std::string> deepseek::get_response(contestant& enemy, revolver& gun) {
    const std::string apiKey = "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";
    const std::string model = "deepseek/deepseek-r1-distill-llama-70b:free";
    std::string userMessage = format_prompt(enemy, gun);

    std::vector<std::string> actions;

    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Failed to initialize curl!" << std::endl;
        return actions;
    }

    std::string url = "https://openrouter.ai/api/v1/chat/completions";
    std::string jsonPayload = R"({
        "model": ")" + model + R"(",
        "messages": [{"role": "user", "content": ")" + userMessage + R"("}]
    })";

    std::string response;

    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, ("Authorization: Bearer " + apiKey).c_str());
    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonPayload.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
    } else {
        try {
            json jsonResponse = json::parse(response);
            std::string content = jsonResponse["choices"][0]["message"]["content"];

            // Разбиваем content на строки и добавляем в actions
            size_t start = 0, end = 0;
            while ((end = content.find('\n', start)) != std::string::npos) {
                std::string line = content.substr(start, end - start);
                if (!line.empty()) actions.push_back(line);
                start = end + 1;
            }
            // Добавляем последнюю строку, если она не пустая
            if (start < content.size()) {
                std::string line = content.substr(start);
                if (!line.empty()) actions.push_back(line);
            }
        } catch (const std::exception& e) {
            std::cerr << "Error parsing JSON: " << e.what() << std::endl;
            std::cerr << "Raw response: " << response << std::endl;
        }
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return actions;
}