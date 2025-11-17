//
// Created by SAMANYU on 20-10-2025.
//

#ifndef JSONPARSER_JSONOBJECT_H
#define JSONPARSER_JSONOBJECT_H

#include "JsonToken.h"
#include <variant>
#include <sstream>
#include <vector>

using JsonValue = std::variant<int, std::string, bool, std::monostate>;

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
    void setJsonObject(const Token &jsonToken)
    {
        convertTokenAndSetValue(jsonToken);
    }

    //not shifting logic of templates to cpp
    void convertTokenAndSetValue(const Token &jsonToken);

    template <typename T>
    void setValue(const T &jsonValue)
    {
        if constexpr  (std::is_same_v<T, float> || std::is_same_v<T, double>)
        {
            std::stringstream ss;
            ss << jsonValue;
            value = ss.str();
            objectType = TokenType::FLOATING_POINT;
        }
        else
            value = jsonValue;
    }

    [[nodiscard]] JsonValue getValue() const;

    std::string asString() const;
    int asNumber() const;
    bool asBool() const;

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

    bool empty() const;

    std::vector<JsonObject> &asList();

    JsonObject &getListElement(const int &index);

    JsonObject &operator[](const int &index);

    [[nodiscard]] bool typeOf(const TokenType &tokenType) const;


};

 std::ostream &operator << (std::ostream &COUT, JsonObject &jsonObject);

#endif //JSONPARSER_JSONOBJECT_H