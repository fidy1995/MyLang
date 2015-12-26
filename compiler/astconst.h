#ifndef ASTCONST
#define ASTCONST

#include "regionconst.h"

const std::string ASTNAME[40] = {
    "Program", "TypeDefineBlock", "VarDefineBlock",
    "FunctionDefineBlock", "ProgramExecuteBlock", "TypeDefinePart",
    "TypeDefineIns", "ClassBlock", "ClassVarPart",
    "ClassFunctionPart", "VarDefineInsPart", "VarDefineIns",
    "FunctionDefinePart", "FunctionName", "FunctionVarDefineBlock",
    "FunctionExecuteBlock", "ArgDefinePart", "MainExecutePart",
    "ExecuteInsBlock", "ExecuteInsPart", "ExecuteIns",
    "EvaluationIns", "LoopIns", "ConditionIns",
    "ReturnIns", "ForIns", "ForeachIns",
    "WhileIns", "IfIns", "Expr",
    "Term1", "Term2", "Term3",
    "ValueStruct", "MemoryStruct", "Array",
    "IndexPart", "FunctionCall", "ArgPart"
    "Token"
};

class AST {
private:
    int type;
    VarFuncRegion r1; // for global region
    VarFuncRegion r2; // for local region

public:
    AST(int _t): type(_t) {
        r1.envName = "";
        r2.envName = "";
    }
    virtual ~AST() {}

    virtual void print() = 0;
    virtual std::vector<AST*> get() = 0;

    void setR1(VarFuncRegion r) { r1 = r; }
    void setR2(VarFuncRegion r) { r2 = r; }
    void setR1Env(std::string env) { r1.envName = env; }
    void setR2Env(std::string env) { r2.envName = env; }
    void addR1(VarFuncRegion r) {
        for (unsigned int i = 0; i < r.varFunc_defs.size(); i++) {
            r1.varFunc_defs.push_back(r.varFunc_defs[i]);
        }
    }
    void addR2(VarFuncRegion r) {
        for (unsigned int i = 0; i < r.varFunc_defs.size(); i++) {
            r2.varFunc_defs.push_back(r.varFunc_defs[i]);
        }
    }
    void addR1(VarFuncdef def) { r1.varFunc_defs.push_back(def); }
    void addR2(VarFuncdef def) { r2.varFunc_defs.push_back(def); }
    VarFuncRegion getR1() { return r1; }
    VarFuncRegion getR2() { return r2; }

    int getTypeNum() { return type; }
    std::string getValue();
    std::vector<AST*> getChildren();
};

// Whole Program
class ProgramAST;

// Program Architecture
class TypeDefineBlockAST;
class VarDefineBlockAST;
class FunctionDefineBlockAST;
class ProgramExecuteBlockAST;

// Type Define Ins and Type Define Block Part
class TypeDefinePartAST;
class TypeDefineInsAST;
class ClassBlockAST;
class ClassVarPartAST;
class ClassFunctionPartAST;

// Var Define Ins Part (no need for Block)
class VarDefineInsPartAST;
class VarDefineInsAST;

// Function Define Block Part (no need for Ins)
class FunctionDefinePartAST;
class FunctionNameAST;
class FunctionVarDefineBlockAST;
class FunctionExecuteBlockAST;
class ArgDefinePartAST;

// Execute Ins and Execute Ins Block Part
class MainExecutePartAST;
class ExecuteInsBlockAST;
class ExecuteInsPartAST;
class ExecuteInsAST;
class EvaluationInsAST;
class LoopInsAST;
class ConditionInsAST;
class ReturnInsAST;
class ForInsAST;
class ForeachInsAST;
class WhileInsAST;
class IfInsAST;

// Expr Part
class ExprAST;
class Term1AST;
class Term2AST;
class Term3AST;

// ValueStruct Part
class ValueStructAST;
class MemoryStructAST;
class ArrayAST;
class IndexPartAST;
class FunctionCallAST;
class ArgPartAST;

// token
class TokenAST;

class TokenAST: public AST {
private:
    Token op;

    std::string typeName;
    std::vector<Vardef> funcArg;
    bool func;

public:
    TokenAST(Token _op): AST(39), op(_op), func(false) {}
    Token getOp() { return op; }

    void print() {}
    std::vector<AST*> get() {
        std::vector<AST*> null;
        return null;
    }

