#include <cassert>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <sstream>

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
    std::string directions;

    std::getline(file, directions);
    std::getline(file, line);

    std::unordered_map<std::string, std::pair<std::string, std::string>> map;

    while (std::getline(file, line))
    {
        const auto parts = Split(line, '=');

        std::string node = parts[0];
        node.pop_back();

        const auto parts2 = Split(parts[1], ' ');

        assert(parts2.size() == 2);

        std::string left = parts2[0];
        std::string right = parts2[1];

        left.erase(0, 1);
        left.pop_back();
        right.pop_back();

        map[node] = { left, right };
    }

    size_t steps{ 0 };
    size_t direction_idx{ 0 };
    std::string curr_node = "AAA";

    while (curr_node != "ZZZ")
    {
        ++steps;

        if (directions[direction_idx] == 'R')
        {
            curr_node = map[curr_node].second;
        }
        else if (directions[direction_idx] == 'L')
        {
            curr_node = map[curr_node].first;
        }

        ++direction_idx;

        direction_idx %= directions.size();
    }

    std::cout << steps << '\n';

    return 0;
}