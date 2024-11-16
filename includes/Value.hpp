#pragma once

#include <iostream>

class Value
{
public:
    int intValue = 0;
    std::string stringValue = "";
	float floatValue = 0.0f;
	bool boolValue = false;
    std::string valueType = "undefined"; // "int", "string" veya "undefined"

    // Varsayılan yapıcı fonksiyon
    Value() : intValue(0), stringValue(""), valueType("undefined") {}

    Value(int val) : intValue(val), valueType("int") {}
    Value(const std::string &val) : stringValue(val), valueType("string") {}
	Value(bool val) : boolValue(val), valueType("bool") {}
	Value(float val) : floatValue(val), valueType("float") {}

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
		else if (valueType == "bool")
		{
			return boolValue ? "true" : "false";
		}
		else if (valueType == "float")
		{
			return std::to_string(floatValue);
		}
        return "";
    }

	bool operator==(const Value &other) const
	{
		if (valueType != other.valueType)
			return false;

		if (valueType == "int")
			return intValue == other.intValue;
		else if (valueType == "float")
			return floatValue == other.floatValue;
		else if (valueType == "string")
			return stringValue == other.stringValue;
		else if (valueType == "bool")
			return boolValue == other.boolValue;

		return false;
	}

};
