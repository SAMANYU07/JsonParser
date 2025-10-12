#pragma once
#include <iostream>
#include <string>
#include <utility>

class JsonMetaData
{
    private:
    std::fstream &fileStream;
    std::string fileData;
    std::string fileName;

    public:
    JsonMetaData(std::fstream& fileStream, std::string fileData, std::string fileName): fileStream(fileStream), fileData(std::move(fileData)), fileName(std::move(fileName)) {}


    [[nodiscard]] std::fstream& getFile() const
    {
        return this->fileStream;
    }
    std::string getFileName()
    {
        return this->fileName;
    }
    std::string getFileData()
    {
        return this->fileData;
    }
};