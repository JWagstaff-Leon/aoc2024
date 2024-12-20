#include <cctype>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " <input filename>" << std::endl;
        return 0;
    }

    std::ifstream fin(argv[1]);
    if (fin.fail())
    {
        std::cout << "Could not open file " << argv[1] << std::endl;
        return 0;
    }

    std::map<char, std::vector<std::pair<size_t, size_t>>> antennae;
    std::set<std::pair<size_t, size_t>> antinodes;

    std::string current_line;
    size_t row = 0;
    size_t col = 0;
    while(!fin.eof())
    {
        std::getline(fin, current_line);
        if (current_line.length() < 1)
        {
            continue;
        }

        for (col = 0; col < current_line.length(); col++)
        {
            if (isalnum(current_line[col]))
            {
                antennae.try_emplace(current_line[col]);
                auto antenna_it = antennae.find(current_line[col]);
                antenna_it->second.push_back({row, col});
            }
        }
        row++;
    }
    fin.close();

    for (auto antenna_list : antennae)
    {
        for (size_t first_antenna_index = 0; first_antenna_index < antenna_list.second.size() - 1; first_antenna_index++)
        {
            for (size_t second_antenna_index = first_antenna_index + 1; second_antenna_index < antenna_list.second.size(); second_antenna_index++)
            {
                std::pair<size_t, size_t> antenna_1 = antenna_list.second[first_antenna_index];
                std::pair<size_t, size_t> antenna_2 = antenna_list.second[second_antenna_index];
                std::pair<size_t, size_t> distance = {antenna_1.first - antenna_2.first,
                                                      antenna_1.second - antenna_2.second};
                std::pair<size_t, size_t> antinode_1 = { antenna_1.first + distance.first,
                                                         antenna_1.second + distance.second };
                std::pair<size_t, size_t> antinode_2 = { antenna_2.first - distance.first,
                                                         antenna_2.second - distance.second };
                if (antinode_1.first >= 0 && antinode_1.first < row &&
                    antinode_1.second >= 0 && antinode_1.second < col)
                {
                    antinodes.insert(antinode_1);
                }
                if (antinode_2.first >= 0 && antinode_2.first < row &&
                    antinode_2.second >= 0 && antinode_2.second < col)
                {
                    antinodes.insert(antinode_2);
                }
            }
        }
    }

    std::cout << "Answer: " << antinodes.size() << "\n";
    return 0;
};
