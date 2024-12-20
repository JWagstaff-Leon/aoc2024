#pragma once

#include <memory>
#include <stdint.h>

class IMathOperation
{
    public:
        virtual int64_t calculate() = 0;
};

class Constant : public IMathOperation
{
    public:
        Constant(int64_t value);
        
        int64_t calculate() override;

    private:
        int64_t value_;
};

class Addition : public IMathOperation
{
    public:
        Addition(IMathOperation* lhs, IMathOperation* rhs);

        int64_t calculate() override;

    private:
        std::unique_ptr<IMathOperation> lhs_;
        std::unique_ptr<IMathOperation> rhs_;
};

class Multiplication : public IMathOperation
{
    public:
        Multiplication(IMathOperation* lhs, IMathOperation* rhs);

        int64_t calculate() override;

    private:
        std::unique_ptr<IMathOperation> lhs_;
        std::unique_ptr<IMathOperation> rhs_;
};

class Concatination : public IMathOperation
{
    public:
        Concatination(IMathOperation* lhs, IMathOperation* rhs);

        int64_t calculate() override;

    private:
        std::unique_ptr<IMathOperation> lhs_;
        std::unique_ptr<IMathOperation> rhs_;
};
