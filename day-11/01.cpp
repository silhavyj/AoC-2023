#include <corecrt.h>
#include <cstddef>
#include <cstdint>
#include <queue>
#include <fstream>
#include <string>
#include <array>
#include <vector>
#include <iostream>

// clang-format off
constexpr std::array<std::array<int, 2>, 4> directions = {{ 
    { 0, -1 },
    { -1, 0 },
    { 0, 1 },
    { 1, 0 }
}};
// clang-format on

int main(int argc, char* argv[])
{
    if (argc < 2) [[unlikely]]
    {
        return 1;
    }

    std::vector<std::string> image;

    {
        std::ifstream file(argv[1]);

        if (!file) [[unlikely]]
        {
            return 2;
        }

        std::string line;

        while (std::getline(file, line))
        {
            image.push_back(line);
        }
    }

    std::vector<std::pair<size_t, size_t>> galaxies;
    std::vector<bool> rows(image.size(), true);
    std::vector<bool> cols(image[0].size(), true);

    const size_t n = image.size();
    const size_t m = image[0].size();

    for (size_t i = 0; i < n; ++i)
    {
        for (size_t j = 0; j < m; ++j)
        {
            if (image[i][j] == '#')
            {
                rows[i] = false;
                cols[j] = false;
                galaxies.push_back({ i, j });
            }
        }
    }

    for (size_t i = 0; i < rows.size(); ++i)
    {
        if (rows[i])
        {
            for (size_t j = 0; j < m; j++)
            {
                image[i][j] = '*';
            }
        }
    }

    for (size_t j = 0; j < cols.size(); ++j)
    {
        if (cols[j])
        {
            for (size_t i = 0; i < n; ++i)
            {
                image[i][j] = '*';
            }
        }
    }

    size_t sum{ 0 };

    const auto ID = [&](size_t i, size_t j) -> size_t { return i * m + j; };

    for (size_t i = 0; i < galaxies.size(); ++i)
    {
        for (size_t j = i + 1; j < galaxies.size(); ++j)
        {
            const size_t id_start = ID(galaxies[i].first, galaxies[i].second);
            const size_t id_finish = ID(galaxies[j].first, galaxies[j].second);

            std::queue<size_t> que;
            std::vector<int64_t> path(n * m, -1);

            que.push(id_start);
            path[id_start] = id_start;

            while (!que.empty())
            {
                const size_t curr_id = que.front();
                que.pop();

                const size_t curr_y = curr_id / m;
                const size_t curr_x = curr_id % m;

                if (curr_id == id_finish)
                {
                    size_t id = curr_id;
                    size_t distance{ 0 };

                    while (path[id] != id)
                    {
                        const size_t y = id / m;
                        const size_t x = id % m;

                        if (image[y][x] == '*')
                        {
                            distance += 1'000'000;
                        }
                        else
                        {
                            ++distance;
                        }

                        id = path[id];
                    }

                    sum += distance;
                    break;
                }

                for (const auto& [move_y, move_x] : directions)
                {
                    const int64_t y = curr_y + move_y;
                    const int64_t x = curr_x + move_x;

                    if (y < 0 || y >= n || x < 0 || x >= m)
                    {
                        continue;
                    }

                    const size_t id = ID(y, x);

                    if (path[id] == -1)
                    {
                        path[id] = curr_id;
                        que.push(id);
                    }
                }
            }
        }
    }

    std::cout << sum << '\n';

    return 0;
}