    void setTypeName(std::string _typeName) { typeName = _typeName; }
    std::string getTypeName() { return typeName; }
    void setFuncArg(std::vector<Vardef> _funcArg) { funcArg = _funcArg; }
    std::vector<Vardef> getFuncArg() { return funcArg; }
    void setFunc(bool _func) { func = _func; }
    bool isFunc() { return func; }
};

class ProgramAST: public AST {
private:
    TypeDefineBlockAST* type;
    VarDefineBlockAST* var;
    FunctionDefineBlockAST* func;
    ProgramExecuteBlockAST* main;
public:
    ProgramAST(TypeDefineBlockAST* _t, VarDefineBlockAST* _v, FunctionDefineBlockAST* _f, ProgramExecuteBlockAST* _m):
      AST(0), type(_t), var(_v), func(_f), main(_m) {}

    void print();
    std::vector<AST*> get();
};

// this class can be NULL
class TypeDefineBlockAST: public AST {
private:
    TypeDefinePartAST* node;
    int type; // 0 for NULL, 1 for type NULL, 2 for type ins_part
public:
    TypeDefineBlockAST(int _t): AST(1), node(NULL), type(_t) {}
    TypeDefineBlockAST(TypeDefinePartAST* _n): AST(1), node(_n), type(2) {}

    void print();
    std::vector<AST*> get();
};

// this class can be NULL
class VarDefineBlockAST: public AST {
private:
    VarDefineInsPartAST* node;
    int type; // 0 for NULL, 1 for var NULL, 2 for var ins_part
public:
    VarDefineBlockAST(int _t): AST(2), node(NULL), type(_t) {}
    VarDefineBlockAST(VarDefineInsPartAST* _n): AST(2), node(_n), type(2) {}

    void print();
    std::vector<AST*> get();
};

// this class can be NULL
class FunctionDefineBlockAST: public AST {
private:
    std::vector<FunctionDefinePartAST*> nodes;
public:
    FunctionDefineBlockAST(std::vector<FunctionDefinePartAST*> _n): AST(3), nodes(_n) {}

    void print();
    std::vector<AST*> get();
};

class ProgramExecuteBlockAST: public AST {
private:
    ExecuteInsBlockAST* node;
public:
    ProgramExecuteBlockAST(ExecuteInsBlockAST* _n): AST(4), node(_n) {}

    void print();
    std::vector<AST*> get();
};

class TypeDefinePartAST: public AST {
private:
    std::vector<TypeDefineInsAST*> nodes;
public:
    TypeDefinePartAST(std::vector<TypeDefineInsAST*> _n): AST(5), nodes(_n) {}

    void print();
    std::vector<AST*> get();
};

class TypeDefineInsAST: public AST {
private:
    ClassBlockAST* node;
    TokenAST* id;
    TokenAST* type;
public:
    TypeDefineInsAST(ClassBlockAST* _n, Token _id, Token _t): AST(6), node(_n) {
        id = new TokenAST(_id);
        type = new TokenAST(_t);
    }
    TypeDefineInsAST(Token _id, Token _t): AST(6), node(NULL) {
        id = new TokenAST(_id);
        type = new TokenAST(_t);
    }

    void print();
    std::vector<AST*> get();
};

class ClassBlockAST: public AST {
private:
    ClassVarPartAST* left;
    ClassFunctionPartAST* right;
public:
    ClassBlockAST(ClassVarPartAST* _l, ClassFunctionPartAST* _r): AST(7), left(_l), right(_r) {}

    void print();
    std::vector<AST*> get();
};

// can be NULL
class ClassVarPartAST: public AST {
private:
    VarDefineInsPartAST* node;
public:
    ClassVarPartAST(): AST(8), node(NULL) {}
    ClassVarPartAST(VarDefineInsPartAST* _n): AST(8), node(_n) {}

    void print();
    std::vector<AST*> get();
};

// can be NULL
class ClassFunctionPartAST: public AST {
private:
    FunctionDefineBlockAST* node;
public:
    ClassFunctionPartAST(FunctionDefineBlockAST* _n): AST(9), node(_n) {}

    void print();
    std::vector<AST*> get();
};

class VarDefineInsPartAST: public AST {
private:
    std::vector<VarDefineInsAST*> nodes;
public:
    VarDefineInsPartAST(std::vector<VarDefineInsAST*> _n): AST(10), nodes(_n) {}

    void print();
    std::vector<AST*> get();
};

