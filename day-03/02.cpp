#include <fstream>
#include <functional>
#include <string>
#include <array>
#include <vector>
#include <iostream>
#include <utility>
#include <unordered_set>
#include <cassert>

// clang-format off
static constexpr std::array<std::array<int, 2>, 8> directions = {{
    { -1, -1 },
    { -1, 0 },
    { -1, 1 },
    { 0, 1 },
    { 1, 1 },
    { 1, 0 },
    { 1, -1 },
    { 0, -1 } 
}};
// clang-format on

struct TNumber
{
    int i;
    size_t start;
    size_t stop;

    bool operator==(const TNumber& other) const
    {
        return i == other.i && start == other.start && stop == other.stop;
    }
};

struct TNumber_Hash
{
    const size_t operator()(const TNumber& number) const
    {
        size_t res = 31;

        res = 17 * res + std::hash<size_t>{}(number.i);
        res = 17 * res + std::hash<size_t>{}(number.start);
        res = 17 * res + std::hash<size_t>{}(number.stop);

        return res;
    }
};

size_t Get_Value(const std::vector<std::string>& schematic, const TNumber& number)
{
    size_t value{ 0 };

    for (size_t j = number.start; j <= number.stop; ++j)
    {
        value += (schematic[number.i][j] - '0') * std::pow(10, number.stop - j);
    }

    return value;
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
    std::vector<std::string> schematic;

    while (std::getline(file, line))
    {
        schematic.push_back(line);
    }

    std::vector<TNumber> numbers;
    std::vector<std::pair<int, int>> stars;

    for (int i = 0; i < schematic.size(); ++i)
    {
        size_t start{ 0 };
        size_t stop{ 0 };
        bool in_number{ false };

        for (int j = 0; j < schematic[i].size(); ++j)
        {
            if (schematic[i][j] == '*')
            {
                stars.push_back({ i, j });
            }

            if (!in_number && std::isdigit(schematic[i][j]))
            {
                in_number = true;
                start = j;
            }

            if (in_number && !std::isdigit(schematic[i][j]))
            {
                in_number = false;
                stop = j - 1;
                numbers.push_back({ i, start, stop });
            }
        }

        if (in_number)
        {
            stop = schematic[i].size() - 1;
            numbers.push_back({ i, start, stop });
        }
    }

    for (const auto& [star_y, star_x] : stars)
    {
        size_t product{ 1 };
        size_t connected_nums{ 0 };
        std::unordered_set<TNumber, TNumber_Hash> seen;

        for (const auto& number : numbers)
        {
            for (size_t k = 0; k < directions.size(); ++k)
            {
                const int y = star_y + directions[k][0];
                const int x = star_x + directions[k][1];

                if (y < 0 || x < 0 || y >= schematic.size() || x >= schematic[0].size())
                {
                    continue;
                }

                if (y == number.i && (x >= number.start && x <= number.stop) && !seen.count(number))
                {
                    seen.insert(number);
                    ++connected_nums;
                    const auto value = Get_Value(schematic, number);

                    product *= value;
                }
            }
        }

        assert(connected_nums <= 2);

        if (connected_nums == 2)
        {
            sum += product;
        }
    }

    std::cout << sum << '\n';

    return 0;
}