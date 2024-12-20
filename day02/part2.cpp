#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

enum LevelDirection
{
    Increasing,
    Decreasing,
    ReadingFirst,
    ReadingSecond,
};

struct LevelCheckResult
{
    bool success;
    size_t error_pos;
};

bool check_levels(
    int32_t previous_level,
    int32_t current_level,
    LevelDirection direction)
{
    if (ReadingFirst == direction)
    {
        return true;
    }

    if (abs(current_level - previous_level) > 3 || current_level == previous_level)
    {
        return false;
    }

    switch (direction)
    {
        case Increasing:
        {
            if (current_level < previous_level)
            {
               return false;
            }
            break;
        }
        case Decreasing:
        {
            if (current_level > previous_level)
            {
                return false;
            }
            break;
        }
    }

    return true;
}

LevelCheckResult check_all_levels(std::vector<int32_t> levels)
{
    int32_t previous_level = 0;
    LevelDirection direction = ReadingFirst;
    LevelCheckResult result;
    result.success = true;
    for (size_t i = 0; i < levels.size(); i++)
    {
        int32_t level = levels[i];
        if(!check_levels(previous_level, level, direction))
        {
            result.success = false;
            result.error_pos = i;
            return result;
        }

        switch (direction)
        {
            case ReadingFirst:
            {
                direction = ReadingSecond;
                break;
            }
            case ReadingSecond:
            {
                if (level > previous_level)
                {
                    direction = Increasing;
                }
                else if (level < previous_level)
                {
                    direction = Decreasing;
                }
                else
                {
                    result.success = false;
                    result.error_pos = i;
                    return result;
                }
                break;
            }
        }
        previous_level = level;
    }

    return result;
}

bool check_all_levels_dampened(std::vector<int32_t> levels)
{
    LevelCheckResult result = check_all_levels(levels);
    if (result.success ||
        result.error_pos >= levels.size() - 1) // Errors at the end are free to remove
    {
        return true;
    }
    std::vector<int32_t> reduced_levels;
    for (size_t i = 0; i < levels.size(); i++) // Remove the index where the error occured
    {
        if (result.error_pos != i)
        {
            reduced_levels.push_back(levels[i]);
        }
    }

    LevelCheckResult reduced_result = check_all_levels(reduced_levels);
    if (reduced_result.success)
    {
        return true;
    }
    reduced_levels.clear();
    for (size_t i = 0; i < levels.size(); i++) // Remove the index previous to where the error occured
    {
        if (result.error_pos - 1 != i)
        {
            reduced_levels.push_back(levels[i]);
        }
    }
    reduced_result = check_all_levels(reduced_levels);
    if (reduced_result.success)
    {
        return true;
    }

    if (2 == result.error_pos) // In this case removing the very first level could change directionality
    {
        reduced_levels.clear();
        for (size_t i = 1; i < levels.size(); i++) // Remove the very first level
        {
            reduced_levels.push_back(levels[i]);
        }
        reduced_result = check_all_levels(reduced_levels);
        return reduced_result.success;
    }
    else
    {
        return reduced_result.success;
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

    uint64_t answer = 0;
    while(!fin.eof())
    {
        std::vector<int32_t> levels;
        std::string input;
        std::getline(fin, input);
        size_t next_space, previous_space = 0;

        if (input.length() < 1)
        {
            continue;
        }

        while(true)
        {
            next_space = input.find(' ', previous_space);
            if (std::string::npos == next_space)
            {
                levels.push_back(std::stoi(input.substr(previous_space, next_space - previous_space)));
                break;
            }

            levels.push_back(std::stoi(input.substr(previous_space, next_space - previous_space)));
            previous_space = next_space + 1;
        }
        if (check_all_levels_dampened(levels))
        {
            answer += 1;
        }
    }
    fin.close();

    std::cout << "Answer: " << answer << std::endl;
    return 0;
};
