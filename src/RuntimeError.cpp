//
// Created by samanyu on 11/16/25.
//

#include "RuntimeError.h"

void RuntimeError::throwError() const
{
    if (errorMessage.empty())
        return;
    throw std::runtime_error(Utils::makeRed(errorMessage));
}

void RuntimeError::setErrorMessage(const std::string &message)
{
        this->errorMessage = message;
}