class VarDefineInsAST: public AST {
private:
    ArrayAST* node;
    TokenAST* type;
public:
    VarDefineInsAST(ArrayAST* _n, Token _t): AST(11), node(_n) {
        type = new TokenAST(_t);
    }

    void print();
    std::vector<AST*> get();
};

class FunctionDefinePartAST: public AST {
private:
    FunctionNameAST* name;
    FunctionVarDefineBlockAST* var;
    FunctionExecuteBlockAST* main;
public:
    FunctionDefinePartAST(FunctionNameAST* _n, FunctionVarDefineBlockAST* _v, FunctionExecuteBlockAST* _m): AST(12), name(_n), var(_v), main(_m) {}

    void print();
    std::vector<AST*> get();
};

class FunctionNameAST: public AST {
private:
    ArgDefinePartAST* node;
    TokenAST* name;
    TokenAST* type;
public:
    FunctionNameAST(ArgDefinePartAST* _n, Token _name, Token _t): AST(13), node(_n) {
        name = new TokenAST(_name);
        type = new TokenAST(_t);
    }
    FunctionNameAST(ArgDefinePartAST* _n, Token _name): AST(13), node(_n), type(NULL) {
        name = new TokenAST(_name);
    }

    void print();
    std::vector<AST*> get();
};

class FunctionVarDefineBlockAST: public AST {
private:
    VarDefineBlockAST* node;
public:
    FunctionVarDefineBlockAST(VarDefineBlockAST* _n): AST(14), node(_n) {}

    void print();
    std::vector<AST*> get();
};

class FunctionExecuteBlockAST: public AST {
private:
    ExecuteInsBlockAST* node;
public:
    FunctionExecuteBlockAST(ExecuteInsBlockAST* _n): AST(15), node(_n) {}

    void print();
    std::vector<AST*> get();
};

class ArgDefinePartAST: public AST {
private:
    std::vector<VarDefineInsAST*> nodes;
public:
    ArgDefinePartAST(std::vector<VarDefineInsAST*> _n): AST(16), nodes(_n) {}

    void print();
    std::vector<AST*> get();
};

class ExecuteInsBlockAST: public AST {
private:
    ExecuteInsPartAST* node;
public:
    ExecuteInsBlockAST(): AST(18), node(NULL) {}
    ExecuteInsBlockAST(ExecuteInsPartAST* _n): AST(18), node(_n) {}

    void print();
    std::vector<AST*> get();
};

class ExecuteInsPartAST: public AST {
private:
    std::vector<ExecuteInsAST*> nodes;
public:
    ExecuteInsPartAST(std::vector<ExecuteInsAST*> _n): AST(19), nodes(_n) {}

    void print();
    std::vector<AST*> get();
};

class EvaluationInsAST: public AST {
private:
    MemoryStructAST* left;
    ExprAST* right;
public:
    EvaluationInsAST(MemoryStructAST* _l, ExprAST* _r): AST(21), left(_l), right(_r) {}
    EvaluationInsAST(ExprAST* _r): AST(21), left(NULL), right(_r) {}

    void print();
    std::vector<AST*> get();
};

class ForInsAST: public AST {
private:
    MemoryStructAST* var;
    ExprAST* left;
    ExprAST* right;
    MainExecutePartAST* main;
    Token op; // to or downto
public:
    ForInsAST(MemoryStructAST* _v, ExprAST* _l, ExprAST* _r, MainExecutePartAST* _m, Token _op): AST(25), var(_v), left(_l), right(_r), main(_m), op(_op) {}

    void print();
    std::vector<AST*> get();

    bool isUpper() { return (op.str == "to"); }
};

class ForeachInsAST: public AST {
private:
    MemoryStructAST* var;
    MemoryStructAST* arr;
    MainExecutePartAST* main;
public:
    ForeachInsAST(MemoryStructAST* _v, MemoryStructAST* _a, MainExecutePartAST* _m): AST(26), var(_v), arr(_a), main(_m) {}

    void print();
    std::vector<AST*> get();
};

class WhileInsAST: public AST {
private:
    ExprAST* node;
    MainExecutePartAST* main;
public:
    WhileInsAST(ExprAST* _n, MainExecutePartAST* _m): AST(27), node(_n), main(_m) {}

    void print();
    std::vector<AST*> get();
};

class LoopInsAST: public AST {
private:
    AST* node;
    int type; // 0 for for, 1 for foreach, 2 for while
public:
    LoopInsAST(ForInsAST* _n): AST(22), node(_n), type(0) {}
    LoopInsAST(ForeachInsAST* _n): AST(22), node(_n), type(1) {}
    LoopInsAST(WhileInsAST* _n): AST(22), node(_n), type(2) {}

