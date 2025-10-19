#include <iostream>
#include "../include/JsonParser.H"

int main()
{
    JsonParser parser("sample.json", true);
    parser.printAllTokens();
    std::cout << parser.getValue("isStudent");
    return 0;
}