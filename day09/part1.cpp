#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

uint64_t calculate_sum_for_file(
    uint64_t file_number,
    uint64_t start_pos,
    uint64_t length)
{
    return file_number * ((start_pos * length) + ((length - 1) * (length))/2);
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

    std::string input;
    std::getline(fin, input);
    fin.close();

    uint32_t start_file_index = 0;
    uint32_t end_file_index = input.length() - 1;
    uint64_t insert_pos = 0;
    uint8_t insert_remaining = input[start_file_index] - '0';
    uint8_t extract_remaining = input[end_file_index] - '0';

    uint64_t answer = 0;

    while (start_file_index <= end_file_index)
    {
        if (0 == start_file_index % 2)
        {
            answer += calculate_sum_for_file(start_file_index / 2, insert_pos, insert_remaining);
            insert_pos += insert_remaining;
            start_file_index++;
            insert_remaining = input[start_file_index] - '0';
            continue;
        }

        if (1 == end_file_index % 2)
        {
            end_file_index--;
            extract_remaining = input[end_file_index] - '0';
            continue;
        }

        if (insert_remaining < extract_remaining)
        {
            answer += calculate_sum_for_file(end_file_index / 2, insert_pos, insert_remaining);
            insert_pos += insert_remaining;
            extract_remaining -= insert_remaining;
            start_file_index++;
            insert_remaining = input[start_file_index] - '0';
            continue;
        }
        else if (insert_remaining == extract_remaining)
        {
            answer += calculate_sum_for_file(end_file_index / 2, insert_pos, insert_remaining);
            insert_pos += insert_remaining;
            start_file_index++;
            end_file_index--;
            insert_remaining = input[start_file_index] - '0';
            extract_remaining = input[end_file_index] - '0';
        }
        else
        {
            answer += calculate_sum_for_file(end_file_index / 2, insert_pos, extract_remaining);
            insert_pos += extract_remaining;
            insert_remaining -= extract_remaining;
            end_file_index--;
            extract_remaining = input[end_file_index] - '0';
            continue;
        }
    }

    std::cout << "Answer: " << answer << "\n";
    return 0;
};
