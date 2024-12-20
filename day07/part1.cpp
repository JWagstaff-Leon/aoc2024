#include <cstdint>
#include <fstream>
#include <iostream>
#include <math.h>
#include <memory>
#include <string>
#include <sstream>
#include <vector>

#include "MathOperations.h"

enum Operation
{
    ADD,
    MULTIPLY
};

std::vector<Operation> make_operation_list(
    size_t   size,
    uint32_t index)
{
    std::vector<Operation> operations;
    for(size_t operation_index = 0; operation_index < size; operation_index++)
    {
        if (index >> (size - (operation_index + 1)) & 0x1)
        {
            operations.push_back(MULTIPLY);
        }
        else
        {
            operations.push_back(ADD);
        }
    }
    return operations;
}

bool can_number_match(
    int64_t              goal_number,
    std::vector<int64_t> numbers)
{
    if (numbers.size() < 2)
    {
        return true;
    }

    std::vector<Operation> operations;
    for(int operation_combo = 0; operation_combo < pow(2, numbers.size() - 1); operation_combo++)
    {
        operations = make_operation_list(numbers.size() - 1, operation_combo);
        IMathOperation* top_operation;
        switch(operations[0])
        {
            case ADD:
            {
                top_operation = new Addition(new Constant(numbers[0]),
                                             new Constant(numbers[1]));
                break;
            }
            case MULTIPLY:
            {
                top_operation = new Multiplication(new Constant(numbers[0]),
                                                   new Constant(numbers[1]));
                break;
            }
        }
        for (int operation_index = 1; operation_index < operations.size(); operation_index++)
        {
            switch(operations[operation_index])
            {
                case ADD:
                {
                    top_operation = new Addition(top_operation,
                                                 new Constant(numbers[operation_index + 1]));
                    break;
                }
                case MULTIPLY:
                {
                    top_operation = new Multiplication(top_operation,
                                                       new Constant(numbers[operation_index + 1]));
                    break;
                }
            }
        }

        if (top_operation->calculate() == goal_number)
        {
            return true;
        }
    }
    return false;
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
    int64_t answer = 0;
    while(!fin.eof())
    {
        int64_t goal_number;
        std::vector<int64_t> numbers;
        std::getline(fin, input, ':');
        if(input.length() < 1)
        {
            continue;
        }
        goal_number = std::stoll(input);

        std::getline(fin, input);
        if(input.length() < 1)
        {
            continue;
        }
        std::stringstream number_stream(input);
        while(!number_stream.eof())
        {
            number_stream >> input;
            numbers.push_back(std::stoll(input));
        }

        if(can_number_match(goal_number, numbers))
        {
            answer += goal_number;
        }
    }
    fin.close();

    std::cout << "Answer: " << answer << "\n";
    return 0;
};
