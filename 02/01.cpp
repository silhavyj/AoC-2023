#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>

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

        bool possible{ true };

        for (const auto& part : parts_2)
        {
            auto parts_3 = Split(part, ' ');

            for (size_t i = 0; i < parts_3.size(); i += 2)
            {
                if (parts_3[i + 1][parts_3[i + 1].size() - 1] == ',')
                {
                    parts_3[i + 1].pop_back();
                }

                if ((parts_3[i + 1] == "red" && std::atoi(parts_3[i].c_str()) > 12) ||
                    (parts_3[i + 1] == "green" && std::atoi(parts_3[i].c_str()) > 13) ||
                    (parts_3[i + 1] == "blue" && std::atoi(parts_3[i].c_str()) > 14))
                {
                    possible = false;
                    break;
                }
            }

            if (!possible)
            {
                break;
            }
        }

        if (possible)
        {
            const auto parts_4 = Split(parts_1[0], ' ');
            const auto id = std::atoi(parts_4[1].c_str());

            sum += id;
        }
    }

    std::cout << sum << '\n';

    return 0;
}