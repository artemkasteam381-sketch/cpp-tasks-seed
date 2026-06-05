#include "base85ed.h"

#include <cstdint>
#include <exception>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

std::vector<uint8_t> read_stdin_to_vector()
{
    constexpr std::streamsize buffer_size = 64 * 1024;
    std::vector<uint8_t> data;
    std::vector<char> buffer(buffer_size);

    std::ios::sync_with_stdio(false);

    while (std::cin)
    {
        std::cin.read(buffer.data(), buffer_size);
        const std::streamsize count = std::cin.gcount();

        if (count > 0)
        {
            data.insert(data.end(), buffer.begin(), buffer.begin() + count);
        }
    }

    return data;
}

void write_vector_to_stdout(const std::vector<uint8_t>& data)
{
    if (!data.empty())
    {
        std::cout.write(reinterpret_cast<const char*>(data.data()),
                        static_cast<std::streamsize>(data.size()));
    }
    std::cout.flush();
}

int main(int argc, const char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Use -e or -d argument\n";
        return 1;
    }

    const std::string mode = argv[1];
    std::function<std::vector<uint8_t>(const std::vector<uint8_t>&)> action;

    if (mode == "-e" || mode == "--encode")
    {
        action = base85::encode;
    }
    else if (mode == "-d" || mode == "--decode")
    {
        action = base85::decode;
    }
    else
    {
        std::cerr << "Use -e or -d argument\n";
        return 1;
    }

    try
    {
        write_vector_to_stdout(action(read_stdin_to_vector()));
    }
    catch (const std::exception& error)
    {
        std::cerr << error.what() << '\n';
        return 1;
    }

    return 0;
}
