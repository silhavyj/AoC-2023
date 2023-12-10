#include <cassert>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <numeric>
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

    std::vector<std::string> start_nodes;
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

        if (node[node.size() - 1] == 'A')
        {
            start_nodes.emplace_back(node);
        }
    }

    assert(start_nodes.size() != 0);

    std::vector<size_t> steps_all;
    steps_all.reserve(start_nodes.size());

    for (const auto& node : start_nodes)
    {
        std::string curr_node = node;
        size_t steps{ 0 };
        size_t direction_idx{ 0 };

        while (curr_node[curr_node.size() - 1] != 'Z')
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

        steps_all.emplace_back(steps);
    }

    size_t result{ steps_all[0] };

    for (size_t i = 1; i < steps_all.size(); ++i)
    {
        result = std::lcm(result, steps_all[i]);
    }

    std::cout << result << '\n';

    return 0;
}