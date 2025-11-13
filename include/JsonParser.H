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

    void parseInit();

    bool readJson();


    bool writeJson();

    ~JsonParser()
    {
        removeEmptyValues();
        writeJson();
        fileStream.close();
    }

    static void throwParsingError(const std::string &errorMessage);

    static void throwParsingError(const std::string &errorMessage, const int lineNo);

    JsonObject getValue(const std::string &key) const;

    bool modifyValue(const std::string &key, const JsonObject &value);

    JsonObject &operator [] (const std::string &key);

    void removeEmptyValues();

    void erase(const std::string &key);

    void printParsedJson() const;
    std::map<std::string, JsonObject>& getJsonDataMap()
    {
        return this->jsonDataMap;
    }

    //for debugging
    void printAllTokens() const;

};