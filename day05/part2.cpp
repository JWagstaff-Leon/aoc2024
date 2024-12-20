#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

std::ostream& operator<< (std::ostream& sout, std::pair<int32_t, int32_t> rule)
{
    sout << rule.first << "|" << rule.second;
    return sout;
}

std::ostream& operator<< (std::ostream& sout, std::vector<std::pair<int32_t, int32_t>> rules)
{
    for (auto rule : rules)
    {
        sout << rule << "\n";
    }
    return sout;
}

std::ostream& operator<< (std::ostream& sout, std::vector<int32_t> pages)
{
    for (auto page : pages)
    {
        sout << page << " ";
    }

    return sout;
}

std::ostream& operator<< (std::ostream& sout, std::vector<std::vector<int32_t>> updates)
{
    for (auto pages : updates)
    {
        sout << pages << "\n";
    }
    return sout;
}

bool check_order(
    std::vector<int32_t> update,
    std::vector<std::pair<int32_t, int32_t>> rules)
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
    return ordered;
}

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
        if (check_order(update, rules))
        {
            continue;
        }

        while (!check_order(update, rules))
        {
            for (std::size_t rule_index = 0; rule_index < rules.size(); rule_index++)
            {
                auto rule = rules[rule_index];

                std::pair<std::pair<size_t, bool>, std::pair<size_t, bool>> found;
                found.first.second  = false;
                found.second.second = false;

                for (std::size_t page_index = 0; page_index < update.size(); page_index++)
                {
                    auto page = update[page_index];

                    if (page == rule.first)
                    {
                        found.first.first  = page_index;
                        found.first.second = true;
                    }

                    if (page == rule.second)
                    {
                        found.second.first  = page_index;
                        found.second.second = true;
                    }
                }
                if (found.first.second && found.second.second && 
                    found.first.first > found.second.first)
                {
                    int32_t moved_page = update[found.second.first];
                    for (std::size_t page_update_index = found.second.first; page_update_index < found.first.first; page_update_index++)
                    {
                        update[page_update_index] = update[page_update_index + 1];
                    }
                    update[found.first.first] = moved_page;
                }
            }
        }

        answer += update[(update.size() / 2)];
    }

    std::cout << "Answer: " << answer << "\n";
    return 0;
};
