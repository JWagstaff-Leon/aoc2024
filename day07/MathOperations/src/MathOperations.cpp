#include "MathOperations.h"

#include <math.h>

Constant::Constant(int64_t value)
{
    value_ = value;
};

int64_t Constant::calculate()
{
    return value_;
};

Addition::Addition(IMathOperation* lhs, IMathOperation* rhs)
{
    lhs_ = std::unique_ptr<IMathOperation>(lhs);
    rhs_ = std::unique_ptr<IMathOperation>(rhs);
};

int64_t Addition::calculate()
{
    return lhs_->calculate() + rhs_->calculate();
};

Multiplication::Multiplication(IMathOperation* lhs, IMathOperation* rhs)
{
    lhs_ = std::unique_ptr<IMathOperation>(lhs);
    rhs_ = std::unique_ptr<IMathOperation>(rhs);
};

int64_t Multiplication::calculate()
{
    return lhs_->calculate() * rhs_->calculate();
};

Concatination::Concatination(IMathOperation* lhs, IMathOperation* rhs)
{
    lhs_ = std::unique_ptr<IMathOperation>(lhs);
    rhs_ = std::unique_ptr<IMathOperation>(rhs);
};


int64_t Concatination::calculate()
{
    int64_t lhs_value = lhs_->calculate();
    int64_t rhs_value = rhs_->calculate();

    int64_t log_value = 1;
    int64_t rhs_throwaway = rhs_value;
    while(rhs_throwaway >= 1)
    {
        log_value *= 10;
        rhs_throwaway /= 10;
    }

    return (lhs_value * log_value) + rhs_value;
};
