#pragma once
#include <iostream>
#include <map>
#include <string>
#include "JsonToken.h"


class LexicalAnalyzer
{
    private:
    std::string json;
    std::map<std::string, std::string> jsonDataMap;
    size_t pos, length;

    public:
    explicit LexicalAnalyzer(const std::string &fileData): json(fileData), pos(0), length(json.size()) {}

    Token getNextToken()
    {
        skipWhitespace();
        if (pos == length)
            return {TokenType::EOF_TOK, "EOF"};
        switch (json[pos])
        {
            case '{':
                pos++;
                return {TokenType::LEFT_BRACE, "{"};
            case '}':
                pos++;
                return {TokenType::RIGHT_BRACE, "}"};
            case ',':
                pos++;
                return {TokenType::COMMA, ","};
            case '\"':
            {
                pos++;
                std::string tempValue;
                while (json[pos] != '\"')
                    tempValue += json[pos++];
                pos++;
                return {TokenType::STRING, tempValue};
            }
            case ':':
                pos++;
                return {TokenType::COLON, ":"};
            case '\n':
                pos++;
                return {TokenType::EOF_TOK, "\n"};
            default:
            {
                if (isdigit(json[pos]))
                {
                    // pos++;
                    std::string tmpStr;
                    while (isdigit(json[pos]))
                        tmpStr += std::to_string(json[pos++] - '0');
                    return {TokenType::NUMBER, tmpStr};
                }
                else if (isalnum(json[pos]))
                {
                    std::string tempValue;
                    while (json[pos] != '\"' && isalnum(json[pos]))
                    {
                        tempValue += json[pos++];
                    }
                    if (json[pos] == '\"')
                        return {TokenType::STRING, tempValue};
                    else if (tempValue == "true")
                        return {TokenType::TRUE_TOK, tempValue};
                    else if (tempValue == "false")
                        return {TokenType::FALSE_TOK, tempValue};
                }
            }
        }
        return {TokenType::INVALID, "INVALID"};
    }

    void skipWhitespace()
    {
        while (pos < length && isspace(json[pos]))
            pos++;
    }

    void resetLexicalAnalyzer() {
        pos = 0;
        jsonDataMap.clear();
    }

    //for debugging
    void printAllTokens()
    {
        resetLexicalAnalyzer();
        Token tmpTkn = getNextToken();
        TokenType tmpType = tmpTkn.type;
        while (tmpType != TokenType::EOF_TOK && tmpType != TokenType::INVALID)
        {
            std::cout << tmpType << " " << tmpTkn.value << "\n";
            tmpTkn = getNextToken();
            tmpType = tmpTkn.type;
        }
    }

};