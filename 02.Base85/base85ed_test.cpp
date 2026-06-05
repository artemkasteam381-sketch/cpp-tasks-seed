#include "base85ed.h"

#include <cstdint>
#include <initializer_list>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

std::vector<uint8_t> bytes_from_string(const std::string& text)
{
    return std::vector<uint8_t>(text.begin(), text.end());
}

std::vector<uint8_t> bytes_from_values(std::initializer_list<uint8_t> values)
{
    return std::vector<uint8_t>(values.begin(), values.end());
}

std::vector<uint8_t> increasing_bytes(std::size_t count)
{
    std::vector<uint8_t> result;
    result.reserve(count);

    for (std::size_t i = 0; i < count; ++i)
    {
        result.push_back(static_cast<uint8_t>(i & 0xFF));
    }

    return result;
}

struct Case
{
    std::vector<uint8_t> decoded;
    std::string encoded;
};

const std::vector<Case> cases = {
    {bytes_from_string(""), ""},
    {bytes_from_string("1"), "F#"},
    {bytes_from_string("12"), "F){"},
    {bytes_from_string("123"), "F)}j"},
    {bytes_from_string("1234"), "F)}kW"},
    {bytes_from_string("C++"), "Ln|u"},
    {bytes_from_string("hello"), "Xk~0{Zv"},
    {bytes_from_string("hello world"), "Xk~0{Zy<MXa%^M"},
    {bytes_from_string("Base85 encoder and decoder"), "LSb`dI5i+;Zewp`WpW^4Ze$>2Wn*t-WpV"},
    {bytes_from_values({0}), "00"},
    {bytes_from_values({0, 0, 0, 0}), "00000"},
    {bytes_from_values({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}), "0RjUA1qKHQ2?`4g4Gs?"},
    {increasing_bytes(256), "009C61O)~M2nh-c3=Iws5D^j+6crX17#SKH9337XAR!_nBqb&%C@Cr{EG;fCFflSSG&MFiI5|2yJUu=?KtV!7L`6nNNJ&adOifNtP*GA-R8>}2SXo+ITwPvYU}0ioWMyV&XlZI|Y;A6DaB*^Tbai%jczJqze0_d@fPsR8goTEOh>41ejE#<ukdcy;l$Dm3n3<ZJoSmMZprN9pq@|{(sHv)}tgWuEu(7hUw6(UkxVgH!yuH4^z`?@9#Kp$P$jQpf%+1cv(9zP<)YaD4*xB0K+}+;a;Njxq<mKk)=;`X~?CtLF@bU8V^!4`l`1$(#{Qds_"},
};

TEST(Base85, EncodesKnownValues)
{
    for (const Case& c : cases)
    {
        EXPECT_EQ(base85::encode(c.decoded), bytes_from_string(c.encoded));
    }
}

TEST(Base85, DecodesKnownValues)
{
    for (const Case& c : cases)
    {
        EXPECT_EQ(base85::decode(bytes_from_string(c.encoded)), c.decoded);
    }
}

TEST(Base85, RoundTripForLengthsFromZeroToThreeHundred)
{
    for (std::size_t length = 0; length <= 300; ++length)
    {
        std::vector<uint8_t> data;
        data.reserve(length);

        for (std::size_t i = 0; i < length; ++i)
        {
            data.push_back(static_cast<uint8_t>((i * 37 + length * 11) & 0xFF));
        }

        EXPECT_EQ(base85::decode(base85::encode(data)), data) << "length = " << length;
    }
}

TEST(Base85, EncodedLengthIsCorrect)
{
    for (std::size_t length = 0; length <= 100; ++length)
    {
        const std::size_t full_blocks = length / 4;
        const std::size_t tail = length % 4;
        const std::size_t expected_length = full_blocks * 5 + (tail == 0 ? 0 : tail + 1);

        EXPECT_EQ(base85::encode(increasing_bytes(length)).size(), expected_length)
                << "length = " << length;
    }
}

TEST(Base85, RejectsInvalidInput)
{
    EXPECT_THROW(base85::decode(bytes_from_string("0")), std::runtime_error);
    EXPECT_THROW(base85::decode(bytes_from_string("abcde0")), std::runtime_error);
    EXPECT_THROW(base85::decode(bytes_from_string("abc de")), std::runtime_error);
    EXPECT_THROW(base85::decode(bytes_from_string("abc\n")), std::runtime_error);
    EXPECT_THROW(base85::decode(bytes_from_string("~~~~~")), std::runtime_error);
}
