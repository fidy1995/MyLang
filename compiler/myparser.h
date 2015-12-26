#ifndef MYPARSER
#define MYPARSER

#include "astconst.h"

class Parser {
private:
    std::vector<Token> old_tokens; // origin tokens, only skip_command will deal with them
    std::vector<Token> tokens; // tokens except comment, all functions except skip_comment deal with them
    int error;

    ProgramAST* program_parser(int lpos, int rpos); // [lpos, npos]

    TypeDefineBlockAST* type_define_block_parser(int lpos, int rpos);
    VarDefineBlockAST* var_define_block_parser(int lpos, int rpos);
    FunctionDefineBlockAST* function_define_block_parser(int lpos, int rpos);
    ProgramExecuteBlockAST* program_execute_block_parser(int lpos, int rpos);

    TypeDefinePartAST* type_define_part_parser(int lpos, int rpos);
    TypeDefineInsAST* type_define_ins_parser(int lpos, int rpos);
    ClassBlockAST* class_block_parser(int lpos, int rpos);
    ClassVarPartAST* class_var_part_parser(int lpos, int rpos);
    ClassFunctionPartAST* class_function_part_parser(int lpos, int rpos);

    VarDefineInsPartAST* var_define_ins_part_parser(int lpos, int rpos);
    VarDefineInsAST* var_define_ins_parser(int lpos, int rpos);

    FunctionDefinePartAST* function_define_part_parser(int lpos, int rpos);
    FunctionNameAST* function_name_parser(int lpos, int rpos);
    FunctionVarDefineBlockAST* function_var_define_block_parser(int lpos, int rpos);
    FunctionExecuteBlockAST* function_execute_block_parser(int lpos, int rpos);
    ArgDefinePartAST* arg_define_part_parser(int lpos, int rpos);

    MainExecutePartAST* main_execute_part_parser(int lpos, int rpos);
    ExecuteInsBlockAST* execute_ins_block_parser(int lpos, int rpos);
    ExecuteInsPartAST* execute_ins_part_parser(int lpos, int rpos);

    ExecuteInsAST* execute_ins_parser(int lpos, int rpos);
    EvaluationInsAST* evaluation_ins_parser(int lpos, int rpos);
    LoopInsAST* loop_ins_parser(int lpos, int rpos);
    ConditionInsAST* condition_ins_parser(int lpos, int rpos);
    ReturnInsAST* return_ins_parser(int lpos, int rpos);
    ForInsAST* for_ins_parser(int lpos, int rpos);
    ForeachInsAST* foreach_ins_parser(int lpos, int rpos);
    WhileInsAST* while_ins_parser(int lpos, int rpos);
    IfInsAST* if_ins_parser(int lpos, int rpos);

    ExprAST* expr_parser(int lpos, int rpos);
    Term1AST* term1_parser(int lpos, int rpos);
    Term2AST* term2_parser(int lpos, int rpos);
    Term3AST* term3_parser(int lpos, int rpos);

    ValueStructAST* value_struct_parser(int lpos, int rpos);
    MemoryStructAST* memory_struct_parser(int lpos, int rpos);
    FunctionCallAST* function_call_parser(int lpos, int rpos);
    ArrayAST* array_parser(int lpos, int rpos);
    ArgPartAST* arg_part_parser(int lpos, int rpos);
    IndexPartAST* index_part_parser(int lpos, int rpos);

    void skip_comment();

public:
    Parser(std::vector<Token> _t);
    AST* do_parsing(int& _error);
};

#endif // MYPARSER

