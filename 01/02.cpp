#include <cctype>
#include <array>
#include <fstream>
#include <string>
#include <iostream>
#include <unordered_map>

const std::array<std::string, 9> words{ "one", "two", "three", "four", "five", "six", "seven", "eight", "nine" };
const std::array<std::string, 9> words_reversed{
    "eno", "owt", "eerht", "ruof", "evif", "xis", "neves", "thgie", "enin"
};

size_t Find_First(const std::string& str)
{
    std::unordered_map<size_t, size_t> mp;

    for (size_t i = 0; i < words.size(); ++i)
    {
        mp[i] = 0;
    }

    for (size_t i = 0; i < str.size(); ++i)
    {
        if (std::isdigit(str[i]))
        {
            return str[i] - '0';
        }

        for (size_t j = 0; j < words.size(); ++j)
        {
            if (str[i] == words[j][mp[j]])
            {
                ++mp[j];

                if (mp[j] == words[j].size())
                {
                    return j + 1;
                }
            }
        }
    }

    return 0;
}

size_t Find_Last(const std::string& str)
{
    std::unordered_map<size_t, size_t> mp;

    for (size_t i = 0; i < words_reversed.size(); ++i)
    {
        mp[i] = 0;
    }

    for (size_t i = str.size() - 1; i >= 0; --i)
    {
        if (std::isdigit(str[i]))
        {
            return str[i] - '0';
        }

        for (size_t j = 0; j < words_reversed.size(); ++j)
        {
            if (str[i] == words_reversed[j][mp[j]])
            {
                ++mp[j];

                if (mp[j] == words_reversed[j].size())
                {
                    return j + 1;
                }
            }
        }
    }

    return 0;
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        return 1;
    }

    std::ifstream file(argv[1]);

    if (!file)
    {
        return 2;
    }

    std::string line;
    size_t sum{ 0 };

    while (std::getline(file, line))
    {
        const size_t digit_1 = Find_First(line);
        const size_t digit_2 = Find_Last(line);

        // std::cout << digit_1 << ' ' << digit_2 << '\n';

        sum += digit_1 * 10 + digit_2;
    }

    std::cout << sum << '\n';

    return 0;
}