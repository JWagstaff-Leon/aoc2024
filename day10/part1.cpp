#include <cstdint>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>

struct Coord
{
    uint8_t row, col;
};

const std::vector<Coord> directions = {
    {
        .row = -1,
        .col = -1
    },
    {
        .row = -1,
        .col = 1
    },
    {
        .row = 1,
        .col = -1
    },
    {
        .row = 1,
        .col = 1
    },
};

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


    std::vector<std::string> grid;
    std::string              current_line;
    std::vector<Coord>       trailheads;
    while(!fin.eof())
    {
        std::getline(fin, current_line);
        if(current_line.length() == 0)
        {
            continue;
        }

        grid.push_back(std::move(current_line));
    }
    fin.close();

    for (uint8_t row = 0; row < grid.size(); row++)
    {
        for (uint8_t col = 0; col < grid[row].length(); col++)
        {
            if (grid[row][col] == '0')
            {
                trailheads.push_back(Coord{.row = row, .col = col});
            }
        }
    }

    for (auto trailhead : trailheads)
    {
        std::set<Coord>                   processed_coords;
        std::set<std::tuple<Coord, char>> to_process_coords;
        std::set<std::tuple<Coord, char>> next_process_coords;

        processed_coords.insert(trailhead);

        for ()
    }

    return 0;
};
