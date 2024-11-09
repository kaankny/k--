# k--
c++ .\sources\*.cpp -I .\includes -DDEBUGMODE

Merhaba! Parser sınıfınızı oluşturmak ve belirttiğiniz söz dizimini (syntax) işlemek için size yardımcı olabilirim. Amacınız, lexer tarafından üretilen tokenları kullanarak bir AST (Abstract Syntax Tree) oluşturmak ve bu AST düğümlerini bir dizide tutmak.

Belirttiğiniz syntax şu şekilde:

```
assign int a 5;
write a;
```

Bu ifadeleri parse ederek AST düğümlerine dönüştüreceğiz ve `m_astNodes` vektöründe saklayacağız.

### 1. AST Düğümlerinin Tanımlanması

Öncelikle, AST düğümlerini temsil edecek bir yapı veya sınıf hiyerarşisi oluşturmanız gerekiyor.

```cpp
// ast.h
#pragma once

#include <string>
#include <vector>

enum class AstNodeType {
    AssignStatement,
    WriteStatement,
    // Gerekirse diğer düğüm tipleri...
};

struct AstNode {
    AstNodeType type;
    virtual ~AstNode() = default;
};

struct AssignNode : public AstNode {
    std::string varType; // "int", "float", vs.
    std::string varName;
    int value; // Şimdilik sadece integer değerler için
};

struct WriteNode : public AstNode {
    std::string varName;
};
```

### 2. Parser Sınıfının Genişletilmesi

Parser sınıfını, lexer'dan gelen tokenları kullanarak AST düğümlerini oluşturacak şekilde genişletelim.

```cpp
// Parser.hpp
#pragma once

#include "ast.h"
#include "../Lexer/Lexer.hpp"
#include "../Lexer/token.h"

#include <vector>

class Parser
{
public:
    static Parser& getInstance()
    {
        static Parser instance;
        return instance;
    }

    void init(const char* inputFilePath);
    void parse();

    std::vector<AstNode *> m_astNodes;

private:
    Parser() {}
    Parser(Parser const&) = delete;
    void operator=(Parser const&) = delete;

    Lexer m_lexer;
    Token m_currentToken;

    void advance();
    void expect(TokenType type);

    AstNode* parseStatement();
    AstNode* parseAssignStatement();
    AstNode* parseWriteStatement();
    // Gerekirse diğer parse fonksiyonları...
};
```

### 3. Parser Fonksiyonlarının Gerçeklenmesi

```cpp
// Parser.cpp
#include "Parser.hpp"
#include <iostream>
#include <cstdlib>

void Parser::init(const char* inputFilePath) {
    m_lexer.init(inputFilePath);
    advance(); // İlk tokenı al
}

void Parser::advance() {
    if (m_lexer.getNextToken()) {
        m_currentToken = m_lexer.getToken();
    } else {
        // Hata yönetimi
        std::cerr << "Lexer'dan token alınamadı!" << std::endl;
        exit(1);
    }
}

void Parser::expect(TokenType type) {
    if (m_currentToken.type != type) {
        // Hata yönetimi
        std::cerr << "Beklenen token türü değil!" << std::endl;
        exit(1);
    }
    advance();
}

void Parser::parse() {
    while (m_currentToken.type != TOKEN_TYPE_EOF) {
        AstNode* stmt = parseStatement();
        if (stmt) {
            m_astNodes.push_back(stmt);
        }
    }
}

AstNode* Parser::parseStatement() {
    if (m_currentToken.type == TOKEN_TYPE_ASSIGN) {
        return parseAssignStatement();
    } else if (m_currentToken.type == TOKEN_TYPE_WRITE) {
        return parseWriteStatement();
    } else {
        // Hata yönetimi
        std::cerr << "Beklenmeyen ifade başlangıcı!" << std::endl;
        exit(1);
    }
    return nullptr;
}

AstNode* Parser::parseAssignStatement() {
    // 'assign' tokenını zaten okumuş durumdayız
    advance();

    // Türü al ('int' veya başka bir şey)
    std::string varType;
    if (m_currentToken.type == TOKEN_TYPE_IDENTIFIER) {
        varType = m_currentToken.identifier;
        advance();
    } else {
        std::cerr << "Beklenen tip ismi!" << std::endl;
        exit(1);
    }

    // Değişken ismi al
    std::string varName;
    if (m_currentToken.type == TOKEN_TYPE_IDENTIFIER) {
        varName = m_currentToken.identifier;
        advance();
    } else {
        std::cerr << "Beklenen değişken ismi!" << std::endl;
        exit(1);
    }

    // Değer al
    int value = 0;
    if (m_currentToken.type == TOKEN_TYPE_INTLIT) {
        value = m_currentToken.intValue;
        advance();
    } else {
        std::cerr << "Beklenen değer!" << std::endl;
        exit(1);
    }

    // ';' bekle
    expect(TOKEN_TYPE_SEMICOLON);

    // AST düğümü oluştur
    AssignNode* node = new AssignNode();
    node->type = AstNodeType::AssignStatement;
    node->varType = varType;
    node->varName = varName;
    node->value = value;

    return node;
}

AstNode* Parser::parseWriteStatement() {
    // 'write' tokenını zaten okumuş durumdayız
    advance();

    // Değişken ismi al
    std::string varName;
    if (m_currentToken.type == TOKEN_TYPE_IDENTIFIER) {
        varName = m_currentToken.identifier;
        advance();
    } else {
        std::cerr << "Beklenen değişken ismi!" << std::endl;
        exit(1);
    }

    // ';' bekle
    expect(TOKEN_TYPE_SEMICOLON);

    // AST düğümü oluştur
    WriteNode* node = new WriteNode();
    node->type = AstNodeType::WriteStatement;
    node->varName = varName;

    return node;
}
```

