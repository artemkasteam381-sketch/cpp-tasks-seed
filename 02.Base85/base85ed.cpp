#include <vector>
#include <cstdint>
#include <string>
#include <stdexcept>
#include <cstring>
#include "base85ed.h"

namespace base85 {

std::vector<uint8_t> encode(const std::vector<uint8_t>& data) {
    std::vector<uint8_t> result;
    if (data.empty()) {
        return result;
    }

    size_t i = 0;
    for (; i + 4 <= data.size(); i += 4) {
        uint32_t value = ((uint32_t)data[i] << 24) |
                         ((uint32_t)data[i + 1] << 16) |
                         ((uint32_t)data[i + 2] << 8) |
                         (uint32_t)data[i + 3];

        std::vector<uint8_t> block(5);
        for (int j = 4; j >= 0; --j) {
            block[j] = (value % 85) + 33;
            value /= 85;
        }
        result.insert(result.end(), block.begin(), block.end());
    }

    // Обработка остатка (если данные не кратны 4 байтам)
    if (i < data.size()) {
        size_t rem = data.size() - i;
        uint32_t value = 0;
        for (size_t j = 0; j < 4; ++j) {
            value <<= 8;
            if (j < rem) {
                value |= data[i + j];
            }
        }

        std::vector<uint8_t> block(5);
        for (int j = 4; j >= 0; --j) {
            block[j] = (value % 85) + 33;
            value /= 85;
        }
        // Записываем только значащие символы (rem + 1)
        result.insert(result.end(), block.begin(), block.begin() + rem + 1);
    }

    return result;
}

std::vector<uint8_t> decode(const std::vector<uint8_t>& data) {
    std::vector<uint8_t> result;
    if (data.empty()) {
        return result;
    }

    // Проверка на корректность длины по стандарту Base85
    // Группы должны быть кратны 5 (или с учетом правильного остатка, 
    // но в рамках простых тестов проверяется базовая валидность)
    if (data.size() % 5 == 1) {
        throw std::runtime_error("Invalid Base85 data length");
    }

    size_t i = 0;
    for (; i + 5 <= data.size(); i += 5) {
        uint64_t value = 0; // используем uint64_t, чтобы избежать переполнения при расчете
        for (size_t j = 0; j < 5; ++j) {
            if (data[i + j] < 33 || data[i + j] > 117) {
                throw std::runtime_error("Invalid character in Base85");
            }
            value = value * 85 + (data[i + j] - 33);
        }

        if (value > 0xFFFFFFFF) {
            throw std::runtime_error("Base85 value overflow");
        }

        result.push_back((value >> 24) & 0xFF);
        result.push_back((value >> 16) & 0xFF);
        result.push_back((value >> 8) & 0xFF);
        result.push_back(value & 0xFF);
    }

    // Обработка неполного последнего блока (остатка)
    if (i < data.size()) {
        size_t rem = data.size() - i;
        if (rem < 2) {
            throw std::runtime_error("Invalid trailing Base85 block length");
        }
        
        uint64_t value = 0;
        for (size_t j = 0; j < 5; ++j) {
            value *= 85;
            if (j < rem) {
                if (data[i + j] < 33 || data[i + j] > 117) {
                    throw std::runtime_error("Invalid character in Base85");
                }
                value += (data[i + j] - 33);
            } else {
                value += 84; // дополняем максимальным значением по стандарту
            }
        }

        if (value > 0xFFFFFFFF) {
            throw std::runtime_error("Base85 value overflow");
        }

        for (size_t j = 0; j < rem - 1; ++j) {
            result.push_back((value >> (24 - j * 8)) & 0xFF);
        }
    }

    return result;
}

} // namespace base85
