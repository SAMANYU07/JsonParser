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

class JsonParser
{
    private:
    std::fstream fileStream;
    std::string fileData;
    std::map<std::string, std::string> jsonDataMap;
    std::list<Token> tokenList;
    std::string fileName;
    public:

    explicit JsonParser(const std::string &fileLocation): JsonParser(fileLocation, false){};

    explicit JsonParser(const std::string &fileLocation, const bool &monitorParsingTime): JsonParser(fileLocation, monitorParsingTime, false){};

    explicit JsonParser(const std::string &fileLocation, const bool &monitorParsingTime, const bool &restoreCout)
    {
        if (const std::filesystem::path filePath = fileLocation; !std::filesystem::exists(filePath))
            return;
        fileStream.open(fileLocation);
        if (!readJson())
            return;
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
                throwParsingError("\" expected before string in lhs", line);
            std::string lhsVal = nextToken.value;
            nextToken = lex.getNextToken();
            // if (!nextToken.typeOf(TokenType::STRING))
            //     throwParsingError("String expected after \" in lhs", line);
            if (!nextToken.typeOf(TokenType::COLON))
                throwParsingError(": expected after key", line);
            // nextToken = lex.getNextToken();
            // if (!nextToken.typeOf(TokenType::INVERTED_COMMA))
            //     throwParsingError("closing \" expected after String in lhs", line);
            // nextToken = lex.getNextToken();
            nextToken = lex.getNextToken();

            //val is string
            std::string rhsVal;
            // if (nextToken.typeOf(TokenType::STRING))
            // {
            //     nextToken = lex.getNextToken();
            //     if (!nextToken.typeOf(TokenType::STRING) && !nextToken.typeOf(TokenType::NUMBER))
            //         throwParsingError("String expected after \" in rhs", line);
            //     rhsVal = nextToken.value;
            //     nextToken = lex.getNextToken();
            //     if (!nextToken.typeOf(TokenType::INVERTED_COMMA))
            //         throwParsingError("\" is expected after string in rhs", line);
            // }
            //else rhs is num
            if (nextToken.typeOf(TokenType::NUMBER) || nextToken.typeOf(TokenType::STRING)
                || nextToken.typeOf(TokenType::FALSE_TOK) || nextToken.typeOf(TokenType::TRUE_TOK))
            {
                rhsVal = nextToken.value;
            }
            else
                throwParsingError("Unfamiliar complex value given\n", line);
            nextToken = lex.getNextToken();
            if (nextToken.typeOf(TokenType::COMMA) || nextToken.typeOf(TokenType::RIGHT_BRACE))
                nextToken = lex.getNextToken();
            else if (!nextToken.typeOf(TokenType::RIGHT_BRACE))
                throwParsingError("Comma(,) expected after a value", line);
            jsonDataMap.insert(std::pair<std::string, std::string>(lhsVal, rhsVal));
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

    static void throwParsingError(const std::string &errorMessage) {
        throwParsingError(errorMessage, -1);
    }

    static void throwParsingError(const std::string &errorMessage, const int lineNo)
    {
        RuntimeError("Parsing error: " + errorMessage + (lineNo != -1 ? " at line: " + std::to_string(lineNo) : ""));
    }

    std::string getValue(const std::string &key) const
    {
        if (!jsonDataMap.contains(key))
            return "";
        return jsonDataMap.at(key);
    }

    void printParsedJson() const
    {
        if (jsonDataMap.empty())
            return;
        for (const auto &[key, value]: jsonDataMap)
            std::cout << key << " = " << value << "\n";
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