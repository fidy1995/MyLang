#include "tokenconst.h"

// all the function for string
bool isReservedWord(std::string str) {
    for (int i = 0; i < RESERVEDWORDNUM; i++) {
        if (str == RESERVEDWORD[i]) return true;
    }
    return false;
}

bool isReservedType(std::string str) {
    for (int i = 0; i < RESERVEDTYPENUM; i++) {
        if (str == RESERVEDTYPE[i]) return true;
    }
    return false;
}

bool isSpecialSymbol(std::string str) {
    for (int i = 0; i < SPECIALSYMBOLNUM; i++) {
        if (str == SPECIALSYMBOL[i]) return true;
    }
    return false;
}

bool isExprOperator(std::string str) {
    for (int i = 0; i < EXPROPERATORNUM; i++) {
        if (str == EXPROPERATOR[i]) return true;
    }
    return false;
}

bool isTerm1Operator(std::string str) {
    for (int i = 0; i < TERMONEOPERATORNUM; i++) {
        if (str == TERMONEOPERATOR[i]) return true;
    }
    return false;
}

bool isTerm2Operator(std::string str) {
    for (int i = 0; i < TERMTWOOPERATORNUM; i++) {
        if (str == TERMTWOOPERATOR[i]) return true;
    }
    return false;
}

// all the function for char
bool isDigit(char chr) {
    return ((chr >= '0') && (chr <= '9'));
}

bool isLetter(char chr) {
    return (((chr >= 'a') && (chr <= 'z')) || ((chr >= 'A') && (chr <= 'Z')));
}

bool isSpecialCharacter(char chr) {
    for (int i = 0; i < SPECIALCHARACTERNUM; i++) {
        if (chr == SPECIALCHARACTER[i]) return true;
    }
    return false;
}
