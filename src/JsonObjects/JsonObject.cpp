//
// Created by samanyu on 11/17/25.
//

#include "JsonObjects/JsonObject.h"
#include "RuntimeError.h"

void JsonObject::convertTokenAndSetValue(const Token &jsonToken)
{
    switch (jsonToken.type)
    {
        case TokenType::NUMBER:
        {
            std::stringstream ss(jsonToken.value);
            int n;
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

[[nodiscard]] JsonValue JsonObject::getValue() const
{
    return value;
}

std::string JsonObject::asString() const
{
    return std::get<std::string>(value);
}

int JsonObject::asNumber() const
{
    return std::get<int>(value);
}

bool JsonObject::asBool() const
{
    return std::get<bool>(value);
}

// JsonObject JsonObject::operator=(const Token &jsonToken)
// {
//     convertTokenAndSetValue(jsonToken);
//     return *this;
// }

// template<typename T>
// JsonObject JsonObject::operator=(const T &jsonValue)
// {
//     setValue(jsonValue);
//     return *this;
// }

bool JsonObject::empty() const
{
    return isEmpty;
}

std::vector<JsonObject> &JsonObject::asList()
{
    return this->listElements;
}

JsonObject &JsonObject::getListElement(const int &index)
{
    return this->listElements.at(index);
}

JsonObject &JsonObject::operator[](const int &index)
{
    return getListElement(index);
}

bool JsonObject::typeOf(const TokenType &tokenType) const
{
    if (this->objectType == tokenType)
        return true;
    return false;
}

std::ostream &operator << (std::ostream &COUT, JsonObject &jsonObject)
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
            else if (std::is_same_v<std::decay_t<decltype(jsonValue)>, int>)
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