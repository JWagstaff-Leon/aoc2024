#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct FileEntry
{
    uint32_t file_id;
    uint8_t  size;
    uint8_t  free_space_after;
};

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

    std::vector<FileEntry> files;

    uint64_t insert_pos = 0;

    for (uint32_t file_index = 0; file_index < input.length(); file_index += 2)
    {
        FileEntry new_entry;
        new_entry.file_id = file_index / 2;
        new_entry.size = input[file_index] - '0';
        new_entry.free_space_after = 0;
        if (file_index + 1 != input.length())
        {
            new_entry.free_space_after = input[file_index + 1] - '0';
        }
        files.push_back(new_entry);
    }

    for (uint32_t current_file_id = input.length() / 2; current_file_id > 0; current_file_id--)
    {
        for (uint32_t end_file_index = files.size() - 1; end_file_index > 0; end_file_index--)
        {
            if (files[end_file_index].file_id != current_file_id)
            {
                continue;
            }
            for (uint32_t start_file_index = 0; start_file_index < end_file_index; start_file_index++)
            {
                if (files[start_file_index].free_space_after >= files[end_file_index].size)
                {
                    FileEntry moved_entry = files[end_file_index];
                    files[end_file_index - 1].free_space_after += files[end_file_index].size + files[end_file_index].free_space_after;
                    for (uint32_t shift_index = end_file_index; shift_index > start_file_index + 1; shift_index--)
                    {
                        files[shift_index] = files[shift_index - 1];
                    }
                    moved_entry.free_space_after = files[start_file_index].free_space_after - moved_entry.size;
                    files[start_file_index + 1] = moved_entry;
                    files[start_file_index].free_space_after = 0;
                    end_file_index++;
                    break;
                }
            }
        }
    }

    uint64_t answer = 0;
    for(auto file : files)
    {
        answer += calculate_sum_for_file(file.file_id, insert_pos, file.size);
        insert_pos += file.size + file.free_space_after;
    }

    std::cout << "Answer: " << answer << "\n";
    return 0;
};
