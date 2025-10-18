#include <iostream>
#include "../include/JsonParser.H"

int main()
{
    JsonParser parser("sample.json", false);
    parser.printAllTokens();
    std::cout << parser.getValue("isStudent");
    return 0;
}