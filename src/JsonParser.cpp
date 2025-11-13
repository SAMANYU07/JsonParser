//
// Created by samanyu on 02/11/2025.
//

#include "JsonParser.H"

void JsonParser::parseInit()
{
    LexicalAnalyzer lex(fileData);
    // check first {
    if (!lex.getNextToken().typeOf(TokenType::LEFT_BRACE))
        throwParsingError("{ expected in beginning of json\n");
    Token nextToken = lex.getNextToken();
    int line = 2;
    while (nextToken.type != TokenType::EOF_TOK && nextToken.type != TokenType::INVALID)
    {
        if (!nextToken.typeOf(TokenType::INVERTED_COMMA) && !nextToken.typeOf(TokenType::STRING))
            throwParsingError("Unexpected token found before key\n(\") expected before key", line);
        nextToken = lex.getNextToken();
        if (!nextToken.typeOf(TokenType::STRING))
            throwParsingError("String expected after \" in lhs", line);
        std::string lhsVal = nextToken.value;
        nextToken = lex.getNextToken();
        if (!nextToken.typeOf(TokenType::INVERTED_COMMA))
            throwParsingError("closing \" expected after String in lhs", line);
        nextToken = lex.getNextToken();
        if (!nextToken.typeOf(TokenType::COLON))
            throwParsingError(": expected after key", line);
        nextToken = lex.getNextToken();

        JsonObject rhsVal;
        // val is list
        if (nextToken.typeOf(TokenType::LEFT_SQUARE_BRACKET))
        {
            std::vector<JsonObject> elementList;
            do
            {
                nextToken = lex.getNextToken();
                JsonObject element1;
                if (nextToken.typeOf(TokenType::INVERTED_COMMA))
                {
                    nextToken = lex.getNextToken();
                    element1.setJsonObject(nextToken);
                    lex.getNextToken();
                }
                else
                    element1.setJsonObject(nextToken);
                elementList.push_back(element1);
                nextToken = lex.getNextToken();
            } while (nextToken.typeOf(TokenType::COMMA));
            if (!nextToken.typeOf(TokenType::RIGHT_SQUARE_BRACKET))
                throwParsingError("Unterminated list", line);
            rhsVal = JsonObject(elementList);
        }
        // val is string
        else if (nextToken.typeOf(TokenType::INVERTED_COMMA))
        {
            nextToken = lex.getNextToken();
            if (nextToken.typeOf(TokenType::INVALID))
                throwParsingError(nextToken.value, line);
            else if (!nextToken.typeOf(TokenType::STRING))
                throwParsingError("Something went wrong!", line);
            rhsVal = JsonObject(nextToken);
            lex.getNextToken();
        }
        // else rhs is num
        else if (nextToken.typeOf(TokenType::NUMBER) || nextToken.typeOf(TokenType::FALSE_TOK) || nextToken.typeOf(TokenType::TRUE_TOK) || nextToken.typeOf(TokenType::NULL_TOK) || nextToken.typeOf(TokenType::FLOATING_POINT))
        {
            rhsVal = JsonObject(nextToken);
        }
        else if (nextToken.typeOf(TokenType::INVALID) && nextToken.value != "INVALID")
            throwParsingError(nextToken.value, line);
        else
            throwParsingError("Unfamiliar complex value given\n", line);
        nextToken = lex.getNextToken();
        if (nextToken.typeOf(TokenType::COMMA) || nextToken.typeOf(TokenType::RIGHT_BRACE))
        {
            nextToken = lex.getNextToken();
            if (nextToken.typeOf(TokenType::RIGHT_BRACE))
                throwParsingError("Trailing comma before closing brace", line);
        }
        else if (!nextToken.typeOf(TokenType::RIGHT_BRACE))
            throwParsingError("Comma(,) expected after a value", line);
        jsonDataMap.insert(std::pair<std::string, JsonObject>(lhsVal, rhsVal));
        line++;
    }
    if (nextToken.typeOf(TokenType::INVALID))
        throwParsingError("Invalid JSON");
}

