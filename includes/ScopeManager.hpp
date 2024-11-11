#pragma once

#include <stack>
#include "Scope.hpp"
#include "Logger.hpp"

class ScopeManager
{
public:
	static ScopeManager& getInstance()
	{
		static ScopeManager instance;
		return instance;
	}

    std::stack<Scope> scopes;

    // Yeni bir kapsam başlatır.
    void beginScope()
    {
        scopes.push(Scope());
    }

    // Kapsamı sonlandırır.
    void endScope()
    {
        if (!scopes.empty())
        {
            scopes.pop();
        }
        else
        {
            Logger::getInstance().log(LogLevel::ERROR, "No scope to end");
            exit(1);
        }
    }

    // Değişkeni mevcut en üst kapsamda ayarlar.
    void setVariable(const std::string &name, const Value &value)
    {
        if (scopes.empty())
        {
            Logger::getInstance().log(LogLevel::ERROR, "No scope to set variable");
            exit(1);
        }
        scopes.top().setVariable(name, value);
    }

    // Değişkeni mevcut kapsam yığınının herhangi bir seviyesinden alır.
    Value getVariable(const std::string &name)
    {
        std::stack<Scope> tempScopes = scopes;
        while (!tempScopes.empty())
        {
            if (tempScopes.top().isVariableDefined(name))
            {
                return tempScopes.top().getVariable(name);
            }
            tempScopes.pop();
        }
        Logger::getInstance().log(LogLevel::ERROR, "Undefined variable: " + name);
        exit(1);
    }

    // Değişkenin türünü mevcut kapsam yığınının herhangi bir seviyesinden alır.
    std::string getVariableType(const std::string &name)
    {
        std::stack<Scope> tempScopes = scopes;
        while (!tempScopes.empty())
        {
            if (tempScopes.top().isVariableDefined(name))
            {
                return tempScopes.top().getVariableType(name);
            }
            tempScopes.pop();
        }
        Logger::getInstance().log(LogLevel::ERROR, "Undefined variable: " + name);
        exit(1);
    }

    // Değişken mevcut bir kapsamda tanımlı mı diye kontrol eder.
    bool isVariableDefined(const std::string &name)
    {
        std::stack<Scope> tempScopes = scopes;
        while (!tempScopes.empty())
        {
            if (tempScopes.top().isVariableDefined(name))
            {
                return true;
            }
            tempScopes.pop();
        }
        return false;
    }

private:
	ScopeManager() {}
	ScopeManager(ScopeManager const&) = delete;
	void operator=(ScopeManager const&) = delete;
};
