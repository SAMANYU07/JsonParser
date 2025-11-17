//
// Created by samanyu on 11/17/25.
//

#include "JsonObjects/JsonToken.h"

bool Token::typeOf(const TokenType &tokenType) const
{
    if (this->type == tokenType)
        return true;
    return false;
}

void Token::setValue(const std::string &newValue)
{
    this->value = newValue;
}


void Token::setType(const TokenType &tokenType)
{
    this->type = tokenType;
}

std::ostream &operator<<(std::ostream &COUT, const TokenType tokenType)
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