bool JsonParser::readJson()
{
    if (!fileStream.is_open())
        return false;
    std::string line;
    while (std::getline(fileStream, line))
    {
        fileData += line;
    }
    return true;
}

bool JsonParser::writeJson()
{
    if (!fileStream.is_open())
        return false;
    fileStream.close();
    fileStream.open(fileLocation, std::ios::in | std::ios::out | std::ios::trunc);
    if (!fileStream.is_open())
        return false;
    fileData.clear();
    fileStream.seekg(0, std::ios::beg);
    fileStream.seekp(0, std::ios::beg);
    fileStream << "{\n";
    for (std::map<std::string, JsonObject>::iterator it = jsonDataMap.begin(); it != jsonDataMap.end(); ++it)
    {
        if (!it->second.asList().empty())
        {
            fileStream << "\t\"" << it->first << "\": [";
            std::vector<JsonObject> elements = it->second.asList();
            for (auto it1 = elements.begin(); it1 != elements.end(); ++it1)
            {
                if (std::holds_alternative<std::string>(it1->getValue()) &&
                    !it1->typeOf(TokenType::FLOATING_POINT))
                    fileStream << "\"" << *it1 << "\"";
                else
                    fileStream << *it1 << " ";
                if (std::next(it1) != elements.end())
                    fileStream << ",";
            }
            fileStream << "]";
        }
        else
        {
            JsonObject obj = it->second;
            JsonValue value = obj.getValue();
            if (std::holds_alternative<std::string>(value) && !obj.typeOf(TokenType::FLOATING_POINT))
                fileStream << "\t\"" << it->first << "\":" << "\"" << it->second << "\"";
            else
                fileStream << "\t\"" << it->first << "\":" << "" << it->second << "";
        }
        if (std::next(it) != jsonDataMap.end())
            fileStream << ",\n";
    }
    fileStream << "\n}";
    return true;
}

void JsonParser::throwParsingError(const std::string &errorMessage)
{
    throwParsingError(errorMessage, -1);
}

void JsonParser::throwParsingError(const std::string &errorMessage, const int lineNo)
{
    RuntimeError("Parsing error: " + errorMessage + (lineNo != -1 ? " at line: " + std::to_string(lineNo) : ""));
}

JsonObject JsonParser::getValue(const std::string &key) const
{
    if (!jsonDataMap.contains(key))
        RuntimeError("Key not found: \"" + key + "\"");
    // return JsonObject(true);
    return jsonDataMap.at(key);
}


bool JsonParser::modifyValue(const std::string &key, const JsonObject &value)
{
    if (!jsonDataMap.contains(key))
        return false;
    jsonDataMap[key] = value;
    return writeJson();
}

JsonObject &JsonParser::operator[](const std::string &key)
{
    if (!jsonDataMap.contains(key))
    {
        JsonObject newVal = JsonObject({TokenType::EMPTY, ""});
        jsonDataMap.insert(std::pair<std::string, JsonObject>(key, newVal));
    }
    // return JsonObject(true);
    return jsonDataMap.at(key);
}


void JsonParser::removeEmptyValues()
{
    for (auto it = jsonDataMap.begin(); it != jsonDataMap.end(); ++it)
    {
        if (it->second.empty())
            jsonDataMap.erase(it);
    }
}

void JsonParser::erase(const std::string &key)
{
    if (!jsonDataMap.contains(key))
        return;
    jsonDataMap.erase(key);
}

void JsonParser::printParsedJson() const
{
    if (jsonDataMap.empty())
        return;
    for (auto [key, value] : jsonDataMap)
        std::cout << key << " == " << value << "\n";
}

void JsonParser::printAllTokens() const
{
    LexicalAnalyzer lex(fileData);
    lex.printAllTokens();
}