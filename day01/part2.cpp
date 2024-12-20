#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

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

    std::map<int32_t, uint32_t> list1, list2;
    std::string input;
    std::map<int32_t, uint32_t>::iterator it;

    int line_num = 0;
    while (!fin.eof())
    {
        int input_num;

        fin >> input;
        fin >> std::ws;
        input_num = std::stoi(input);
        if (list1.end() != (it = list1.find(input_num)))
        {
            (*it).second += 1;
        }
        else
        {
            list1.emplace(input_num, 1);
        }

        fin >> input;
        fin >> std::ws;
        input_num = std::stoi(input);
        if (list2.end() != (it = list2.find(input_num)))
        {
            (*it).second += 1;
        }
        else
        {
            list2.emplace(input_num, 1);
        }
    }
    fin.close();

    uint64_t answer = 0;
    for (auto answer_it : list1)
    {
        if (list2.end() != (it = list2.find(answer_it.first)))
        {
            answer += answer_it.first * answer_it.second * (*it).second;
        }
    }
    std::cout << "Answer: " << answer << std::endl;

    return 0;
};