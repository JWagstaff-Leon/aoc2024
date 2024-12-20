#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

uint32_t check_for_x_mas(
    const std::vector<std::string>& crossword)
{
    const std::string word = "MAS";
    uint32_t count = 0;
    std::pair<int, int> directions[] = {
        {-1, -1},
        { 1,  1},
        {-1,  1},
        { 1, -1}
    };
    int check_row, check_col, check_word_index;
    bool starts_m;

    for (int row = 0; row < crossword.size(); row++)
    {
        for (int col = 0; col < crossword[row].length(); col++)
        {
            if (crossword[row][col] == word[0])
            {
                for (auto direction : directions)
                {
                    check_row = row;
                    check_col = col;
                    for (check_word_index = 1; check_word_index < word.length(); check_word_index++)
                    {
                        check_row += direction.first;
                        check_col += direction.second;
                        if (check_row < 0                ||
                            check_col < 0                || 
                            check_row >= crossword.size() ||
                            check_col >= crossword[check_row].length())
                        {
                            break;
                        }

                        if (crossword[check_row][check_col] != word[check_word_index])
                        {
                            break;
                        }
                    }

                    if (check_word_index == word.length())
                    {
                        check_row = row + direction.first;
                        check_col = col + direction.second;

                        // To get the orthagonal direction, negate one of the elements of the direction
                        check_row += direction.first * -1;
                        check_col += direction.second;

                        if (check_row < 0                ||
                            check_col < 0                || 
                            check_row >= crossword.size() ||
                            check_col >= crossword[check_row].length())
                        {
                            continue;
                        }

                        if (crossword[check_row][check_col] != 'M' &&
                            crossword[check_row][check_col] != 'S')
                        {
                            continue;
                        }

                        starts_m = (crossword[check_row][check_col] == 'M');

                        // Negate both direction values to travel in the opposite but parallel direction
                        check_row += direction.first * 2;
                        check_col += direction.second * -2;
                        if (check_row < 0                ||
                            check_col < 0                || 
                            check_row >= crossword.size() ||
                            check_col >= crossword[check_row].length())
                        {
                            continue;
                        }

                        if (starts_m)
                        {
                            if (crossword[check_row][check_col] == 'S')
                            {
                                count += 1;
                            }
                        }
                        else
                        {
                            if (crossword[check_row][check_col] == 'M')
                            {
                                count += 1;
                            }
                        }
                    }
                }
            }
            // char temp;
            // std::cin >> temp;
        }
    }

    return count / 2;
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

    std::vector<std::string> input;
    std::string currentLine;
    while(!fin.eof())
    {
        std::getline(fin, currentLine);
        if (currentLine.length() > 0)
        {
            input.push_back(std::move(currentLine));
        }
    }
    fin.close();

    uint64_t answer = check_for_x_mas(input);
    std::cout << "Answer: " << answer << "\n";
    return 0;
};