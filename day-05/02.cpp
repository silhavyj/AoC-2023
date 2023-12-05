#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cassert>
#include <ranges>

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

    std::stringstream buffer;
    buffer << file.rdbuf();

    const auto maps = Split(buffer.str(), '\n');

    std::vector<size_t> result{};

    const auto seed_ranges = Split(Split(maps[0], ':')[1], ' ');

    assert(seed_ranges.size() % 2 == 0);

    for (size_t i = 0; i < seed_ranges.size(); i += 2)
    {
        const size_t start = std::stoll(seed_ranges[i]);
        const size_t count = std::stoll(seed_ranges[i + 1]);

        for (size_t seed_idx = start; seed_idx <= start + count; ++seed_idx)
        {
            result.emplace_back(seed_idx);
        }
    }

    assert(maps.size() > 2);

    std::vector<bool> converted(result.size(), false);

    for (size_t i = 2; i < maps.size(); ++i)
    {
        if (maps[i].empty())
        {
            continue;
        }

        if (!std::isdigit(maps[i][0]))
        {
            converted = std::vector<bool>(result.size(), false);
            continue;
        }

        const auto record = Split(maps[i], ' ');

        const size_t dest_idx = std::stoll(record[0]);
        const size_t src_idx = std::stoll(record[1]);
        const size_t size = std::stoll(record[2]);

        for (size_t j = 0; j < result.size(); ++j)
        {
            if (converted[j])
            {
                continue;
            }

            if (result[j] >= src_idx && result[j] <= (src_idx + size))
            {
                converted[j] = true;
                result[j] = dest_idx + (result[j] - src_idx);
            }
        }
    }

    std::cout << std::ranges::min(result) << '\n';

    return 0;
}