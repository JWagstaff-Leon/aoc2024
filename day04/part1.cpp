#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

uint32_t check_for_word(
    const std::vector<std::string>& crossword,
    const std::string&              word)
{
    uint32_t count = 0;
    std::pair<int, int> directions[] = {
        {-1, -1},
        {-1,  0},
        {-1,  1},
        { 0, -1},
        { 0,  1},
        { 1, -1},
        { 1,  0},
        { 1,  1}
    };
    int check_row, check_col, check_word_index;

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
                        count += 1;
                    }
                }
            }
        }
    }

    return count;
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

    uint64_t answer = check_for_word(input, "XMAS");
    std::cout << "Answer: " << answer << "\n";
    return 0;
};