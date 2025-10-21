#include <iostream>
#include "../include/JsonParser.H"

int main()
{
    JsonParser parser("sample.json");
    // parser.printAllTokens();
    // std::cout << parser["id"];
    parser.printParsedJson();
    return 0;
}