#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <unordered_set>
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

std::unordered_set<size_t> Parse_Winning_Nums(const std::string& first_chunk)
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

    return winning_numbers;
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

    while (std::getline(file, line))
    {
        const auto parts_1 = Split(line, '|');
        const auto winning_numbers = Parse_Winning_Nums(parts_1[0]);
        const auto parts_2 = Split(parts_1[1], ' ');

        bool first_winning_number{ true };
        size_t points{ 0 };

        for (const auto& part : parts_2)
        {
            const auto number = std::atoi(part.c_str());

            if (winning_numbers.count(number))
            {
                if (first_winning_number)
                {
                    points = 1;
                    first_winning_number = false;
                }
                else
                {
                    points *= 2;
                }
            }
        }

        sum += points;
    }

    std::cout << sum << '\n';

    return 0;
}