#include "telegram.h"
#include <curl/curl.h>
#include "json.hpp"
#include <thread>
#include <chrono>

using json = nlohmann::json;

const std::string token = "7928899642:AAGmfMYq3LmjwgySM9lx0heY51Ov6sbscko";
const int64_t chat_id = 1508659574;

size_t writecallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

void sendTelegramMessage(const std::string& text) {
    CURL* curl = curl_easy_init();
    if (curl) {
        std::string url = "https://api.telegram.org/bot" + token + "/sendMessage";
        std::string postFields = "chat_id=" + std::to_string(chat_id) + "&text=" + curl_easy_escape(curl, text.c_str(), 0);

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postFields.c_str());
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
}

// Отправка сообщения с кастомной клавиатурой (reply_markup)
// void sendTelegramMessageWithKeyboard(const std::string& token, int64_t chat_id, const std::string& text, const std::vector<std::string>& buttons) {
//     CURL* curl = curl_easy_init();
//     if (curl) {
//         std::string url = "https://api.telegram.org/bot" + token + "/sendMessage";
//
//         // Формируем JSON клавиатуры
//         json keyboard = {
//             {"keyboard", json::array({buttons})},
//             {"one_time_keyboard", true},
//             {"resize_keyboard", true}
//         };
//         std::string keyboardStr = keyboard.dump();
//
//         // Формируем поля POST-запроса
//         std::string postFields = "chat_id=" + std::to_string(chat_id) +
//             "&text=" + curl_easy_escape(curl, text.c_str(), 0) +
//             "&reply_markup=" + curl_easy_escape(curl, keyboardStr.c_str(), 0);
//
//         curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
//         curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postFields.c_str());
//         curl_easy_perform(curl);
//         curl_easy_cleanup(curl);
//     }
// }

void sendTelegramMessageWithKeyboard(

    const std::string& text,
    const std::vector<std::vector<std::string>>& button_rows // теперь вектор векторов!
) {
    CURL* curl = curl_easy_init();
    if (curl) {
        std::string url = "https://api.telegram.org/bot" + token + "/sendMessage";

        // Формируем JSON клавиатуры
        json keyboard = {
            {"keyboard", button_rows},
            {"one_time_keyboard", true},
            {"resize_keyboard", true}
        };
        std::string keyboardStr = keyboard.dump();

        // Формируем поля POST-запроса
        std::string postFields = "chat_id=" + std::to_string(chat_id) +
            "&text=" + curl_easy_escape(curl, text.c_str(), 0) +
            "&reply_markup=" + curl_easy_escape(curl, keyboardStr.c_str(), 0);

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postFields.c_str());
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
}

// void sendTelegramMessageWithInlineKeyboard(
//     const std::string& token,
//     int64_t chat_id,
//     const std::string& text,
//     const std::vector<std::pair<std::string, std::string>>& buttons // {текст кнопки, callback_data}
// ) {
//     CURL* curl = curl_easy_init();
//     if (curl) {
//         std::string url = "https://api.telegram.org/bot" + token + "/sendMessage";
//
//         // Формируем inline_keyboard (один ряд)
//         json row = json::array();
//         for (const auto& btn : buttons) {
//             row.push_back({
//                 {"text", btn.first},
//                 {"callback_data", btn.second}
//             });
//         }
//         json keyboard = {
//             {"inline_keyboard", json::array({row})}
//         };
//         std::string keyboardStr = keyboard.dump();
//
//         // Формируем поля POST-запроса
//         std::string postFields = "chat_id=" + std::to_string(chat_id) +
//             "&text=" + curl_easy_escape(curl, text.c_str(), 0) +
//             "&reply_markup=" + curl_easy_escape(curl, keyboardStr.c_str(), 0);
//
//         curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
//         curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postFields.c_str());
//         curl_easy_perform(curl);
//         curl_easy_cleanup(curl);
//     }
// }

// Функция для ожидания и получения нового сообщения пользователя из Telegram
std::string waitForTelegramInput() {
    int64_t last_update_id = getLastUpdateId();
    while (true) {
        CURL* curl = curl_easy_init();
        std::string readBuffer;
        if (curl) {
            std::string url = "https://api.telegram.org/bot" + token + "/getUpdates?timeout=10";
            if (last_update_id != 0) {
                url += "&offset=" + std::to_string(last_update_id + 1);
            }
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writecallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
            CURLcode res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);

            if (res == CURLE_OK) {
                auto j = json::parse(readBuffer);
                if (j.contains("result") && j["result"].is_array()) {
                    for (const auto& update : j["result"]) {
                        if (update.contains("update_id")) {
                            last_update_id = update["update_id"];
                        }
                        if (update.contains("message") && update["message"].contains("chat") &&
                            update["message"]["chat"]["id"] == chat_id &&
                            update["message"].contains("text")) {
                            return update["message"]["text"].get<std::string>();
                            }
                    }
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int64_t getLastUpdateId() {
    CURL* curl = curl_easy_init();
    std::string readBuffer;
    int64_t last_update_id = 0;
    if (curl) {
        std::string url = "https://api.telegram.org/bot" + token + "/getUpdates?timeout=1";
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writecallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        auto j = json::parse(readBuffer);
        if (j.contains("result") && j["result"].is_array()) {
            for (const auto& update : j["result"]) {
                if (update.contains("update_id") && update["update_id"].get<int64_t>() > last_update_id) {
                    last_update_id = update["update_id"].get<int64_t>();
                }
            }
        }
    }
    return last_update_id;
}