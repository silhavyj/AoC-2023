#include <cassert>
#include <cstdint>
#include <queue>
#include <fstream>
#include <string>
#include <array>
#include <vector>
#include <iostream>

std::vector<std::array<int, 2>> Get_Directions(char pipe)
{
    // clang-format off
    switch (pipe)
    {
        case 'S':
            return {{ 
                { 0, -1 },
                { -1, 0 },
                { 0, 1 },
                { 1, 0 }
            }};

        case '|':
            return {{ 
                { -1, 0 },
                { 1, 0 }
            }};

        case '-':
            return {{ 
                { 0, -1 },
                { 0, 1 }
            }};

        case 'L':
            return {{ 
                { -1, 0 },
                { 0, 1 }
            }};

        case 'J':
            return {{ 
                { -1, 0 },
                { 0, -1 }
            }};

        case '7':
            return {{ 
                { 1, 0 },
                { 0, -1 }
            }};

        case 'F':
            return {{ 
                { 1, 0 },
                { 0, 1 }
            }};

        default:
            return {};
    }
    // clang-format on

    return {};
}

int main(int argc, char* argv[])
{
    if (argc < 2) [[unlikely]]
    {
        return 1;
    }

    std::ifstream file(argv[1]);

    if (!file) [[unlikely]]
    {
        return 2;
    }

    std::string line;
    std::vector<std::string> map;

    bool start_found{ false };
    size_t y{ 0 };
    size_t start_y;
    size_t start_x;

    while (std::getline(file, line))
    {
        if (!start_found)
        {
            for (size_t x = 0; x < line.length(); ++x)
            {
                if (line[x] == 'S')
                {
                    start_y = y;
                    start_x = x;
                    start_found = true;

                    break;
                }
            }
        }

        ++y;
        map.emplace_back(line);
    }

    assert(start_found);

    const size_t n = map.size();
    const size_t m = map[0].size();

    std::vector<std::vector<size_t>> graph(n * m);

    const auto ID = [&](size_t y, size_t x) -> size_t { return y * m + x; };

    const auto Add_Connection = [&](int64_t from_i, int64_t from_j, int64_t to_i, int64_t to_j) -> void {
        if (to_i < 0 || to_j < 0 || to_i >= n || to_j >= m)
        {
            return;
        }

        graph[ID(from_i, from_j)].push_back(ID(to_i, to_j));
    };

    for (int64_t i = 0; i < n; ++i)
    {
        for (int64_t j = 0; j < m; ++j)
        {
            switch (map[i][j])
            {
                case 'S':
                    Add_Connection(i, j, i - 1, j);
                    Add_Connection(i, j, i + 1, j);
                    Add_Connection(i, j, i, j - 1);
                    Add_Connection(i, j, i, j + 1);
                    break;

                case '|':
                    Add_Connection(i, j, i - 1, j);
                    Add_Connection(i, j, i + 1, j);
                    break;

                case '-':
                    Add_Connection(i, j, i, j - 1);
                    Add_Connection(i, j, i, j + 1);
                    break;

                case 'L':
                    Add_Connection(i, j, i - 1, j);
                    Add_Connection(i, j, i, j + 1);
                    break;

                case 'J':
                    Add_Connection(i, j, i - 1, j);
                    Add_Connection(i, j, i, j - 1);
                    break;

                case '7':
                    Add_Connection(i, j, i + 1, j);
                    Add_Connection(i, j, i, j - 1);
                    break;

                case 'F':
                    Add_Connection(i, j, i + 1, j);
                    Add_Connection(i, j, i, j + 1);
                    break;

                default:
                    break;
            }
        }
    }

    std::vector<std::vector<int64_t>> results(n, std::vector<int64_t>(m, -1));
    std::queue<size_t> que;

    results[start_y][start_x] = 0;
    size_t result{ 0 };

    for (const auto& neighbor : graph[ID(start_y, start_x)])
    {
        const size_t y = neighbor / m;
        const size_t x = neighbor % m;

        if (map[y][x] != '.')
        {
            for (const auto& neighbor2 : graph[neighbor])
            {
                const size_t y2 = neighbor2 / m;
                const size_t x2 = neighbor2 % m;

                if (start_x == x2 && start_y == y2)
                {
                    results[y][x] = 1;
                    que.push(neighbor);
                    break;
                }
            }
        }
    }

    while (!que.empty())
    {
        const size_t curr = que.front();
        que.pop();

        const size_t curr_y = curr / m;
        const size_t curr_x = curr % m;

        std::vector<size_t> solved_neighbors;

        result = std::max(result, static_cast<size_t>(results[curr_y][curr_x]));

        for (const auto& neighbor : graph[curr])
        {
            const size_t y = neighbor / m;
            const size_t x = neighbor % m;

            if (results[y][x] != -1)
            {
                solved_neighbors.emplace_back(results[y][x]);
            }
            else if (map[y][x] != '.')
            {
                results[y][x] = results[curr_y][curr_x] + 1;
                que.push(neighbor);
            }
        }
    }

    std::cout << result << '\n';

    return 0;
}