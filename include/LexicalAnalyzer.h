#pragma once
#include <iostream>
#include <fstream>
#include <filesystem>
#include <list>
#include <map>
#include <string>
#include "JsonToken.h"
#include "../Utils/Utils.h"


class LexicalAnalyzer
{
    private:
    std::string json;
    std::map<std::string, std::string> jsonDataMap;
    size_t pos, length;

    public:
    explicit LexicalAnalyzer(std::string fileData): json(fileData), pos(0), length(json.size()) {}

    Token getNextToken()
    {
        skipWhitespace();
        switch (json[pos])
        {
            case '{':
                pos++;
                return Token(TokenType::LEFT_BRACE, "{");
            case '}':
                pos++;
                return Token(TokenType::RIGHT_BRACE, "}");
            case ',':
                pos++;
                return Token(TokenType::COMMA, ",");
            case '\"':
                pos++;
                return Token(TokenType::INVERTED_COMMA, "\"");
            case ':':
                pos++;
                return Token(TokenType::COLON, ":");
            case '\n':
                pos++;
                return Token(TokenType::EOF_TOK, "\n");
            default:
            {
                if (isdigit(json[pos]))
                {
                    // pos++;
                    std::string tmpStr;
                    while (isdigit(json[pos]))
                        tmpStr += std::to_string(json[pos++] - '0');
                    return Token(TokenType::NUMBER, tmpStr);
                }
                else if (isalpha(json[pos]))
                {
                    std::string tempValue;
                    while (json[pos] != '\"' && isalnum(json[pos]))
                        tempValue += json[pos++];
                    return Token(TokenType::STRING, tempValue);
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

    void printAllTokens()
    {
        TokenType tmpType;
        while (tmpType != TokenType::RIGHT_BRACE && tmpType != TokenType::INVALID)
        {
            Token tmpTkn = getNextToken();
            tmpType = tmpTkn.type;
            std::cout << tmpType << " " << tmpTkn.value << "\n";
        }
    }

};