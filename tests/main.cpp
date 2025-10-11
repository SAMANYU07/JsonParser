#include <iostream>
#include "../include/JsonParser.H"

int main()
{
    JsonParser parser("sample.json");
    std::cout << parser.getValue("age");
    return 0;
}