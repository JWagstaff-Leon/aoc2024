#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

int main(int argc, char *argv[])
{
    int count = 0;
    int goal = 5;
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

    std::vector<std::pair<int32_t, int32_t>> rules;
    std::vector<std::vector<int32_t>>        updates;

    std::string                 input_string;
    std::stringstream           input_stream;
    char                        throwaway;

    std::pair<int32_t, int32_t> new_rule;
    int32_t                     new_page;

    do
    {
        std::getline(fin, input_string);
        if (input_string.length() > 1)
        {
            input_stream << input_string << "\n";
        }
    } while (input_string.length() > 1);

    while (!input_stream.eof())
    {
        input_stream >> new_rule.first >> throwaway >> new_rule.second >> std::ws;
        rules.push_back(new_rule);
    }

    while(!fin.eof())
    {
        std::vector<int32_t> pages;

        std::getline(fin, input_string);
        if (input_string.length() <= 1)
        {
            continue;
        }

        input_stream.clear();
        input_stream << input_string << "\n";

        while (!input_stream.eof())
        {
            input_stream >> new_page >> throwaway;
            pages.push_back(new_page);
        }
        updates.push_back(std::move(pages));
    }
    fin.close();

    uint64_t answer = 0;
    for (auto update : updates)
    {
        bool ordered = true;
        for (auto rule : rules)
        {
            bool valid = true;
            std::pair<bool, bool> found;
            found.first  = false;
            found.second = false;
            for (auto page : update)
            {
                if (page == rule.first)
                {
                    found.first = true;
                }

                if (page == rule.second)
                {
                    found.second = true;
                }

                if (valid && found.second && !found.first)
                {
                    valid = false;
                }
            }
            if (found.first && found.second && !valid)
            {
                ordered = false;
                break;
            }
        }
        if (ordered)
        {
            answer += update[(update.size() / 2)];
        }
    }

    std::cout << "Answer: " << answer << "\n";
    return 0;
};
