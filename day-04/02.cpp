#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <unordered_set>
#include <unordered_map>
#include <cassert>

std::vector<std::string> Split(const std::string& str, char c)
{
    std::vector<std::string> parts;

    std::stringstream ss{ str };
    std::string part;

    while (getline(ss, part, c))
    {
        if (part != "")
        {
            parts.emplace_back(part);
        }
    }

    return parts;
}

std::pair<size_t, std::unordered_set<size_t>> Parse_Winning_Nums(const std::string& first_chunk)
{
    std::unordered_set<size_t> winning_numbers;

    const auto parts_1 = Split(first_chunk, ':');
    const auto parts_2 = Split(parts_1[1], ' ');

    for (const auto& part : parts_2)
    {
        const auto number = std::atoi(part.c_str());
        assert(winning_numbers.count(number) == 0);
        winning_numbers.insert(number);
    }

    const auto parts_3 = Split(parts_1[0].c_str(), ' ');
    const auto card_id = std::atoi(parts_3[1].c_str());

    return { card_id, winning_numbers };
}

int main([[maybe_unused]] int argc, char* argv[])
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
    std::unordered_map<size_t, size_t> copies;

    while (std::getline(file, line))
    {
        const auto parts_1 = Split(line, '|');
        const auto [card_id, winning_numbers] = Parse_Winning_Nums(parts_1[0]);
        const auto parts_2 = Split(parts_1[1], ' ');

        size_t count{ 0 };

        ++copies[card_id];

        for (const auto& part : parts_2)
        {
            const auto number = std::atoi(part.c_str());

            if (winning_numbers.count(number))
            {
                ++count;
                copies[card_id + count] += copies[card_id];
            }
        }

        sum += copies[card_id];
    }

    std::cout << sum << '\n';

    return 0;
}
