class RuntimeError
{
    std::string errorMessage;
    public:
    RuntimeError() = default;
    explicit RuntimeError(const std::string &errorMessage)
    {
        throw std::runtime_error(Utils::makeRed(errorMessage));
        exit(EXIT_FAILURE);
    }

    void setErrorMessage(const std::string &errorMessage)
    {
        this->errorMessage = errorMessage;
    }

    void throwError()
    {
        if (errorMessage.empty())
            return;
        throw std::runtime_error(Utils::makeRed(errorMessage));
        exit(EXIT_FAILURE);
    }
};