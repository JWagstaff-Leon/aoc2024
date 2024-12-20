#include <cctype>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

static bool do_muls = true;

std::pair<int32_t, bool> extract_number(
    const std::string& string,
    uint32_t&          pos)
{
    int32_t result = 0;
    bool negative = false;
    if ('-' == string[pos])
    {
        negative = true;
        pos += 1;
    }

    while (isdigit(string[pos]))
    {
        if (pos > string.length())
        {
            return std::make_pair(0, false);
        }

        result *= 10;
        result += string[pos] - '0';
        pos++;
    }

    if (negative)
    {
        result *= -1;
    }

    return std::make_pair(result, true);
}

std::pair<int32_t, bool> perform_mul(
    const std::string& string,
    uint32_t&          pos)
{
    int32_t  result             = 0;
    uint32_t current_character  = pos;

    std::pair<int32_t, bool> extract_result;

    if (string[current_character] != '(')
    {
        return std::make_pair(0, false);
    }
    current_character += 1;
    if (current_character > string.length())
    {
        return std::make_pair(0, false);
    }

    extract_result = extract_number(string, current_character);
    if (extract_result.second)
    {
        result = extract_result.first;
    }
    else
    {
        return std::make_pair(0, false);
    }

    if (string[current_character] != ',')
    {
        return std::make_pair(0, false);
    }
    current_character += 1;
    if (current_character > string.length())
    {
        return std::make_pair(0, false);
    }

    extract_result = extract_number(string, current_character);
    if (extract_result.second)
    {
        result *= extract_result.first;
    }
    else
    {
        return std::make_pair(0, false);
    }

    if (string[current_character] != ')')
    {
        return std::make_pair(0, false);
    }

    pos = current_character;

    if (do_muls)
    {
        return std::make_pair(result, true);
    }
    else
    {
        return std::make_pair(0, false);
    }
}

std::pair<int32_t, bool> enable_muls(
    const std::string& string,
    uint32_t&          pos)
{
    do_muls = true;
    pos += 3;
    return std::make_pair(0, false);
}

std::pair<int32_t, bool> disable_muls(
    const std::string& string,
    uint32_t&          pos)
{
    do_muls = false;
    pos += 6;
    return std::make_pair(0, false);
}

struct Instruction
{
    std::string label;
    std::pair<int32_t, bool> (*perform)(const std::string&, uint32_t&);
};

const Instruction instructions[] = {
    {"mul", perform_mul},
    {"do()", enable_muls},
    {"don't()", disable_muls}
};

int32_t check_for_instruction(
    const std::string& string,
    uint32_t&          pos)
{
    std::pair<int32_t, bool> result;
    uint32_t current_pos = pos;
    for (auto instruction : instructions)
    {
        if (instruction.label == string.substr(pos, instruction.label.length()))
        {
            current_pos += instruction.label.length();
            result = instruction.perform(string, current_pos);
            if (result.second)
            {
                pos = current_pos;
                return result.first;
            }
        }
    }

    return 0;
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

    std::string input, currentLine;
    while(!fin.eof())
    {
        std::getline(fin, currentLine);
        input += currentLine;
        if (!fin.eof())
        {
            input += '\n';
        }
    }
    fin.close();

    int64_t answer = 0;
    uint32_t next_substr_pos = 0;
    while (next_substr_pos < input.length())
    {
        answer += check_for_instruction(input, next_substr_pos);
        next_substr_pos++;
    }
    
    std::cout << "Answer: " << answer << "\n";
    return 0;
};