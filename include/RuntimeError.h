#pragma once
#include "../Utils/Utils.h"

class RuntimeError
{
    std::string errorMessage;
    public:
    RuntimeError() = default;
    explicit RuntimeError(const std::string &errorMessage)
    {
        throw std::runtime_error(Utils::makeRed(errorMessage));
    }

    void setErrorMessage(const std::string &message)
    {
        this->errorMessage = message;
    }

    void throwError() const
    {
        if (errorMessage.empty())
            return;
        throw std::runtime_error(Utils::makeRed(errorMessage));
    }
};