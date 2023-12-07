#include <cassert>
#include <fstream>
#include <string>
#include <execution>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <unordered_map>

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

struct THands final
{
    explicit THands(const std::string& cards, size_t bid)
    : m_cards{ cards }
    , m_bid{ bid }
    {
    }

    bool operator<(const THands& other) const
    {
        assert(m_cards.size() == other.m_cards.size());

        const size_t rank_this = Get_Cards_Type();
        const size_t rank_other = other.Get_Cards_Type();

        if (rank_this != rank_other)
        {
            return rank_this < rank_other;
        }
        else
        {
            // clang-format off
            static const std::unordered_map<char, size_t> rank_map{
                { 'J', 0 },
                { '2', 1 },
                { '3', 2 },
                { '4', 3 },
                { '5', 4 },
                { '6', 5 },
                { '7', 6 },
                { '8', 7 },
                { '9', 8 },
                { 'T', 9 },
                { 'Q', 10 },
                { 'K', 11 },
                { 'A', 12 }
            };
            // clang-format on

            for (size_t i = 0; i < m_cards.size(); ++i)
            {
                if (m_cards[i] != other.m_cards[i])
                {
                    return rank_map.at(m_cards[i]) < rank_map.at(other.m_cards[i]);
                }
            }
        }

        return false;
    }

    std::string m_cards;
    size_t m_bid;

private:
    bool Is_Five_Of_Kind(const std::unordered_map<char, size_t>& count) const
    {
        return count.size() == 1;
    }

    bool Is_Four_Of_Kind(const std::unordered_map<char, size_t>& count) const
    {
        return count.size() == 2 &&
               std::any_of(count.begin(), count.end(), [](const auto& pair) { return pair.second == 4; });
    }

    bool Is_Full_House(const std::unordered_map<char, size_t>& count) const
    {
        return count.size() == 2 &&
               std::any_of(count.begin(), count.end(), [](const auto& pair) { return pair.second == 3; });
    }

    bool Is_Three_Of_Kind(const std::unordered_map<char, size_t>& count) const
    {
        return count.size() == 3 &&
               std::any_of(count.begin(), count.end(), [](const auto& pair) { return pair.second == 3; });
    }

    bool Is_Two_Pair(const std::unordered_map<char, size_t>& count) const
    {
        return count.size() == 3 &&
               std::count_if(count.begin(), count.end(), [](const auto& pair) { return pair.second == 2; }) == 2;
    }

    bool Is_One_Pair(const std::unordered_map<char, size_t>& count) const
    {
        return count.size() == 4 &&
               std::any_of(count.begin(), count.end(), [](const auto& pair) { return pair.second == 2; });
    }

    size_t Get_Cards_Type() const
    {
        std::unordered_map<char, size_t> count;
        size_t j_count{ 0 };
        size_t max_count{ 0 };
        char max_count_card{ '2' };

        for (const auto& card : m_cards)
        {
            if (card == 'J')
            {
                ++j_count;
            }
            else
            {
                ++count[card];

                if (count[card] > max_count)
                {
                    max_count = count[card];
                    max_count_card = card;
                }
            }
        }

        while (j_count > 0)
        {
            --j_count;
            ++count[max_count_card];
        }

        if (Is_Five_Of_Kind(count))
        {
            return 7;
        }
        else if (Is_Four_Of_Kind(count))
        {
            return 6;
        }
        else if (Is_Full_House(count))
        {
            return 5;
        }
        else if (Is_Three_Of_Kind(count))
        {
            return 4;
        }
        else if (Is_Two_Pair(count))
        {
            return 3;
        }
        else if (Is_One_Pair(count))
        {
            return 2;
        }

        return 1;
    }
};

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
    std::vector<THands> hands;

    while (std::getline(file, line))
    {
        const auto parts = Split(line, ' ');
        hands.emplace_back(parts[0], std::stoll(parts[1]));
    }

    size_t result{ 0 };
    std::sort(std::execution::par_unseq, hands.begin(), hands.end());

    for (size_t i = 0; i < hands.size(); ++i)
    {
        result += (i + 1) * hands[i].m_bid;
    }

    std::cout << result << '\n';

    return 0;
}