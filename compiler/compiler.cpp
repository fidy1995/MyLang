#include "compiler.h"

Compiler::Compiler(std::string str) {
    std::vector<std::string> program;
    std::string tmp = "";

    for (unsigned int i = 0; i < str.size(); i++) {
        if (str[i] == '\n') {
            program.push_back(tmp);
            tmp = "";
        }
        else {
            tmp += str[i];
        }
    }

    if (tmp != "") program.push_back(tmp);

    myScanner = new Scanner(program);
    tokens = myScanner->do_scanning(error_token);

    if (error_token == 0) {
        myParser = new Parser(tokens);
        root = myParser->do_parsing(error_AST);
    }
    else {
        error_AST = error_token;
    }

    if (error_AST == 0) {
        myAnalysis = new Analysis(root);
        myAnalysis->do_analysing(error_analysis);
    }
    else {
        error_analysis = error_AST;
    }
}
