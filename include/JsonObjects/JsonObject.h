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
    std::vector<JsonObject> listElements;
    // todo: try eliminating objectType from this class
    TokenType objectType;
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
    explicit  JsonObject(const Token &jsonToken): objectType(jsonToken.type)
    {
        convertTokenAndSetValue(jsonToken);
    }
    explicit JsonObject(const std::vector<JsonObject> &jsonObjectList)
    {
        this->listElements = jsonObjectList;
        isEmpty = false;
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
            case TokenType::FLOATING_POINT:
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
    void setValue(const T &jsonValue)
    {
        if constexpr  (std::is_same_v<T, int>)
            value = static_cast<double>(jsonValue);
        else
            value = jsonValue;
    }

    [[nodiscard]] JsonValue getValue() const
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

    template <typename T>
    JsonObject operator = (const T &jsonValue)
    {
        setValue(jsonValue);
        return *this;
    }

    bool empty() const {
        return isEmpty;
    }

    std::vector<JsonObject> &asList()
    {
        return this->listElements;
    }

    JsonObject &getListElement(const int &index)
    {
        return this->listElements.at(index);
    }

    JsonObject &operator[](const int &index)
    {
        return getListElement(index);
    }

    [[nodiscard]] bool typeOf(const TokenType &tokenType) const
    {
        if (this->objectType == tokenType)
            return true;
        return false;
    }


};

inline  std::ostream &operator << (std::ostream &COUT, JsonObject &jsonObject)
{
    if (!jsonObject.asList().empty())
    {
        for (auto &element : jsonObject.asList())
            COUT << element << " ";
    }
    else
    {
        std::visit([&COUT, jsonObject]([[maybe_unused]] const auto &jsonValue) {
            if (std::is_same_v<std::decay_t<decltype(jsonValue)>, std::monostate>)
                COUT << "null";
            else if (std::is_same_v<std::decay_t<decltype(jsonValue)>, std::string>
                || jsonObject.typeOf(TokenType::FLOATING_POINT))
                COUT << jsonObject.asString();
            else if (std::is_same_v<std::decay_t<decltype(jsonValue)>, double>)
                COUT << jsonObject.asNumber();
            else if (std::is_same_v<std::decay_t<decltype(jsonValue)>, bool>)
            {
                if (jsonObject.asBool())
                    COUT << "true";
                else
                    COUT << "false";
            }
            else
                RuntimeError("Unsupported object type");
        }, jsonObject.getValue());
    }
    return COUT;
}

#endif //JSONPARSER_JSONOBJECT_H