#pragma once

#include <unordered_map>
#include <string>
#include "Value.hpp"
#include "Logger.hpp"

class Scope
{
public:
    std::unordered_map<std::string, Value> variables;

    void setVariable(const std::string &name, const Value &value)
    {
        variables[name] = value;
    }

    Value getVariable(const std::string &name)
    {
        auto it = variables.find(name);
        if (it != variables.end())
        {
            return it->second;
        }
        else
        {
            Logger::getInstance().log(LogLevel::ERROR, "Undefined variable: " + name);
            exit(1);
        }
    }

	std::string getVariableType(const std::string &name)
	{
		auto it = variables.find(name);
		if (it != variables.end())
		{
			return it->second.valueType;
		}
		else
		{
			Logger::getInstance().log(LogLevel::ERROR, "Undefined variable: " + name);
			exit(1);
		}
	}

	bool isVariableDefined(const std::string &name)
	{
		return variables.find(name) != variables.end();
	}
};
