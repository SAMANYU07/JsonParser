#pragma once
#include <iostream>
#include <fstream>
#include <filesystem>
#include <list>
#include <map>
#include <string>
#include "LexicalAnalyzer.h"
#include "RuntimeError.h"
#include "../Utils/ExecutionTimer.h"
#include "JsonObjects/JsonObject.h"

class JsonParser
{
    private:
    std::fstream fileStream;
    std::string fileData;
    std::map<std::string, JsonObject> jsonDataMap;
    std::list<Token> tokenList;
    std::string fileName;
    std::string fileLocation;
    public:

    explicit JsonParser(const std::string &fileLocation): JsonParser(fileLocation, false){};

    explicit JsonParser(const std::string &fileLocation, const bool &monitorParsingTime): JsonParser(fileLocation, monitorParsingTime, false){};

    explicit JsonParser(const std::string &fileLocation, const bool &monitorParsingTime, const bool &restoreCout)
    {
        if (const std::filesystem::path filePath = fileLocation; !std::filesystem::exists(filePath))
            return;
        fileStream.open(fileLocation, std::ios::in | std::ios::out);
        if (!readJson())
            return;
        this->fileLocation = fileLocation;
        if (monitorParsingTime)
            ExecutionTimer timer(restoreCout);
        parseInit();

    }

    void parseInit()
    {
        LexicalAnalyzer lex(fileData);
        //check first {
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

            //val is string
            JsonObject rhsVal;
            if (nextToken.typeOf(TokenType::INVERTED_COMMA))
            {
                nextToken = lex.getNextToken();
                if (nextToken.typeOf(TokenType::INVALID))
                    throwParsingError(nextToken.value, line);
                else if (!nextToken.typeOf(TokenType::STRING))
                    throwParsingError("Something went wrong!", line);
                rhsVal = JsonObject(nextToken);
                lex.getNextToken();
            }
            //else rhs is num
            else if (nextToken.typeOf(TokenType::NUMBER)
                || nextToken.typeOf(TokenType::FALSE_TOK) || nextToken.typeOf(TokenType::TRUE_TOK)
                || nextToken.typeOf(TokenType::NULL_TOK))
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

    bool readJson()
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


    bool writeJson()
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
        fileStream << "{";
        std::map<std::string, JsonObject>::iterator it;
        for (it = jsonDataMap.begin(); it != jsonDataMap.end(); ++it)
        {
            fileStream << "\"" << it->first << "\":" << "\"" << it->second << "\"";
            if (std::next(it) != jsonDataMap.end())
                fileStream << ",\n";
        }
        fileStream << "\n}";
        return true;
    }

    ~JsonParser()
    {
        fileStream.close();
    }

    static void throwParsingError(const std::string &errorMessage) {
        throwParsingError(errorMessage, -1);
    }

    static void throwParsingError(const std::string &errorMessage, const int lineNo)
    {
        RuntimeError("Parsing error: " + errorMessage + (lineNo != -1 ? " at line: " + std::to_string(lineNo) : ""));
    }

    JsonObject getValue(const std::string &key) const
    {
        if (!jsonDataMap.contains(key))
            RuntimeError("Key not found: \"" + key + "\"");
            // return JsonObject(true);
        return jsonDataMap.at(key);
    }

    bool modifyValue(const std::string &key, JsonObject &value)
    {
        if (!jsonDataMap.contains(key))
            return false;
        jsonDataMap[key] = value;
        return writeJson();
    }

    JsonObject operator [] (const std::string &key) const
    {
        if (!jsonDataMap.contains(key))
            RuntimeError("Key not found: \"" + key + "\"");
            // return JsonObject(true);
        return jsonDataMap.at(key);
    }

    void printParsedJson() const
    {
        if (jsonDataMap.empty())
            return;
        for (const auto &[key, value]: jsonDataMap)
            std::cout << key << " = " << value << "\n";
    }
    std::map<std::string, JsonObject>& getJsonDataMap()
    {
        return this->jsonDataMap;
    }

    //for debugging
    void printAllTokens() const
    {
        LexicalAnalyzer lex(fileData);
        lex.printAllTokens();
    }

};