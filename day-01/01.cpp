#include <fstream>
#include <string>
#include <iostream>

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
        size_t p1{ 0 };
        size_t p2{ 0 };

        for (size_t i = 0; i < line.size(); ++i)
        {
            if (std::isdigit(line[i]))
            {
                p1 = i;
                break;
            }
        }

        for (size_t i = line.size() - 1; i >= 0; --i)
        {
            if (std::isdigit(line[i]))
            {
                p2 = i;
                break;
            }
        }

        const std::string number_str = std::string{ line[p1] } + std::string{ line[p2] };

        sum += std::stoi(number_str);
    }

    std::cout << sum << '\n';

    return 0;
}