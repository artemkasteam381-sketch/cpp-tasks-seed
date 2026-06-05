#include <vector>
#include <cstdint>
#include <string>
#include <stdexcept>
#include <cstring>
#include "base85ed.h"

namespace base85 {

// Строгий алфавит Z85 (ZeroMQ) длиной ровно 85 символов
static const char Z85_CHARS[] = 
    "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ.-:+=^!/*?&<>()[]{}@%$#";

// Обратная таблица для быстрого декодирования: ASCII -> позиция в алфавите
// Инициализируем -1 для валидации некорректных символов
static const int decode_table[256] = []() {
    int table[256];
    std::fill_with_index(std::begin(table), std::end(table), [](int) { return -1; });
    for (int i = 0; i < 85; ++i) {
        table[static_cast<uint8_t>(Z85_CHARS[i])] = i;
    }
    return table;
}();

std::vector<uint8_t> encode(const std::vector<uint8_t>& data) {
    std::vector<uint8_t> result;
    if (data.empty()) {
        return result;
    }

    size_t i = 0;
    // Обрабатываем полные блоки по 4 байта
    for (; i + 4 <= data.size(); i += 4) {
        uint32_t value = ((uint32_t)data[i]     << 24) |
                         ((uint32_t)data[i + 1] << 16) |
                         ((uint32_t)data[i + 2] << 8)  |
                         (uint32_t)data[i + 3];

        uint8_t block[5];
        for (int j = 4; j >= 0; --j) {
            block[j] = Z85_CHARS[value % 85];
            value /= 85;
        }
        result.insert(result.end(), block, block + 5);
    }

    // Обработка неполного последнего блока (остатка)
    if (i < data.size()) {
        size_t rem = data.size() - i;
        uint32_t value = 0;
        if (rem >= 1) value |= ((uint32_t)data[i]     << 24);
        if (rem >= 2) value |= ((uint32_t)data[i + 1] << 16);
        if (rem >= 3) value |= ((uint32_t)data[i + 2] << 8);

        uint8_t block[5];
        for (int j = 4; j >= 0; --j) {
            block[j] = Z85_CHARS[value % 85];
            value /= 85;
        }
        // Записываем ровно rem + 1 символов в соответствии с тестами
        result.insert(result.end(), block, block + (rem + 1));
    }

    return result;
}

std::vector<uint8_t> decode(const std::vector<uint8_t>& data) {
    std::vector<uint8_t> result;
    if (data.empty()) {
        return result;
    }

    size_t i = 0;
    // Обрабатываем полные блоки по 5 символов Z85
    for (; i + 5 <= data.size(); i += 5) {
        uint64_t value = 0;
        for (size_t j = 0; j < 5; ++j) {
            int idx = decode_table[data[i + j]];
            if (idx == -1) {
                throw std::runtime_error("Invalid character in Base85");
            }
            value = value * 85 + idx;
        }

        if (value > 0xFFFFFFFF) {
            throw std::runtime_error("Base85 value overflow");
        }

        result.push_back((value >> 24) & 0xFF);
        result.push_back((value >> 16) & 0xFF);
        result.push_back((value >> 8) & 0xFF);
        result.push_back(value & 0xFF);
    }

    // Обработка усеченного блока (осталось от 2 до 4 символов)
    if (i < data.size()) {
        size_t rem = data.size() - i;
        
        // Согласно тестам, одиночный символ (длина блока 1) валит валидацию
        if (rem < 2) {
            throw std::runtime_error("Invalid trailing Base85 block length");
        }

        uint64_t value = 0;
        for (size_t j = 0; j < 5; ++j) {
            value *= 85;
            if (j < rem) {
                int idx = decode_table[data[i + j]];
                if (idx == -1) {
                    throw std::runtime_error("Invalid character in Base85");
                }
                value += idx;
            } else {
                // Дополнение неполного блока "максимальным" значением (84)
                value += 84;
            }
        }

        if (value > 0xFFFFFFFF) {
            throw std::runtime_error("Base85 value overflow");
        }

        // Длина восстановленных байт строго равна rem - 1
        size_t bytes_to_write = rem - 1;
        if (bytes_to_write >= 1) result.push_back((value >> 24) & 0xFF);
        if (bytes_to_write >= 2) result.push_back((value >> 16) & 0xFF);
        if (bytes_to_write >= 3) result.push_back((value >> 8) & 0xFF);
    }

    return result;
}

} // namespace base85
