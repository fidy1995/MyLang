#ifndef TOKENCONST
#define TOKENCONST

#include <iostream>
#include <string>

enum TokenType {
    NONENO = -1,
    RESERVEDWORDNO,
    RESERVEDTYPENO,
    IDENTIFIERNO,
    SPECIALSYMBOLNO,
    NUMBERNO,
    COMMENTNO,
    CHARNO,
    STRINGNO
};

const int TYPENAMENUM = 9;

const int RESERVEDWORDNUM = 21;
const int RESERVEDTYPENUM = 6;
const int SPECIALSYMBOLNUM = 19;

const int EXPROPERATORNUM = 6;
const int TERMONEOPERATORNUM = 4;
const int TERMTWOOPERATORNUM = 5;

const int SPECIALCHARACTERNUM = 16;

const std::string TYPENAME[TYPENAMENUM] = {
    "Error", "Reserved Word", "Reserved Type",
    "Identifier", "Special Symbol", "Number",
    "Comment", "Char", "String"
};

const std::string RESERVEDWORD[RESERVEDWORDNUM] = {  // no "const"
    "and", "begin", "div", "do", "downto", "else",
    "end", "for", "foreach", "function", "if", "in",
    "mod", "not", "or", "then", "to","type",
    "var", "while", "xor"
};
const std::string RESERVEDTYPE[RESERVEDTYPENUM] = { "integer", "real", "boolean", "string", "char", "class" };
const std::string SPECIALSYMBOL[SPECIALSYMBOLNUM] = {
    "+", "-", "*", "/", "=", ">", "<", ">=", "<=", "!=",
    "==", "[", "]", ";", ":", ",", ".", "(", ")"
};

const std::string EXPROPERATOR[EXPROPERATORNUM] = { "==", "!=", ">", "<", ">=", "<=" };
const std::string TERMONEOPERATOR[TERMONEOPERATORNUM] = { "+", "-", "or", "xor" };
const std::string TERMTWOOPERATOR[TERMTWOOPERATORNUM] = { "*", "/", "div", "mod", "and" };

const char SPECIALCHARACTER[SPECIALCHARACTERNUM] = {
    '+', '-', '*', '/', '=', '>', '<', '!',
    '[', ']', ';', ':', ',', '.', '(', ')'
};

class Token {
public:
    std::string str;
    TokenType type;

    std::string getValue() { return str; }
    std::string getTypeToString() { return TYPENAME[type + 1]; }
};

// all the function for string
bool isReservedWord(std::string str);
bool isReservedType(std::string str);
bool isSpecialSymbol(std::string str);
bool isExprOperator(std::string str);
bool isTerm1Operator(std::string str);
bool isTerm2Operator(std::string str);

// all the function for char
bool isDigit(char chr);
bool isLetter(char chr);
bool isSpecialCharacter(char chr);

#endif // TOKENCONST

