#ifndef ANALYSIS
#define ANALYSIS

#include "myparser.h"

class Analysis {
private:
    AST* root;
    TypeRegion typeRegion;
    VarFuncDescriptorTable VFDT;
    TypeDescriptorTable TDT;
    std::string code;
    int error;
    std::vector<int> warning;

    bool legalEvaluation(std::string memoryOrigin, std::string exprOrigin, int& warning);
    int getIndexFromTDT(std::string name);
    int getIndexFromVFRegion(std::string name, VarFuncRegion region);

    std::string getTransType(std::string str);
    std::string getTransOp(std::string str);

    VarFuncRegion loadLibFunc(VarFuncRegion r);

    void createAndCheckDT(AST* node);

public:
    Analysis(AST* _r):root(_r), error(0) {}
    void do_analysing(int& _error);

    VarFuncDescriptorTable getVFDT() { return VFDT; }
    TypeDescriptorTable getTDT() { return TDT; }
    std::string getCode() { return code; }
};

#endif // ANALYSIS

