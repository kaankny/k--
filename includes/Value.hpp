#pragma once

#include <iostream>

class Value
{
public:
    int intValue = 0;
    std::string stringValue = "";
	float floatValue = 0.0f;
	bool boolValue = false;
	char charValue = '\0';
    std::string valueType = "undefined"; // "int", "string" veya "undefined"

    // Varsayılan yapıcı fonksiyon
    Value() : intValue(0), stringValue(""), valueType("undefined") {}

    Value(int val) : intValue(val), valueType("int") {}
    Value(const std::string &val) : stringValue(val), valueType("string") {}
	Value(bool val) : boolValue(val), valueType("bool") {}
	Value(float val) : floatValue(val), valueType("float") {}
	Value(char val) : charValue(val), valueType("char") {}

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
		else if (valueType == "char")
		{
			return std::string(1, charValue);
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
		else if (valueType == "char")
			return charValue == other.charValue;

		return false;
	}

	Value operator+(const Value &other) const
	{
		if (this->valueType == "int" && other.valueType == "int")
		{
			return Value(this->intValue + other.intValue);
		}
		else if (this->valueType == "float" && other.valueType == "float")
		{
			return Value(this->floatValue + other.floatValue);
		}
		else if (this->valueType == "int" && other.valueType == "float")
		{
			return Value(this->intValue + other.floatValue);
		}
		else if (this->valueType == "float" && other.valueType == "int")
		{
			return Value(this->floatValue + other.intValue);
		}
		else if (this->valueType == "string" && other.valueType == "string")
		{
			return Value(this->stringValue + other.stringValue);
		}
		else if (this->valueType == "string" && other.valueType == "int")
		{
			return Value(this->stringValue + std::to_string(other.intValue));
		}
		else if (this->valueType == "int" && other.valueType == "string")
		{
			return Value(std::to_string(this->intValue) + other.stringValue);
		}
		else if (this->valueType == "string" && other.valueType == "bool")
		{
			return Value(this->stringValue + (other.boolValue ? "true" : "false"));
		}
		else if (this->valueType == "bool" && other.valueType == "string")
		{
			return Value((this->boolValue ? "true" : "false") + other.stringValue);
		}
		else if (this->valueType == "string" && other.valueType == "float")
		{
			return Value(this->stringValue + std::to_string(other.floatValue));
		}
		else if (this->valueType == "float" && other.valueType == "string")
		{
			return Value(std::to_string(this->floatValue) + other.stringValue);
		}
		else if (this->valueType == "char" && other.valueType == "char")
		{
			return Value(std::string(1, this->charValue) + std::string(1, other.charValue));
		}
		else if (this->valueType == "char" && other.valueType == "string")
		{
			return Value(std::string(1, this->charValue) + other.stringValue);
		}
		else if (this->valueType == "string" && other.valueType == "char")
		{
			return Value(this->stringValue + std::string(1, other.charValue));
		}
		else
		{
			std::cerr << "Type mismatch in '+' operation" << std::endl;
			exit(1);
		}
	}

	Value operator-(const Value &other) const
	{
		if (this->valueType == "int" && other.valueType == "int")
		{
			return Value(this->intValue - other.intValue);
		}
		else if (this->valueType == "float" && other.valueType == "float")
		{
			return Value(this->floatValue - other.floatValue);
		}
		else if (this->valueType == "int" && other.valueType == "float")
		{
			return Value(this->intValue - other.floatValue);
		}
		else if (this->valueType == "float" && other.valueType == "int")
		{
			return Value(this->floatValue - other.intValue);
		}
		else
		{
			std::cerr << "Type mismatch in '-' operation" << std::endl;
			exit(1);
		}
	}

	Value operator*(const Value &other) const
	{
		if (this->valueType == "int" && other.valueType == "int")
		{
			return Value(this->intValue * other.intValue);
		}
		else if (this->valueType == "float" && other.valueType == "float")
		{
			return Value(this->floatValue * other.floatValue);
		}
		else if (this->valueType == "int" && other.valueType == "float")
		{
			return Value(this->intValue * other.floatValue);
		}
		else if (this->valueType == "float" && other.valueType == "int")
		{
			return Value(this->floatValue * other.intValue);
		}
		else if (this->valueType == "string" && other.valueType == "int")
		{
			std::string result = "";
			for (int i = 0; i < other.intValue; i++)
			{
				result += this->stringValue;
			}
			return Value(result);
		}
		else if (this->valueType == "int" && other.valueType == "string")
		{
			std::string result = "";
			for (int i = 0; i < this->intValue; i++)
			{
				result += other.stringValue;
			}
			return Value(result);
		}
		else
		{
			std::cerr << "Type mismatch in '*' operation" << std::endl;
			exit(1);
		}
	}

	Value operator/(const Value &other) const
	{
		if ((other.valueType == "int" && other.intValue == 0) ||
			(other.valueType == "float" && other.floatValue == 0.0))
		{
			std::cerr << "Division by zero" << std::endl;
			exit(1);
		}

		if (this->valueType == "int" && other.valueType == "int")
		{
			return Value(this->intValue / other.intValue);
		}
		else if (this->valueType == "float" && other.valueType == "float")
		{
			return Value(this->floatValue / other.floatValue);
		}
		else if (this->valueType == "int" && other.valueType == "float")
		{
			return Value(this->intValue / other.floatValue);
		}
		else if (this->valueType == "float" && other.valueType == "int")
		{
			return Value(this->floatValue / other.intValue);
		}
		else
		{
			std::cerr << "Type mismatch in '/' operation" << std::endl;
			exit(1);
		}
	}

	Value operator%(const Value &other) const
	{
		if (this->valueType == "int" && other.valueType == "int")
		{
			return Value(this->intValue % other.intValue);
		}
		else if (this->valueType == "float" && other.valueType == "float")
		{
			return Value(static_cast<int>(this->floatValue) % static_cast<int>(other.floatValue));
		}
		else if (this->valueType == "int" && other.valueType == "float")
		{
			return Value(this->intValue % static_cast<int>(other.floatValue));
		}
		else if (this->valueType == "float" && other.valueType == "int")
		{
			return Value(static_cast<int>(this->floatValue) % other.intValue);
		}
		else
		{
			std::cerr << "Type mismatch in '%' operation" << std::endl;
			exit(1);
		}
	}


};
