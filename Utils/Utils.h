#pragma once
class Utils
{
    public:
    static bool isSpace(char c)
    {
        if (c == ' ')
            return true;
        return false;
    }
    static std::string makeRed(const std::string& message)
    {
        return "\033[31m" + message + "\033[0m";
    }
};