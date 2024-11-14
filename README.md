## assign
```
assign int age = 19;
assign string name = "Kaan";
assign string surname = "Kanyilmaz";
assign string fullName = name + " " + surname;	
assign bool isStudent = true;
```

## write, writeln
```
writeln("Age: " + age);
writeln("Name: " + name);
writeln("Surname: " + surname);
writeln("Full Name: " + fullName);
writeln("Is Student: " + isStudent);
```

## read
```
// SOON
read("Enter your age: ", age);
read("Enter your name: ", name);
read("Enter your surname: ", surname);
read("Are you a student? (true/false): ", isStudent);
```

## if-elseif-else
```
if (age > 18) {
	writeln("You are an adult.");
} elseif (age == 18) {
	writeln("You are 18 years old.");
} else {
	writeln("You are a child.");
} endif;
```

## while
```
assign int i = 0;
while (i < 5) {
	writeln("i: " + i);
	i = i + 1;
} endwhile;
```

## for
```
for (assign int i = 0; i < 5; i = i + 1) {
	writeln("i: " + i);
} endfor;

assign int i = 0;
for (; i < 5; i = i + 1) {
	writeln("i: " + i);
} endfor;

for (;;)
{
	writeln(num);
	num = num + 1;
	if (num == 10)
	{
		break;
	} endif;
} endfor;
```

## funcation definition
```
function isPrime(num: int) -> bool {
	assign bool isPrime = false;
	assign int divisor = 2;
	assign bool isLooping = true;
	assign int remainder = 0;

	if (num <= 1)
	{
		isPrime = false;
		writeln("Gecersiz sayi girdiniz.");
	}
	else
	{
		for (assign int i = 2; (i < num) && (isLooping); i = i + 1)
		{
			remainder = num - (num / i) * i;

			if (remainder == 0)
			{
				isLooping = false;
				isPrime = false;
			}
			else
			{
				isPrime = true;
			} endif;
		} endfor;

		if (isPrime)
		{
			writeln("Girdiginiz sayi asaldir.");
		}
		else
		{
			writeln("Girdiginiz sayi asal degildir.");
		} endif;
	} endif;
	return isPrime;
} endfunction;

```

## function call
```
assign int num = 7;
assign bool isPrime = call isPrime(num);
writeln("isPrime: " + call isPrime(num));	
```

Bu tür dönüşümleri desteklemek için birkaç değişiklik yapabiliriz:

1. **Lexer** ve **Parser**: Parantez içindeki tür dönüşümünü ve ilgili ifadeyi algılayabilmesi için genişletmeliyiz.
2. **AST Yapısı**: Tür dönüşümünü temsil eden yeni bir düğüm (örneğin `AST_NODE_TYPE_CAST`) ekleyebiliriz.
3. **Interpreter**: Dönüşüm yapılmış değerlere yönelik işlemler sırasında uygun tür dönüşümünü uygulayabilmesi için yeni bir işleyici ekleyebiliriz.

Aşağıda, gerekli değişiklikleri nasıl yapabileceğimize dair genel bir plan ve kod örnekleri bulunmaktadır.

### 1. Lexer Güncellemesi
Öncelikle, `(float)` veya `(int)` gibi tür dönüşüm ifadelerini algılaması için Lexer’a tür dönüşüm anahtar kelimelerini ekleyelim:

```cpp
case '(': {
    // Parantez içinde bir tür dönüşümü varsa işleme al
    if (this->m_currentIndex + 1 < this->m_inputFileContent.size() &&
        (this->m_inputFileContent.substr(this->m_currentIndex + 1, 5) == "float" ||
         this->m_inputFileContent.substr(this->m_currentIndex + 1, 3) == "int")) 
    {
        if (this->m_inputFileContent.substr(this->m_currentIndex + 1, 5) == "float") {
            this->m_token.type = TOKEN_TYPE_CAST_FLOAT;
            this->m_currentIndex += 6;  // "(float" 6 karakter uzunluğunda
        } else if (this->m_inputFileContent.substr(this->m_currentIndex + 1, 3) == "int") {
            this->m_token.type = TOKEN_TYPE_CAST_INT;
            this->m_currentIndex += 4;  // "(int" 4 karakter uzunluğunda
        }
    } else {
        this->m_token.type = TOKEN_TYPE_LPAREN;
    }
    break;
}
```

### 2. Parser’da Tür Dönüşümü Ayrıştırma
Parser’da, tür dönüşüm ifadelerini tanıyıp AST içinde uygun bir düğüm türü ile temsil edelim:

```cpp
t_ast_node *Parser::parseFactor() {
    if (m_currentToken->type == TOKEN_TYPE_CAST_FLOAT || m_currentToken->type == TOKEN_TYPE_CAST_INT) {
        std::string castType = (m_currentToken->type == TOKEN_TYPE_CAST_FLOAT) ? "float" : "int";
        advanceToken(); // Parantez içindeki tür dönüşümünü geç

        if (m_currentToken->type != TOKEN_TYPE_RPAREN) {
            Logger::getInstance().log(LogLevel::ERROR, "Expected ')' after cast type");
            exit(1);
        }
        advanceToken();

        t_ast_node *exprNode = parseFactor();

        t_ast_node_cast *castNode = new t_ast_node_cast;
        castNode->type = t_ast_node_type::AST_NODE_TYPE_CAST;
        castNode->castType = castType;
        castNode->expr = exprNode;

        return castNode;
    }
    // Diğer parseFactor kodları...
}
```

### 3. AST Yapısına Tür Dönüşümü Düğümü Ekleme
AST yapısında tür dönüşümü için yeni bir düğüm yapısı ekleyelim. Örneğin, `t_ast_node_cast` adında bir yapı oluşturabiliriz:

```cpp
struct t_ast_node_cast : t_ast_node {
    std::string castType;
    t_ast_node *expr;
};
```

### 4. Interpreter’da Tür Dönüşümünü Yorumlama
Son olarak, `evaluateExpression` fonksiyonunda tür dönüşümü düğümünü yorumlayacak bir işlem ekleyelim:

```cpp
Value Interpreter::evaluateExpression(t_ast_node *node) {
    switch (node->type) {
        case t_ast_node_type::AST_NODE_TYPE_CAST: {
            t_ast_node_cast *castNode = static_cast<t_ast_node_cast *>(node);
            Value value = evaluateExpression(castNode->expr);
            
            if (castNode->castType == "float" && value.valueType == "int") {
                return Value(static_cast<float>(value.intValue));
            }
            else if (castNode->castType == "int" && value.valueType == "float") {
                return Value(static_cast<int>(value.floatValue));
            }
            Logger::getInstance().log(LogLevel::ERROR, "Invalid cast operation");
            exit(1);
        }
        // Diğer evaluateExpression durumları...
    }
}
```

### Özet
Bu adımlarla tür dönüşümü desteklenmiş olur. Artık, `assign float res = (float)a + b;` ifadesi düzgün bir şekilde yorumlanacaktır. Tür dönüşümlerini desteklemek için `evaluateExpression` işlevinde yeni veri türleri eklenebilir.