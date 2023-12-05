#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <algorithm>

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

    std::string line;
    size_t sum{ 0 };

    while (std::getline(file, line))
    {
        const auto parts_1 = Split(line, ':');
        const auto parts_2 = Split(parts_1[1], ';');

        size_t red_min{ 0 };
        size_t green_min{ 0 };
        size_t blue_min{ 0 };

        for (const auto& part : parts_2)
        {
            auto parts_3 = Split(part, ' ');

            for (size_t i = 0; i < parts_3.size(); i += 2)
            {
                if (parts_3[i + 1][parts_3[i + 1].size() - 1] == ',')
                {
                    parts_3[i + 1].pop_back();
                }

                const auto amount = static_cast<size_t>(std::atoi(parts_3[i].c_str()));

                if (parts_3[i + 1] == "red")
                {
                    red_min = std::max(red_min, amount);
                }
                else if (parts_3[i + 1] == "green")
                {
                    green_min = std::max(green_min, amount);
                }
                else if (parts_3[i + 1] == "blue")
                {
                    blue_min = std::max(blue_min, amount);
                }
                else
                {
                    return 3;
                }
            }
        }

        sum += (red_min * green_min * blue_min);
    }

    std::cout << sum << '\n';

    return 0;
}