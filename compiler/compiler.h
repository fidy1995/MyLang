#ifndef COMPILER
#define COMPILER

#include "analysis.h"

const std::string ErrorList[] = {
    "OK", "Bad Symbol", "Symbol Not Match",
    "Bad Identifier", "Bad Number", "Bad Char",
    "Unknown Token", "Lost or Error in Execute Part", "Lost or Error in Main Part",
    "Begin-end Not Match", "Lost or Error in Type Part", "Lost or Error in Var Part",
    "Lost or Error in Function Part", "; Lost", ". Lost",
    "Then-else Not Match", "Expression Error", "Bracket Not Match",
    "Memory Part Error", "Function Call or Class Error", "Array or Class Error",
    "Type Has Same Name", "Type Not Found", "Parent And Children Class Has Same Name",
    "Var Or Func Has Same Name", "Type Not Suitable For Evaluation", "Type Not Suitable For Return",
    "Type Not Suitable For For", "Type Not Suitable For While", "Type Not Suitable For If",
    "Type Not Suitable For Expr", "Var Or Func Not Defined", "Type Not Suitable For Array Index",
    "Secret Error"
};

class Compiler {
private:
    Scanner* myScanner;
    Parser* myParser;
    Analysis* myAnalysis;
    std::vector<Token> tokens;
    AST* root;

    int error_token;
    int error_AST;
    int error_analysis;

public:
    Compiler(std::string str);
    ~Compiler() {
        if (myScanner) delete myScanner;
        if (myParser) delete myParser;
        if (myAnalysis) delete myAnalysis;
        if (root) delete root;
    }

    std::vector<Token> getTokens() { return tokens; }
    AST* getAST() { return root; }
    std::vector<VarFuncDescriptor> getVFDs() {
        std::vector<VarFuncDescriptor> null;
        if (error_analysis == 0) {
            return (myAnalysis->getVFDT()).varFuncDescriptors;
        }
        else {
            return null;
        }
    }

    std::vector<TypeDescriptor> getTDs() {
        std::vector<TypeDescriptor> null;
        if (error_analysis == 0) {
            return (myAnalysis->getTDT()).typeDescriptors;
        }
        else {
            return null;
        }
    }

    std::string getCode() {
        std::string null;
        if (error_analysis == 0) {
            return myAnalysis->getCode();
        }
        else {
            return null;
        }
    }

    int getTokenErrorNum() { return error_token; }
    int getAstErrorNum() { return error_AST; }
    int getAnalysisErrorNum() { return error_analysis; }
    std::string getTokenError() { return ErrorList[error_token]; }
    std::string getAstError() { return ErrorList[error_AST]; }
    std::string getAnalysisError() { return ErrorList[error_analysis]; }
};

#endif // COMPILER

