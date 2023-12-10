#include <cassert>
#include <cstdint>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>

std::vector<int64_t> Get_History(const std::string& str, char c)
{
    std::vector<int64_t> parts;

    std::stringstream ss{ str };
    std::string part;

    while (getline(ss, part, c))
    {
        if (part != "")
        {
            parts.emplace_back(std::atoll(part.c_str()));
        }
    }

    return parts;
}

int64_t Calculate_Prediction(const std::vector<int64_t>& history)
{
    bool all_zeros{ false };
    std::vector<int64_t> last_differences = history;
    std::vector<int64_t> first_values;

    while (!all_zeros)
    {
        all_zeros = true;
        std::vector<int64_t> new_differences;

        for (size_t i = 1; i < last_differences.size(); ++i)
        {
            const auto diff = last_differences[i] - last_differences[i - 1];
            new_differences.emplace_back(diff);

            if (diff != 0)
            {
                all_zeros = false;
            }
        }

        first_values.emplace_back(new_differences[0]);

        last_differences = new_differences;
    }

    std::vector<int64_t> predictions(first_values.size(), 0);

    for (int64_t i = predictions.size() - 2; i >= 0; --i)
    {
        predictions[i] = first_values[i] - predictions[i + 1];
    }

    const int64_t prediction = history[0] - predictions[0];

    return prediction;
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
    int64_t sum{ 0 };

    while (std::getline(file, line))
    {
        const auto history = Get_History(line, ' ');
        sum += Calculate_Prediction(history);
    }

    std::cout << sum << '\n';

    return 0;
}