#pragma once
#include <iostream>
#include <map>
#include <string>
#include "JsonObjects/JsonToken.h"
#include "../Utils/Utils.h"


class LexicalAnalyzer
{
    private:
    std::string json;
    std::map<std::string, std::string> jsonDataMap;
    size_t pos, length;

    public:
    explicit LexicalAnalyzer(const std::string &fileData): json(fileData), pos(0), length(json.size()) {}

    Token getNextToken();

    void skipWhitespace();

    void resetLexicalAnalyzer();

    //for debugging
    void printAllTokens();

};