    void print();
    std::vector<AST*> get();
};

class IfInsAST: public AST {
private:
    ExprAST* node;
    MainExecutePartAST* then_main;
    MainExecutePartAST* else_main;
public:
    IfInsAST(ExprAST* _n, MainExecutePartAST* _t, MainExecutePartAST* _e): AST(28), node(_n), then_main(_t), else_main(_e) {}
    IfInsAST(ExprAST* _n, MainExecutePartAST* _t): AST(28), node(_n), then_main(_t), else_main(NULL) {}

    void print();
    std::vector<AST*> get();
};

// only 'if', maybe we can add something more
class ConditionInsAST: public AST {
private:
    AST* node;
public:
    ConditionInsAST(IfInsAST* _n): AST(23), node(_n) {}

    void print();
    std::vector<AST*> get();
};

class ReturnInsAST: public AST {
private:
    ExprAST* node;
public:
    ReturnInsAST(): AST(24), node(NULL) {}
    ReturnInsAST(ExprAST* _n): AST(24), node(_n) {}

    void print();
    std::vector<AST*> get();
};

class ExecuteInsAST: public AST {
private:
    AST* node;
    int type; // 0 for eva, 1 for loop, 2 for cond, 3 for ret
public:
    ExecuteInsAST(EvaluationInsAST* _n): AST(20), node(_n), type(0) {}
    ExecuteInsAST(LoopInsAST* _n): AST(20), node(_n), type(1) {}
    ExecuteInsAST(ConditionInsAST* _n): AST(20), node(_n), type(2) {}
    ExecuteInsAST(ReturnInsAST* _n): AST(20), node(_n), type(3) {}

    void print();
    int getType() { return type; }
    std::vector<AST*> get();
};

// this class can be NULL
class MainExecutePartAST: public AST {
private:
    AST* node;
    int type; // 0 for NULL, 1 for ins, 2 for ins_block
public:
    MainExecutePartAST(): AST(17), node(NULL), type(0) {}
    MainExecutePartAST(ExecuteInsAST* _n): AST(17), node(_n), type(1) {}
    MainExecutePartAST(ExecuteInsBlockAST* _n): AST(17), node(_n), type(2) {}

    void print();
    std::vector<AST*> get();
    int getType() { return type; }
};

class ExprAST: public AST {
private:
    ExprAST* left;
    Term1AST* right;
    TokenAST* op;

    std::string typeName;

public:
    ExprAST(ExprAST* _l, Term1AST* _r, Token _op): AST(29), left(_l), right(_r) {
        op = new TokenAST(_op);
    }
    ExprAST(Term1AST* _r): AST(29), left(NULL), right(_r), op(NULL) {}

    void print();
    std::vector<AST*> get();

    void setTypeName(std::string _typeName) { typeName = _typeName; }
    std::string getTypeName() { return typeName; }
};

class Term1AST: public AST {
private:
    Term1AST* left;
    Term2AST* right;
    TokenAST* op;

    std::string typeName;

public:
    Term1AST(Term1AST* _l, Term2AST* _r, Token _op): AST(30), left(_l), right(_r) {
        op = new TokenAST(_op);
    }
    Term1AST(Term2AST* _r): AST(30), left(NULL), right(_r), op(NULL) {}

    void print();
    std::vector<AST*> get();

    void setTypeName(std::string _typeName) { typeName = _typeName; }
    std::string getTypeName() { return typeName; }
};

class Term2AST: public AST {
private:
    Term2AST* left;
    Term3AST* right;
    TokenAST* op;

    std::string typeName;

public:
    Term2AST(Term2AST* _l, Term3AST* _r, Token _op): AST(31), left(_l), right(_r) {
        op = new TokenAST(_op);
    }
    Term2AST(Term3AST* _r): AST(31), left(NULL), right(_r), op(NULL) {}

    void print();
    std::vector<AST*> get();

    void setTypeName(std::string _typeName) { typeName = _typeName; }
    std::string getTypeName() { return typeName; }
};

class ValueStructAST: public AST {
private:
    MemoryStructAST* node;
    TokenAST* value;
    int type; // 0 for MemoryStruct, 1 for Token

    std::string typeName;

public:
    ValueStructAST(MemoryStructAST* _n): AST(33), node(_n), value(NULL), type(0) {}
    ValueStructAST(Token _v): AST(33), node(NULL), type(1) {
        value = new TokenAST(_v);
    }

