#pragma once
#include <iostream>
#include <fstream>
#include <filesystem>
#include <list>
#include <map>
#include <string>
#include "JsonMetaData.h"
#include "LexicalAnalyzer.h"
#include "RuntimeError.h"

class JsonParser
{
    private:
    std::fstream fileStream;
    std::string fileData;
    std::map<std::string, std::string> jsonDataMap;
    std::list<Token> tokenList;
    std::string fileName;
    public:
    explicit JsonParser(const std::string &fileLocation)
    {
        std::filesystem::path filePath = fileLocation;
        if (!std::filesystem::exists(filePath))
            return;
        fileStream.open(fileLocation);
        if (!readJson())
            return;
        JsonMetaData jsonMetaData(fileStream, fileData, fileName);
        parseInit();
        
    }

    void parseInit()
    {
        LexicalAnalyzer lex(fileData);
        //check first {
        Token firstBrace = lex.getNextToken();
        if (!firstBrace.typeOf(TokenType::LEFT_BRACE))
            throwParsingError("{ expected in beginning of json\n");
        Token nextToken = lex.getNextToken();
        while (nextToken.type != TokenType::RIGHT_BRACE && nextToken.type != TokenType::INVALID)
        {
            if (!nextToken.typeOf(TokenType::INVERTED_COMMA))
                throwParsingError("\" expected before string in lhs");
            nextToken = lex.getNextToken();
            if (!nextToken.typeOf(TokenType::STRING))
                throwParsingError("String expected after \" in lhs");
            std::string lhsVal = nextToken.value;
            nextToken = lex.getNextToken();
            if (!nextToken.typeOf(TokenType::INVERTED_COMMA))
                throwParsingError("closing \" expected after String in lhs");
            nextToken = lex.getNextToken();
            if (!nextToken.typeOf(TokenType::COLON))
                throwParsingError(": expected after key");
            nextToken = lex.getNextToken();

            //val is string
            std::string rhsVal;
            if (nextToken.typeOf(TokenType::INVERTED_COMMA))
            {
                nextToken = lex.getNextToken();
                if (!nextToken.typeOf(TokenType::STRING))
                throwParsingError("String expected after \" in rhs");
                rhsVal = nextToken.value;
                nextToken = lex.getNextToken();
                if (!nextToken.typeOf(TokenType::INVERTED_COMMA))
                    throwParsingError("\" is expected after string in rhs");
            }
            //else rhs is num
            else if (nextToken.typeOf(TokenType::NUMBER))
            {
                rhsVal = nextToken.value;
            }
            else
            throwParsingError("Unfamilier complex value given\n");
            nextToken = lex.getNextToken();
            if (nextToken.typeOf(TokenType::COMMA))
                nextToken = lex.getNextToken();
            else if (!nextToken.typeOf(TokenType::RIGHT_BRACE))
                throwParsingError("Comma(,) expected after a value");
            jsonDataMap.insert(std::pair<std::string, std::string>(lhsVal, rhsVal));
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

    void throwParsingError(const std::string &errorMessage) const
    {
        RuntimeError("Parsing error: " + errorMessage);
    }

    std::string getValue(const std::string &key) const
    {
        if (jsonDataMap.find(key) == jsonDataMap.end())
            return "";
        return jsonDataMap.at(key);
    }

    void printParsedJson() const
    {
        if (jsonDataMap.empty())
            return;
        for (auto it = jsonDataMap.begin(); it != jsonDataMap.end(); ++it)
            std::cout << it->first << " = " << it->second << "\n";
    }
    std::map<std::string, std::string>& getJsonDataMap()
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