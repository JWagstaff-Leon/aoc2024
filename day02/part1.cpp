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

        int32_t previous_level = 0;
        LevelDirection direction = ReadingFirst;
        bool valid_levels = true;
        for(auto level : levels)
        {
            if (ReadingFirst == direction)
            {
                previous_level = level;
                direction = ReadingSecond;

                continue;
            }

            if (abs(level - previous_level) > 3 || level == previous_level)
            {
                valid_levels = false;
                break;
            }

            if (ReadingSecond == direction)
            {
                if (level > previous_level)
                {
                    direction = Increasing;
                }
                else
                {
                    direction = Decreasing;
                }

                previous_level = level;
                continue;
            }

            switch (direction)
            {
                case Increasing:
                {
                    if (level < previous_level)
                    {
                        valid_levels = false;
                    }
                    break;
                }
                case Decreasing:
                {
                    if (level > previous_level)
                    {
                        valid_levels = false;
                    }
                    break;
                }
            }

            if (!valid_levels)
            {
                break;
            }

            previous_level = level;
        }

        if (valid_levels)
        {
            answer += 1;
        }
    }
    fin.close();

    std::cout << "Answer: " << answer << std::endl;
    return 0;
};
