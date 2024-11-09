#pragma once

#include <iostream>

class Value
{
public:
    int intValue = 0;
    std::string stringValue = "";
    std::string valueType = "undefined"; // "int", "string" veya "undefined"

    // Varsayılan yapıcı fonksiyon
    Value() : intValue(0), stringValue(""), valueType("undefined") {}

    Value(int val) : intValue(val), valueType("int") {}
    Value(const std::string &val) : stringValue(val), valueType("string") {}

    // toString fonksiyonu
    std::string toString() const
    {
        if (valueType == "int")
        {
            return std::to_string(intValue);
        }
        else if (valueType == "string")
        {
            return stringValue;
        }
        return "";
    }
};
