#pragma once
#include <iostream>

enum class TokenType
{
    NUMBER,
    STRING,
    TRUE_TOK,
    FALSE_TOK,
    NULL_TOK,
    COMMA,
    LEFT_BRACE,
    RIGHT_BRACE,
    INVERTED_COMMA,
    EOF_TOK,
    COLON,
    INVALID
};

struct Token
{
    TokenType type;
    std::string value;

    Token() = default;

    Token(TokenType type, std::string value): type(type), value(value) {}

    bool typeOf(const TokenType &type)
    {
        if (this->type == type)
            return true;
        return false;
    }

    void setValue(const std::string &value)
    {
        this->value = value;
    }

    void setType(const TokenType &tokenType)
    {
        this->type = tokenType;
    }
};

inline std::ostream& operator << (std::ostream&COUT, const TokenType tokenType)
{
    switch (tokenType)
    {
        case TokenType::NUMBER:
            COUT << "NUMBER";
            break;
        case TokenType::STRING:
            COUT << "STRING";
            break;
        case TokenType::TRUE_TOK:
            COUT << "TRUE_TOK";
            break;
        case TokenType::FALSE_TOK:
            COUT << "FALSE_TOK";
            break;
        case TokenType::NULL_TOK:
            COUT << "NULL_TOK";
            break;
        case TokenType::COMMA:
            COUT << "COMMA";
            break;
        case TokenType::LEFT_BRACE:
            COUT << "LEFT_BRACE";
            break;
        case TokenType::RIGHT_BRACE:
            COUT << "RIGHT_BRACE";
            break;
        case TokenType::INVERTED_COMMA:
            COUT << "INVERTED_COMMA";
            break;
        case TokenType::EOF_TOK:
            COUT << "EOF_TOK";
            break;
        case TokenType::COLON:
            COUT << "COLON";
            break;
        case TokenType::INVALID:
            COUT << "INVALID";
            break;
    }
    return COUT;
}