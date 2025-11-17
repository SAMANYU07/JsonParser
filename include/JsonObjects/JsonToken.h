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
    INVALID,
    EMPTY,
    LEFT_SQUARE_BRACKET,
    RIGHT_SQUARE_BRACKET,
    FLOATING_POINT,
};

struct Token
{
    TokenType type = TokenType::INVALID;
    std::string value;

    Token() = default;

    Token(const TokenType type, std::string value): type(type), value(std::move(value)) {}

    [[nodiscard]] bool typeOf(const TokenType &tokenType) const;

    void setValue(const std::string &newValue);

    void setType(const TokenType &tokenType);
};

std::ostream& operator << (std::ostream&COUT, const TokenType tokenType);