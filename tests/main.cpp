#include <iostream>
#include "../include/JsonParser.H"

int main()
{
    JsonParser parser("sample.json");
    // parser.printAllTokens();
    // std::cout << parser["iertg"];
    // parser.printParsedJson();
    Token token(TokenType::STRING, "INR");
    parser["preferred_currency"] = token;

    /*
    JsonObject jsonData(token);
    if (bool res = parser.modifyValue("preferred_currency", jsonData); !res)
        std::cout << "modifyValue(preferred_currency) failed" << std::endl;
    else
        std::cout << "modifyValue(preferred_currency) success" << std::endl;
     */
    return 0;
}