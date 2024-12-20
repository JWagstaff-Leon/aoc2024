#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
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

    std::vector<int32_t> list1, list2;
    std::string input;

    while(!fin.eof())
    {
        fin >> input;
        list1.push_back(std::stoi(input));

        fin >> input;
        list2.push_back(std::stoi(input));
    }
    fin.close();

    std::sort(list1.begin(), list1.end());
    std::sort(list2.begin(), list2.end());

    uint64_t answer = 0;
    for (int i = 0; i < list1.size() && i < list2.size(); i++)
    {
        answer += std::abs(list1[i] - list2[i]);
    }
    std::cout << "Answer: " << answer << std::endl;

    return 0;
};