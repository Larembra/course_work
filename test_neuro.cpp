#include <iostream>
#include "curl/curl.h"
#include <string>
#include "json.hpp"

using json = nlohmann::json; // Если используется nlohmann/json

// Callback-функция для записи ответа от сервера
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t total_size = size * nmemb;
    output->append((char*)contents, total_size);
    return total_size;
}

int main() {
    const std::string apiKey = "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";
    const std::string model = "deepseek/deepseek-r1-distill-llama-70b:free";
    const std::string userMessage = R"(You are participating in Magic Revolver Duel - a variation of russian roulette with magical rules.
Here are the conditions: you and your opponent have 3 HP each (maximum 3) and 3 MP each (maximum 10, recovers 1 MP per turn).
A 6-chamber revolver is used with a random number of bullets (from 1 to 5).
Available spells: 'enhancedshot' (3 MP, next shot deals double damage - 2 HP), 'stun' (5 MP, opponent skips a turn), 'heal' (3 MP, +1 HP to your health, but not above maximum),
'swapbullet' (3 MP, if there was a bullet in chamber, bullet disappears, if wasn't, bullet appears), 'fireball' (5 MP, deals 1 HP damage to the enemy), 'bloodritual' (0 MP, you lose 1 HP,
but instantly gain 2 MP that can be used this turn). Each of your moves must be strictly formatted: first list the used spells, EACH ON A NEW LINE (if you have enough mana, you can use more than one, as well as no spells per turn),
then ON A NEW LINE always write 'shootmyself' (gives an extra turn if there wasn't a bullet in the chamber) or 'shootenemy' (deals 1 HP damage if bullet).
You cannot use spells you don't have enough mana for.

Format of your output:
enhancedshot
shootenemy

Current status: Your HP: 3 | MP: 10
Opponent HP: 2 | MP: 5
Remaining chambers: 3
Remaining bullets: 2
FOLLOW THE FORMAT, DON'T ADD EXPLANATIONS. If HP drops to 0 - game over. Let's begin!)";

    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Failed to initialize curl!" << std::endl;
        return 1;
    }

    // Настройка запроса
    std::string url = "https://openrouter.ai/api/v1/chat/completions";
    std::string jsonPayload = R"({
        "model": ")" + model + R"(",
        "messages": [{"role": "user", "content": ")" + userMessage + R"("}]
    })";

    std::string response;

    // Устанавливаем заголовки
    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, ("Authorization: Bearer " + apiKey).c_str());
    headers = curl_slist_append(headers, "Content-Type: application/json");

    // Настраиваем параметры запроса
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonPayload.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    // Выполняем запрос
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
    } else {
        try {
            // Парсим JSON-ответ
            json jsonResponse = json::parse(response);

            // Извлекаем content из первого choice
            std::string content = jsonResponse["choices"][0]["message"]["content"];

            // Выводим только content
            std::cout << content << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Error parsing JSON: " << e.what() << std::endl;
            std::cerr << "Raw response: " << response << std::endl;
        }
    }

    // Очищаем ресурсы
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return 0;
}