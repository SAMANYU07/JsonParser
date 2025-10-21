#include <iostream>
#include "../include/JsonParser.H"

int main()
{
    JsonParser parser("sample.json");
    // parser.printAllTokens();
    std::cout << parser["iertg"];
    // parser.printParsedJson();
    return 0;
}