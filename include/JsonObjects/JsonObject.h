//
// Created by SAMANYU on 20-10-2025.
//

#ifndef JSONPARSER_JSONOBJECT_H
#define JSONPARSER_JSONOBJECT_H

#include "JsonToken.h"
#include <variant>
#include <sstream>

using JsonValue = std::variant<double, std::string, bool, std::monostate>;

class JsonObject
{
    JsonValue value;
    public:
    bool isEmpty = true;
    explicit JsonObject()
    {
        isEmpty = true;
    }
    explicit JsonObject(const bool &_isEmpty)
    {
        isEmpty = _isEmpty;
    }
    explicit  JsonObject(const Token &jsonToken)
    {
        convertTokenAndSetValue(jsonToken);
    }

    void convertTokenAndSetValue(const Token &jsonToken)
    {
        switch (jsonToken.type)
        {
            case TokenType::NUMBER:
            {
                std::stringstream ss(jsonToken.value);
                double n;
                ss >> n;
                value = n;
                break;
            }
            case TokenType::STRING:
            case TokenType::EMPTY:
            {
                value = jsonToken.value;
                break;
            }
            case TokenType::NULL_TOK:
            {
                value = std::monostate{};
                break;
            }
            case TokenType::TRUE_TOK:
            case TokenType::FALSE_TOK:
            {
                std::stringstream ss(jsonToken.value);
                bool b;
                ss >> std::boolalpha >> b;
                value = b;
                break;
            }
            default:
                RuntimeError("Something went wrong!");
        }
        isEmpty = false;
    }

    template <typename T>
    void setValue(const Token &jsonToken)
    {
        std::stringstream ss;
        ss << jsonToken.value;
        value = static_cast<T>(value);
    }

    JsonValue getValue() const
    {
        return value;
    }

    std::string asString() const
    {
        return std::get<std::string>(value);
    }
    int asNumber() const
    {
        return std::get<double>(value);
    }
    bool asBool() const
    {
        return std::get<bool>(value);
    }

    JsonObject operator = (const Token &jsonToken)
    {
        convertTokenAndSetValue(jsonToken);
        return *this;
    }

    bool empty() const {
        return isEmpty;
    }


};

std::ostream &operator << (std::ostream &COUT, const JsonObject &jsonObject)
{
    std::visit([&COUT, jsonObject](const auto &jsonValue) {
        if (std::is_same_v<std::decay_t<decltype(jsonValue)>, std::monostate>)
            COUT << "null";
        else if (std::is_same_v<std::decay_t<decltype(jsonValue)>, std::string>)
            COUT << jsonObject.asString();
        else if (std::is_same_v<std::decay_t<decltype(jsonValue)>, double>)
            COUT << jsonObject.asNumber();
        else if (std::is_same_v<std::decay_t<decltype(jsonValue)>, bool>)
            COUT << jsonObject.asBool();
        else
            RuntimeError("Unsupported object type");
    }, jsonObject.getValue());
    return COUT;
}

#endif //JSONPARSER_JSONOBJECT_H