

#ifndef TELEGRAM_H
#define TELEGRAM_H

#include <string>
#include <vector>
#include <cstdint>

// Отправка обычного сообщения
void sendTelegramMessage(const std::string& text);

// Отправка сообщения с клавиатурой
void sendTelegramMessageWithKeyboard(const std::string& text, const std::vector<std::vector<std::string>>& button_rows);

// void sendTelegramMessageWithInlineKeyboard(
//     const std::string& token,
//     int64_t chat_id,
//     const std::string& text,
//     const std::vector<std::pair<std::string, std::string>>& buttons // {текст кнопки, callback_data}
// );

// Ожидание и получение нового сообщения пользователя
std::string waitForTelegramInput();

// Вспомогательная функция для CURL (вы можете сделать её static в cpp, если не планируете использовать снаружи)
size_t writecallback(void* contents, size_t size, size_t nmemb, void* userp);

int64_t getLastUpdateId();

#endif //TELEGRAM_H
