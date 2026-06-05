#include "base85ed.h"

#include <array>
#include <cstdint>
#include <stdexcept>
#include <vector>

namespace base85
{
namespace
{
constexpr char alphabet[] =
    "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz!#$%&()*+-;<=>?@^_`{|}~";

std::array<int, 256> make_decode_table()
{
    std::array<int, 256> table{};
    table.fill(-1);

    for (int i = 0; i < 85; ++i)
    {
        table[static_cast<unsigned char>(alphabet[i])] = i;
    }

    return table;
}

const std::array<int, 256> decode_table = make_decode_table();

uint32_t make_value(uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3)
{
    return (static_cast<uint32_t>(b0) << 24) |
           (static_cast<uint32_t>(b1) << 16) |
           (static_cast<uint32_t>(b2) << 8) |
           static_cast<uint32_t>(b3);
}

uint32_t read_full_block(const std::vector<uint8_t>& bytes, std::size_t pos)
{
    return make_value(bytes[pos], bytes[pos + 1], bytes[pos + 2], bytes[pos + 3]);
}

uint32_t read_tail_block(const std::vector<uint8_t>& bytes, std::size_t pos, std::size_t tail)
{
    uint8_t b0 = 0;
    uint8_t b1 = 0;
    uint8_t b2 = 0;

    if (tail >= 1)
    {
        b0 = bytes[pos];
    }
    if (tail >= 2)
    {
        b1 = bytes[pos + 1];
    }
    if (tail >= 3)
    {
        b2 = bytes[pos + 2];
    }

    return make_value(b0, b1, b2, 0);
}

void append_encoded_block(std::vector<uint8_t>& out, uint32_t value, std::size_t count)
{
    uint8_t block[5]{};

    for (int i = 4; i >= 0; --i)
    {
        block[i] = static_cast<uint8_t>(alphabet[value % 85]);
        value /= 85;
    }

    for (std::size_t i = 0; i < count; ++i)
    {
        out.push_back(block[i]);
    }
}

uint32_t decode_block(const std::vector<uint8_t>& b85str, std::size_t pos, std::size_t count)
{
    uint64_t value = 0;

    for (std::size_t i = 0; i < 5; ++i)
    {
        int digit = 84;

        if (i < count)
        {
            digit = decode_table[static_cast<unsigned char>(b85str[pos + i])];
            if (digit < 0)
            {
                throw std::runtime_error("invalid Base85 character");
            }
        }

        value = value * 85 + static_cast<uint64_t>(digit);
    }

    if (value > 0xFFFFFFFFULL)
    {
        throw std::runtime_error("Base85 value is too large");
    }

    return static_cast<uint32_t>(value);
}

void append_decoded_block(std::vector<uint8_t>& out, uint32_t value, std::size_t count)
{
    if (count >= 1)
    {
        out.push_back(static_cast<uint8_t>((value >> 24) & 0xFF));
    }
    if (count >= 2)
    {
        out.push_back(static_cast<uint8_t>((value >> 16) & 0xFF));
    }
    if (count >= 3)
    {
        out.push_back(static_cast<uint8_t>((value >> 8) & 0xFF));
    }
    if (count >= 4)
    {
        out.push_back(static_cast<uint8_t>(value & 0xFF));
    }
}
} // namespace

std::vector<uint8_t> encode(const std::vector<uint8_t>& bytes)
{
    std::vector<uint8_t> out;
    out.reserve(((bytes.size() + 3) / 4) * 5);

    std::size_t pos = 0;

    while (pos + 4 <= bytes.size())
    {
        append_encoded_block(out, read_full_block(bytes, pos), 5);
        pos += 4;
    }

    const std::size_t tail = bytes.size() - pos;
    if (tail > 0)
    {
        append_encoded_block(out, read_tail_block(bytes, pos, tail), tail + 1);
    }

    return out;
}

std::vector<uint8_t> decode(const std::vector<uint8_t>& b85str)
{
    if (b85str.size() % 5 == 1)
    {
        throw std::runtime_error("invalid Base85 length");
    }

    std::vector<uint8_t> out;
    out.reserve((b85str.size() / 5) * 4 + 4);

    std::size_t pos = 0;

    while (pos + 5 <= b85str.size())
    {
        append_decoded_block(out, decode_block(b85str, pos, 5), 4);
        pos += 5;
    }

    const std::size_t tail = b85str.size() - pos;
    if (tail > 0)
    {
        append_decoded_block(out, decode_block(b85str, pos, tail), tail - 1);
    }

    return out;
}

} // namespace base85
