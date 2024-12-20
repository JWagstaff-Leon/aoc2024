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
    bool traversed_up;
    bool traversed_down;
    bool traversed_left;
    bool traversed_right;
};

bool does_guard_leave_grid(
    std::vector<std::vector<GridSpace>> grid,
    Guard guard)
{
    while (guard.row >= 0 && guard.row < grid.size() &&
           guard.col >= 0 && guard.col < grid[guard.row].size())
    {
        switch (guard.direction)
        {
            case UP:
            {
                if (grid[guard.row][guard.col].traversed_up)
                {
                    return false;
                }
                grid[guard.row][guard.col].traversed_up = true;
                if (guard.row > 0 && grid[guard.row - 1][guard.col].is_block)
                {
                    guard.direction = RIGHT;
                }
                else
                {
                    guard.row -= 1;
                }
                break;
            }
            case DOWN:
            {
                if (grid[guard.row][guard.col].traversed_down)
                {
                    return false;
                }
                grid[guard.row][guard.col].traversed_down = true;
                if (guard.row + 1 < grid.size() && grid[guard.row + 1][guard.col].is_block)
                {
                    guard.direction = LEFT;
                }
                else
                {
                    guard.row += 1;
                }
                break;
            }
            case LEFT:
            {
                if (grid[guard.row][guard.col].traversed_left)
                {
                    return false;
                }
                grid[guard.row][guard.col].traversed_left = true;
                if (guard.col > 0 && grid[guard.row][guard.col - 1].is_block)
                {
                    guard.direction = UP;
                }
                else
                {
                    guard.col -= 1;
                }
                break;
            }
            case RIGHT:
            {
                if (grid[guard.row][guard.col].traversed_right)
                {
                    return false;
                }
                grid[guard.row][guard.col].traversed_right = true;
                if (guard.col + 1 < grid[guard.row].size() && grid[guard.row][guard.col + 1].is_block)
                {
                    guard.direction = DOWN;
                }
                else
                {
                    guard.col += 1;
                }
                break;
            }

        }
    }
    return true;
}


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
            current_grid_space.is_block        = false;
            current_grid_space.traversed       = false;
            current_grid_space.traversed_up    = false;
            current_grid_space.traversed_down  = false;
            current_grid_space.traversed_left  = false;
            current_grid_space.traversed_right = false;

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
    
    Guard guard_start = guard;
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
    for (row = 0; row < grid.size(); row++)
    {
        for (col = 0; col < grid[row].size(); col++)
        {
            if (grid[row][col].traversed)
            {
                if(row == guard_start.row && col == guard_start.col)
                {
                    continue;
                }
                grid[row][col].is_block = true;
                if (!does_guard_leave_grid(grid, guard_start))
                {
                    answer += 1;
                }
                grid[row][col].is_block = false;
            }
        }
    }
    std::cout << "Answer: " << answer << "\n";
    return 0;
};
