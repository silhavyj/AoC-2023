#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
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

size_t Find_Min(size_t time, size_t distance, size_t lhs, size_t rhs)
{
    while (lhs < rhs)
    {
        const size_t mid = lhs + (rhs - lhs) / 2;

        if (mid * (time - mid) < distance)
        {
            lhs = mid + 1;
        }
        else
        {
            rhs = mid;
        }
    }

    while (lhs * (time - lhs) <= distance)
    {
        ++lhs;
    }

    return lhs;
}

size_t Find_Max(size_t time, size_t distance, size_t lhs, size_t rhs)
{
    while (lhs < rhs)
    {
        const size_t mid = lhs + (rhs - lhs) / 2;

        if (mid * (time - mid) >= distance)
        {
            lhs = mid + 1;
        }
        else
        {
            rhs = mid;
        }
    }

    while (rhs * (time - rhs) <= distance)
    {
        --rhs;
    }

    return rhs;
}

int main([[maybe_unused]] int argc, char* argv[])
{
    if (argc < 2)
    {
        return 1;
    }

    std::string time_line;
    std::string distance_line;

    {
        std::ifstream file(argv[1]);

        if (!file)
        {
            return 2;
        }

        std::getline(file, time_line);
        std::getline(file, distance_line);
    }

    const auto times = Split(Split(time_line, ':')[1], ' ');
    const auto distances = Split(Split(distance_line, ':')[1], ' ');

    assert(times.size() == distances.size());

    size_t result{ 1 };

    for (size_t i = 0; i < times.size(); ++i)
    {
        const size_t time = std::stoll(times[i]);
        const size_t distance = std::stoll(distances[i]);

        const size_t half_total_time = time / 2;

        const size_t min_time = Find_Min(time, distance, 0, half_total_time);
        const size_t max_time = Find_Max(time, distance, half_total_time + 1, time);

        result *= (max_time - min_time + 1);
    }

    std::cout << result << '\n';

    return 0;
}