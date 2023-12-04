#include <fstream>
#include <string>
#include <array>
#include <vector>
#include <iostream>
#include <utility>
#include <unordered_map>

static constexpr std::array<std::array<int, 2>, 8> direction = {
    { { -1, -1 }, { -1, 0 }, { -1, 1 }, { 0, 1 }, { 1, 1 }, { 1, 0 }, { 1, -1 }, { 0, -1 } }
};

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

    std::unordered_map<int, std::vector<std::pair<size_t, size_t>>> numbers;

    for (int i = 0; i < schematic.size(); ++i)
    {
        size_t start{ 0 };
        size_t stop{ 0 };
        bool in_number{ false };

        for (int j = 0; j < schematic[i].size(); ++j)
        {
            if (!in_number && std::isdigit(schematic[i][j]))
            {
                in_number = true;
                start = j;
            }

            if (in_number && !std::isdigit(schematic[i][j]))
            {
                in_number = false;
                stop = j - 1;
                numbers[i].push_back({ start, stop });
            }
        }

        if (in_number)
        {
            stop = schematic[i].size() - 1;
            numbers[i].push_back({ start, stop });
        }
    }

    for (const auto& [i, nums] : numbers)
    {
        for (const auto& [start, stop] : nums)
        {
            bool isolated{ true };
            size_t value{ 0 };

            for (int j = start; j <= stop; ++j)
            {
                for (size_t k = 0; k < direction.size(); ++k)
                {
                    const int y = i + direction[k][0];
                    const int x = j + direction[k][1];

                    if (y < 0 || x < 0 || y >= schematic.size() || x >= schematic[0].size())
                    {
                        continue;
                    }

                    if (!std::isdigit(schematic[y][x]) && schematic[y][x] != '.')
                    {
                        isolated = false;
                        break;
                    }
                }

                value += (schematic[i][j] - '0') * std::pow(10, stop - j);
            }

            if (!isolated)
            {
                sum += value;
            }
        }
    }

    std::cout << sum << '\n';

    return 0;
}