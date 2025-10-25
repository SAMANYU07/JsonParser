#include <iostream>
#include "../include/JsonParser.H"

int main()
{
    JsonParser parser("sample.json");
    Token token(TokenType::STRING, "INR");
    Token token2(TokenType::STRING, "USD");

    //modifying an existing value
    // parser["preferred_currency"] = token;
    //
    // //adding a new value
    // parser["secondary_currency"] = token2;
    //
    // //deleting a value
    // parser.erase("secondary_currency");


    /*
    JsonObject jsonData(token);
    if (bool res = parser.modifyValue("preferred_currency", jsonData); !res)
        std::cout << "modifyValue(preferred_currency) failed" << std::endl;
    else
        std::cout << "modifyValue(preferred_currency) success" << std::endl;
     */

    std::cout << parser["scores"][1];

    // parser.printAllTokens();
    return 0;
}