### 4. Lexer İyileştirmeleri

Parser'ın beklentilerine uygun olarak lexer'ınızı güncellemelisiniz. Özellikle, `assign`, `write`, `int` gibi anahtar kelimeleri ve tanımlayıcıları doğru şekilde ayırt etmek önemlidir.

Lexer'ınızın `getNextToken()` fonksiyonunda tanımlayıcıları ve anahtar kelimeleri işlerken şu değişiklikleri yapabilirsiniz:

```cpp
// Anahtar kelime kontrolü
if (this->m_token.identifier == "assign") {
    this->m_token.type = TOKEN_TYPE_ASSIGN;
} else if (this->m_token.identifier == "write") {
    this->m_token.type = TOKEN_TYPE_WRITE;
} else if (this->m_token.identifier == "int") {
    this->m_token.type = TOKEN_TYPE_INT;
    // Gerekirse diğer anahtar kelimeler...
} else {
    this->m_token.type = TOKEN_TYPE_IDENTIFIER;
}
```

### 5. Token Türlerinin Tanımlanması

Token türlerini `token.h` dosyasında güncelleyin:

```cpp
// token.h
#pragma once

#include <string>

enum TokenType {
    TOKEN_TYPE_EOF,
    TOKEN_TYPE_SEMICOLON,
    TOKEN_TYPE_ASSIGN,
    TOKEN_TYPE_WRITE,
    TOKEN_TYPE_INT,
    TOKEN_TYPE_IDENTIFIER,
    TOKEN_TYPE_INTLIT,
    // Diğer token türleri...
};

struct Token {
    TokenType type;
    int intValue;
    std::string identifier;
    // Gerekirse diğer alanlar...
};
```

### 6. Ana Program (main) İçinde Kullanım

Parser'ınızı kullanarak AST'yi oluşturabilirsiniz.

```cpp
// main.cpp
#include "Parser.hpp"
#include <iostream>

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Kullanım: program <input_file>" << std::endl;
        return 1;
    }

    Parser& parser = Parser::getInstance();
    parser.init(argv[1]);
    parser.parse();

    // AST düğümlerini işleme
    for (auto node : parser.m_astNodes) {
        if (node->type == AstNodeType::AssignStatement) {
            AssignNode* assignNode = static_cast<AssignNode*>(node);
            std::cout << "Assign: " << assignNode->varType << " "
                      << assignNode->varName << " = "
                      << assignNode->value << std::endl;
        } else if (node->type == AstNodeType::WriteStatement) {
            WriteNode* writeNode = static_cast<WriteNode*>(node);
            std::cout << "Write: " << writeNode->varName << std::endl;
        }
        // Bellek temizliği
        delete node;
    }

    return 0;
}
```

### 7. Örnek Giriş Dosyası

`input.txt` dosyasının içeriği:

```
assign int a 5;
write a;
```

Programı derleyip çalıştırdığınızda aşağıdaki çıktıyı almalısınız:

```
Assign: int a = 5
Write: a
```

### 8. Ek Notlar

- **Bellek Yönetimi:** AST düğümlerini dinamik olarak oluşturduğunuz için program sonunda bunları `delete` ile temizlemelisiniz. Alternatif olarak, akıllı işaretçiler (`std::unique_ptr`, `std::shared_ptr`) kullanabilirsiniz.
  
- **Hata Yönetimi:** Hatalar için daha gelişmiş bir yönetim sistemi eklemeyi düşünebilirsiniz. Örneğin, özel exception sınıfları tanımlayabilirsiniz.
  
- **Genişletilebilirlik:** Dilinizi genişletmek isterseniz, yeni token türleri ve AST düğümleri ekleyerek parser'ınızı ve lexer'ınızı güncelleyebilirsiniz.

- **Kaçış Karakterleri ve Diğer Detaylar:** Eğer diliniz string veya karakter sabitlerini destekleyecekse, lexer'ın bu türleri doğru şekilde işlemesini sağlamalısınız. Önceki mesajda belirttiğim lexer iyileştirmelerini uygulamak faydalı olacaktır.

### 9. Sonuç

Bu şekilde, lexer ve parser'ınızı birleştirerek belirttiğiniz syntax'ı parse eden ve AST düğümlerini oluşturan bir sistem kurabilirsiniz. AST düğümlerini bir vektörde tutarak daha sonra bu düğümler üzerinden kodunuzu yorumlayabilir veya derleyebilirsiniz.

Eğer herhangi bir konuda daha fazla yardıma ihtiyacınız olursa veya sorunlarla karşılaşırsanız, lütfen sormaktan çekinmeyin!
