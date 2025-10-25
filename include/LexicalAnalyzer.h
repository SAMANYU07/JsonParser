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
            case '[':
                pos++;
                return {TokenType::LEFT_SQUARE_BRACKET, "["};
            case ']':
                pos++;
                return {TokenType::RIGHT_SQUARE_BRACKET, "]"};
            case '\"':
            {
                pos++;
                return {TokenType::INVERTED_COMMA, "\""};
            }
            case ':':
                pos++;
                return {TokenType::COLON, ":"};
            case '\n':
                pos++;
                return {TokenType::EOF_TOK, "\n"};
            default:
            {
                if ((isdigit(json[pos]) || json[pos] == '-') && json[pos - 1] != '\"')
                {
                    // pos++;
                    std::string tmpStr;
                    if (json[pos] == '-')
                        tmpStr += json[pos++];
                    while (isdigit(json[pos]) || json[pos] == '.')
                    {
                        if (json[pos] == '.')
                        {
                            if (tmpStr.find(".") != std::string::npos)
                                return {TokenType::INVALID, "Invalid number format"};
                            tmpStr += json[pos++];
                        }
                        else
                            tmpStr += std::to_string(json[pos++] - '0');
                    }
                    return {TokenType::NUMBER, tmpStr};
                }
                else if (json[pos - 1] == '\"' || isalnum(json[pos]))
                {
                    std::string tempValue;
                    //actually a string after "
                    if (json[pos - 1] == '\"')
                    {
                        while (!Utils::isEndOfString(json, pos))
                        {
                            if (json[pos] == '\"' && json[pos - 1] != '\\' && isalnum(json[pos + 1]))
                                return {TokenType::INVALID, "Unterminated string"};
                            tempValue += json[pos++];
                        }
                        if (json[pos] == '\"')
                            return {TokenType::STRING, tempValue};
                    }
                    else
                    {
                        while (isalnum(json[pos]))
                            tempValue += json[pos++];
                    }
                    if (tempValue == "true")
                        return {TokenType::TRUE_TOK, tempValue};
                    else if (tempValue == "false")
                        return {TokenType::FALSE_TOK, tempValue};
                    else if (tempValue == "null")
                        return {TokenType::NULL_TOK, tempValue};
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
        if (tmpType == TokenType::EOF_TOK)
            std::cout << tmpType << " " << tmpTkn.value << "\n";
        else
            std::cout << "Something is wrong\n";
    }

};