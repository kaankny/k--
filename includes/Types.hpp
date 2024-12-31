#pragma once
#include <unordered_map>
#include <string>

// Type eşlemeleri burada tanımlanır, Parser gibi sınıflarda kullanılabilir
namespace TypeMapping {
    static const std::unordered_map<int, std::string> typeMapping = {
		{TOKEN_TYPE_AUTO, "auto"},
        {TOKEN_TYPE_INT, "int"},
        {TOKEN_TYPE_STRING, "string"},
        {TOKEN_TYPE_BOOL, "bool"},
		{TOKEN_TYPE_FLOAT, "float"},
		{TOKEN_TYPE_CHAR, "char"},
        // Yeni türler buraya eklenir
    };

    inline std::string getTypeName(int tokenType) {
        auto it = typeMapping.find(tokenType);
        if (it != typeMapping.end()) {
            return it->second;
        } else {
            return "unknown";
        }
    }
}
