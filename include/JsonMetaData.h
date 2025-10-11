#pragma once
#include <iostream>
#include <string>

class JsonMetaData
{
    private:
    std::fstream &fileStream;
    std::string fileData;
    std::string fileName;

    public:
    JsonMetaData(std::fstream& fileStream, std::string fileData, std::string fileName): fileStream(fileStream), fileData(fileData), fileName(fileName) {}


    std::fstream& getFile()
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