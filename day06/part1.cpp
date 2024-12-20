#include <cstdint>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>

enum FacingDirection
{
    UP, DOWN, LEFT, RIGHT
};

struct Guard
{
    int row, col;
    FacingDirection direction;
};

struct GridSpace
{
    bool is_block, traversed;
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

    std::vector<std::vector<GridSpace>> grid;
    std::string current_line;

    Guard guard;
    int row = 0;
    int col = 0;

    while(!fin.eof())
    {
        std::vector<GridSpace> current_grid_line;
        GridSpace              current_grid_space;
        std::getline(fin, current_line);
        if(current_line.length() < 1)
        {
            continue;
        }

        col = 0;
        for (auto space : current_line)
        {
            current_grid_space.is_block  = false;
            current_grid_space.traversed = false;

            switch(space)
            {
                case '#':
                {
                    current_grid_space.is_block = true;
                    break;
                }
                case '^':
                {
                    guard.row = row;
                    guard.col = col;
                    guard.direction = UP;
                    break;
                }
                case '<':
                {
                    guard.row = row;
                    guard.col = col;
                    guard.direction = LEFT;
                    break;
                }
                case '>':
                {
                    guard.row = row;
                    guard.col = col;
                    guard.direction = RIGHT;
                    break;
                }
                case 'V':
                {
                    guard.row = row;
                    guard.col = col;
                    guard.direction = DOWN;
                    break;
                }
            }

            
            current_grid_line.push_back(current_grid_space);
            col++;
        }

        grid.push_back(std::move(current_grid_line));
        row++;
    }
    fin.close();
    while (guard.row >= 0 && guard.row < grid.size() &&
           guard.col >= 0 && guard.col < grid[guard.row].size())
    {
        grid[guard.row][guard.col].traversed = true;
        switch (guard.direction)
        {
            case UP:
            {
                if (guard.row > 1 && grid[guard.row - 1][guard.col].is_block)
                {
                    guard.direction = RIGHT;
                    continue;
                }
                guard.row -= 1;
                break;
            }
            case DOWN:
            {
                if (guard.row + 1 < grid.size() && grid[guard.row + 1][guard.col].is_block)
                {
                    guard.direction = LEFT;
                    continue;
                }
                guard.row += 1;
                break;
            }
            case LEFT:
            {
                if (guard.col > 1 && grid[guard.row][guard.col - 1].is_block)
                {
                    guard.direction = UP;
                    continue;
                }
                guard.col -= 1;
                break;
            }
            case RIGHT:
            {
                if (guard.col + 1 < grid[guard.row].size() && grid[guard.row][guard.col + 1].is_block)
                {
                    guard.direction = DOWN;
                    continue;
                }
                guard.col += 1;
                break;
            }
        }
    }

    uint64_t answer = 0;
    for (auto line : grid)
    {
        for (auto space : line)
        {
            if (space.traversed)
            {
                answer += 1;
            }
        }
    }
    std::cout << "Answer: " << answer << "\n";
    return 0;
};
