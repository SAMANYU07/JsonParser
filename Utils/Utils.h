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

    static bool isTrueString(const std::string &str) {
        return str.compare("true") == 0;
    }

    static bool isFalseString(const std::string &str) {
        return str.compare("false") == 0;
    }

    static bool isEndOfString(const std::string &str, const int &pos) {
        if (str[pos] == '\"' && (str[pos + 1] == ',' || str[pos + 1] == '}' || str[pos + 1] == ':' || str[pos + 1] == ']'))
            return true;
        return false;
    }
};