    void print();
    int getType() { return type; }
    std::vector<AST*> get();

    void setTypeName(std::string _typeName) { typeName = _typeName; }
    std::string getTypeName() { return typeName; }
};

class Term3AST: public AST {
private:
    AST* node;
    TokenAST* op1;
    TokenAST* op2;

    std::string typeName;

public:
    Term3AST(ExprAST* _n, Token _op1, Token _op2): AST(32), node(_n) {
        op1 = new TokenAST(_op1);
        op2 = new TokenAST(_op2);
    }
    Term3AST(Term3AST* _n, Token _op1): AST(32), node(_n), op2(NULL) {
        op1 = new TokenAST(_op1);
    }
    Term3AST(ValueStructAST* _n): AST(32), node(_n), op1(NULL), op2(NULL) {}

    void print();
    std::vector<AST*> get();

    void setTypeName(std::string _typeName) { typeName = _typeName; }
    std::string getTypeName() { return typeName; }
};

class ArrayAST: public AST {
private:
    IndexPartAST* node;
    TokenAST* id;

    std::string typeName;
    std::string idName;
    int maxIndexLevel;
    bool defined;

public:
    ArrayAST(IndexPartAST* _n, Token _id): AST(35), node(_n), defined(false) {
        id = new TokenAST(_id);
    }
    ArrayAST(Token _id): AST(35), node(NULL), defined(false) {
        id = new TokenAST(_id);
    }

    void print();
    std::vector<AST*> get();

    void setTypeName(std::string _typeName) { typeName = _typeName; }
    std::string getTypeName() { return typeName; }
    void setIdName(std::string _idName) { idName = _idName; }
    std::string getIdName() { return idName; }
    void setMaxIndexLevel(int _maxIndexLevel) { maxIndexLevel = _maxIndexLevel; }
    int getMaxIndexLevel() { return maxIndexLevel; }
    void changeDefined() { defined = !defined; }
    bool isDefined() { return defined; }
};

class FunctionCallAST: public AST {
private:
    ArgPartAST* node;
    TokenAST* id;

    std::string typeName;

public:
    FunctionCallAST(ArgPartAST* _n, Token _id): AST(37), node(_n) {
        id = new TokenAST(_id);
    }
    FunctionCallAST(Token _id): AST(37), node(NULL) {
        id = new TokenAST(_id);
    }

    void print();
    std::vector<AST*> get();

    void setTypeName(std::string _typeName) { typeName = _typeName; }
    std::string getTypeName() { return typeName; }
};

class MemoryStructAST: public AST {
private:
    MemoryStructAST* left;
    AST* right;
    int type; // 0 for M.F, 1 for M.A, 2 for F, 3 for A

    std::string typeName;

public:
    MemoryStructAST(MemoryStructAST* _l, FunctionCallAST* _r): AST(34), left(_l), right(_r), type(0) {}
    MemoryStructAST(MemoryStructAST* _l, ArrayAST* _r): AST(34), left(_l), right(_r), type(1) {}
    MemoryStructAST(FunctionCallAST* _r): AST(34), left(NULL), right(_r), type(2) {}
    MemoryStructAST(ArrayAST* _r): AST(34), left(NULL), right(_r), type(3) {}

    void print();
    int getType() { return type; }
    std::vector<AST*> get();

    void setTypeName(std::string _typeName) { typeName = _typeName; }
    std::string getTypeName() { return typeName; }
};

class IndexPartAST: public AST {
private:
    std::vector<ExprAST*> nodes;

    int maxIndexLevel;

public:
    IndexPartAST(std::vector<ExprAST*> _n): AST(36), nodes(_n) {}

    void print();
    std::vector<AST*> get();

    void setMaxIndexLevel(int _maxIndexLevel) { maxIndexLevel = _maxIndexLevel; }
    int getMaxIndexLevel() { return maxIndexLevel; }
};

class ArgPartAST: public AST {
private:
    std::vector<ExprAST*> nodes;

    std::vector<std::string> typeNames;

public:
    ArgPartAST(std::vector<ExprAST*> _n): AST(38), nodes(_n) {}

    void print();
    std::vector<AST*> get();

    void addTypeNames(std::string _typeName) { typeNames.push_back(_typeName); }
    std::vector<std::string> getTypeNames() { return typeNames; }
};

#endif // ASTCONST

