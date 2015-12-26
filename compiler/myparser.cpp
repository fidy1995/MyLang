#include "myparser.h"

Parser::Parser(std::vector<Token> _t) {
    old_tokens = _t;
    error = 0;
    skip_comment(); // skip comments in old_tokens, and put result in tokens
}

ProgramAST* Parser::program_parser(int lpos, int rpos) {
    ProgramAST* node = NULL;

    // at least <begin end .> three tokens
    // check end and .
    if (rpos - lpos < 2 || tokens[rpos - 1].str != "end") {
        error = 7;
        return node;
    }
    if (tokens[rpos].str != ".") {
        error = 14;
        return node;
    }

    // end_num
    int begin_num = 0;
    int end_num = 1;

    // var, function, main position
    int var_pos = -1;
    int func_pos = -1;
    int main_pos = -1;

    // from right to left
    // search for main begin
    // skip first two
    for (int i = rpos - 2; i >= lpos; i--) {
        if (tokens[i].str == "begin") {
            end_num--;
            if (end_num == 0) {
                main_pos = i;
                break;
            }
            continue;
        }
        if (tokens[i].str == "end") {
            end_num++;
            continue;
        }
    }

    // main not found? error
    if (main_pos == -1) {
        error = 8;
        return node; // NULL
    }

    // search for var
    // from left to right
    // skip all things in begin-end
    // var must be smaller than func
    for (int i = lpos; i < main_pos; i++) {
        if (tokens[i].str == "begin") {
            begin_num++;
            continue;
        }
        if (tokens[i].str == "end") {
            begin_num--;
            continue;
        }

        // begin-end not match
        if (begin_num < 0) {
            error = 9;
            return node; // NULL
        }

        // not in begin end
        if (begin_num == 0) {
            if (tokens[i].str == "var" && var_pos == -1) {
                var_pos = i;
            }
            if (tokens[i].str == "function") {
                func_pos = i;
                break;  // don't need to do more
            }
        }
    }

    if (func_pos == -1) {
        func_pos = main_pos;
    }

    if (var_pos == -1) {
        var_pos = func_pos;
    }

    node = new ProgramAST(type_define_block_parser(lpos, var_pos - 1), var_define_block_parser(var_pos, func_pos - 1), function_define_block_parser(func_pos, main_pos - 1), program_execute_block_parser(main_pos, rpos));

    return node;
}

TypeDefineBlockAST* Parser::type_define_block_parser(int lpos, int rpos) {
    TypeDefineBlockAST* node = NULL;

    // choose method
    // notice it can be NULL
    if (lpos > rpos) {
        node = new TypeDefineBlockAST(0);
    }
    else if (tokens[lpos].str == "type") {
        if (lpos == rpos) {
            node = new TypeDefineBlockAST(1);
        }
        else {
            node = new TypeDefineBlockAST(type_define_part_parser(lpos + 1, rpos));
        }
    }
    else {
        error = 10;
        return node; // NULL
    }

    return node;
}

VarDefineBlockAST* Parser::var_define_block_parser(int lpos, int rpos) {
    VarDefineBlockAST* node = NULL;

    // choose method
    // notice it can be NULL
    if (lpos > rpos) {
        node = new VarDefineBlockAST(0);
    }
    else if (tokens[lpos].str == "var") {
        if (lpos == rpos) {
            node = new VarDefineBlockAST(1);
        }
        else {
            node = new VarDefineBlockAST(var_define_ins_part_parser(lpos + 1, rpos));
        }
    }
    else {
        error = 11;
        return node; // NULL
    }

    return node;
}

FunctionDefineBlockAST* Parser::function_define_block_parser(int lpos, int rpos) {
    FunctionDefineBlockAST* node = NULL;

    std::vector<FunctionDefinePartAST*> child;

    // lpos > rpos? choose NULL
    if (lpos > rpos) {
        node = new FunctionDefineBlockAST(child);
        return node;
    }

    // the first token is not function? error
    // the end token is not ;? error
    if (tokens[lpos].str != "function") {
        error = 12;
        return node; // NULL
    }
    if (tokens[rpos].str != ";") {
        error = 13;
        return node; // NULL
    }

    // for last 'function' position
    int last_pos = lpos;

    // search for 'function', skip the first one
    for (int i = lpos + 1; i <= rpos; i++) {
        if (tokens[i].str == "function") {
            // we should check ';'
            if (tokens[i - 1].str != ";") {
                error = 13;
                return node; // NULL
            }
            child.push_back(function_define_part_parser(last_pos, i - 2));
            last_pos = i;
        }
    }
    child.push_back(function_define_part_parser(last_pos, rpos - 1));

    node = new FunctionDefineBlockAST(child);

    return node;
}

ProgramExecuteBlockAST* Parser::program_execute_block_parser(int lpos, int rpos) {
    ProgramExecuteBlockAST* node = NULL;

    // lpos>rpos? error
    // also the last token must be '.'
    if (lpos > rpos || tokens[rpos].str != ".") {
        error = 7;
        return node; // NULL
    }

    node = new ProgramExecuteBlockAST(execute_ins_block_parser(lpos, rpos - 1));

    return node;
}

TypeDefinePartAST* Parser::type_define_part_parser(int lpos, int rpos) {
    TypeDefinePartAST* node = NULL;

    std::vector<TypeDefineInsAST*> child;

    int tmp_l = lpos;
    int tmp_r = rpos;

    while (true) {
        // need to count the num of begin
        // we should pass all the things in them when searching ';'.
        int begin_num = 0;

        // ;'s position
        int semicolon_pos = -1;

        // tmp_l > tmp_r? break
        if (tmp_l > tmp_r) {
            break;
        }

        // search for first ';'
        // notice we should pass all the context in begin-end
        for (int i = tmp_l; i <= tmp_r; i++) {
            // deal with begin-end first because they control begin_num
            if (tokens[i].type == RESERVEDWORDNO) {
                if (tokens[i].str == "begin") {
                    begin_num++;
                    continue;
                }
                if (tokens[i].str == "end") {
                    begin_num--;
                    continue;
                }
            }

            // error: begin-end not match
            if (begin_num < 0) {
                error = 9;
                return node; // NULL
            }

            // if now we are not in begin-end
            // see whether the token is ';'
            if (begin_num == 0) {
                if (tokens[i].str == ";") {
                    semicolon_pos = i;
                    break;
                }
            }
        }

        // choose method
        // ; not found? error
        if (semicolon_pos == -1) {
            error = 13;
            return node; // NULL
        }
        else {
            child.push_back(type_define_ins_parser(tmp_l, semicolon_pos - 1));
            tmp_l = semicolon_pos + 1;
        }
    }

    // error case: no child
    if (child.size() == 0) {
        error = 7;
        return node;
    }

    node = new TypeDefinePartAST(child);

    return node;
}

TypeDefineInsAST* Parser::type_define_ins_parser(int lpos, int rpos) {
    TypeDefineInsAST* node = NULL;

    // template (Token(ID) = Token(ID/RT)...)
    // so at least three tokens, and the second is '='
    if (rpos - lpos < 2 || tokens[lpos + 1].str != "=") {
        error = 7;
        return node; // NULL
    }

    if (tokens[lpos].type != IDENTIFIERNO || (tokens[lpos + 2].type != IDENTIFIERNO && tokens[lpos + 2].type != RESERVEDTYPENO)) {
        error = 10;
        return node; // NULL
    }

    // choose method
    if (rpos - lpos == 2) {
        node = new TypeDefineInsAST(tokens[lpos], tokens[lpos + 2]);
    }
    else {
        // if RT, then must be 'class'
        if (tokens[lpos + 2].type == RESERVEDTYPENO && tokens[lpos + 2].str != "class") {
            error = 10;
            return node;
        }

        node = new TypeDefineInsAST(class_block_parser(lpos + 3, rpos), tokens[lpos], tokens[lpos + 2]);
    }

    return node;
}

ClassBlockAST* Parser::class_block_parser(int lpos, int rpos) {
    ClassBlockAST* node = NULL;

    // lpos > rpos? error
    // check begin-end
    if (lpos > rpos || tokens[lpos].str != "begin" || tokens[rpos].str != "end") {
        error = 10;
        return node; // NULL
    }

    // for 'function' position
    int function_pos = -1;

    // search for first 'function', can skip the first and last one
    for (int i = lpos + 1; i < rpos; i++) {
        if (tokens[i].str == "function") {
            function_pos = i;
            break;
        }
    }

    if (function_pos == -1) {
        function_pos = rpos; // set class_function_part to NULL
    }

    node = new ClassBlockAST(class_var_part_parser(lpos + 1, function_pos - 1), class_function_part_parser(function_pos, rpos - 1));

    return node;
}

ClassVarPartAST* Parser::class_var_part_parser(int lpos, int rpos) {
    ClassVarPartAST* node = NULL;

    // choose method
    if (lpos > rpos) {
        node = new ClassVarPartAST();
    }
    else {
        node = new ClassVarPartAST(var_define_ins_part_parser(lpos, rpos));
    }

    return node;
}

ClassFunctionPartAST* Parser::class_function_part_parser(int lpos, int rpos) {
    ClassFunctionPartAST* node = NULL;
    node = new ClassFunctionPartAST(function_define_block_parser(lpos, rpos));
    return node;
}

VarDefineInsPartAST* Parser::var_define_ins_part_parser(int lpos, int rpos) {
    VarDefineInsPartAST* node = NULL;

    std::vector<VarDefineInsAST*> child;

    // lpos > rpos? error
    if (lpos > rpos) {
        error = 11;
        return node; // NULL
    }

    // for last ';' position
    int last_pos = lpos - 1;

    // search for first ';'
    for (int i = lpos; i <= rpos; i++) {
        if (tokens[i].str == ";") {
            child.push_back(var_define_ins_parser(last_pos + 1, i - 1));
            last_pos = i;
        }
    }

    // error case: no child or lost ;
    if (child.size() == 0 || last_pos != rpos) {
        error = 13;
        return node; // NULL
    }

    node = new VarDefineInsPartAST(child);

    return node;
}

VarDefineInsAST* Parser::var_define_ins_parser(int lpos, int rpos) {
    VarDefineInsAST* node = NULL;

    // template id (Array) : Token(ID/RT)
    // so at least three tokens, the the second from right is ':'
    if (rpos - lpos < 2 || tokens[rpos - 1].str != ":") {
        error = 11;
        return node; // NULL
    }

    // also we need to check the last token
    if (tokens[rpos].type != IDENTIFIERNO && tokens[rpos].type != RESERVEDTYPENO) {
        error = 11;
        return node; // NULL
    }

    node = new VarDefineInsAST(array_parser(lpos, rpos - 2), tokens[rpos]);

    return node;
}

FunctionDefinePartAST* Parser::function_define_part_parser(int lpos, int rpos) {
    FunctionDefinePartAST* node = NULL;

    // lpos > rpos? error
    if (lpos > rpos || tokens[lpos].str != "function") {
        error = 12;
        return node;
    }

    // var and begin's position
    int var_pos = -1;
    int begin_pos = -1;

    // no need for lpos because of the first token 'function'
    // notice var_pos < begin_pos is always true
    for (int i = lpos + 1; i <= rpos; i++) {
        if (tokens[i].str == "var") {
            var_pos = i;
        }
        else if (tokens[i].str == "begin") {
            begin_pos = i;
            break; // no need for continues
        }
    }

    // begin not exists? error;
    // notice var may not exist
    if (begin_pos == -1) {
        error = 12;
        return node; // NULL
    }

    if (var_pos == -1) {
        var_pos = begin_pos; // make var part empty
    }

    node = new FunctionDefinePartAST(function_name_parser(lpos, var_pos - 1), function_var_define_block_parser(var_pos, begin_pos - 1), function_execute_block_parser(begin_pos, rpos));

    return node;
}

FunctionNameAST* Parser::function_name_parser(int lpos, int rpos) {
    FunctionNameAST* node = NULL;

    // lpos > rpos? error
    if (lpos > rpos) {
        error = 12;
        return node;
    }

    // template is <function ID (...)> | <function ID (...) : ID|RT>
    // the difference between them is the last token
    if (tokens[rpos].str == ")") { // for first template
        if (rpos - lpos < 3 || tokens[lpos].str != "function" || tokens[lpos + 1].type != IDENTIFIERNO || tokens[lpos + 2].str != "(") {
            error = 12;
            return node; // NULL
        }
        node = new FunctionNameAST(arg_define_part_parser(lpos + 3, rpos - 1), tokens[lpos + 1]);
    }
    else {
        if (rpos - lpos < 5 || tokens[lpos].str != "function" || tokens[lpos + 1].type != IDENTIFIERNO || tokens[lpos + 2].str != "(") {
            error = 12;
            return node; // NULL
        }
        else if ((tokens[rpos].type != IDENTIFIERNO && tokens[rpos].type != RESERVEDTYPENO) || tokens[rpos - 1].str != ":" || tokens[rpos - 2].str != ")") {
            error = 12;
            return node; // NULL
        }
        node = new FunctionNameAST(arg_define_part_parser(lpos + 3, rpos - 3), tokens[lpos + 1], tokens[rpos]);
    }

    return node;
}

FunctionVarDefineBlockAST* Parser::function_var_define_block_parser(int lpos, int rpos) {
    FunctionVarDefineBlockAST* node = NULL;
    node = new FunctionVarDefineBlockAST(var_define_block_parser(lpos, rpos));
    return node;
}

FunctionExecuteBlockAST* Parser::function_execute_block_parser(int lpos, int rpos) {
    FunctionExecuteBlockAST* node = NULL;
    node = new FunctionExecuteBlockAST(execute_ins_block_parser(lpos, rpos));
    return node;
}

ArgDefinePartAST* Parser::arg_define_part_parser(int lpos, int rpos) {
    ArgDefinePartAST* node = NULL;

    std::vector<VarDefineInsAST*> child;

    // lpos > rpos? choose NULL
    if (lpos > rpos) {
        node = new ArgDefinePartAST(child);
        return node; // NULL
    }

    // for last ',' position
    int last_pos = lpos - 1;

    // search for ','
    for (int i = lpos; i <= rpos; i++) {
        if (tokens[i].str == ",") {
            child.push_back(var_define_ins_parser(last_pos + 1, i - 1));
            last_pos = i;
        }
    }
    child.push_back(var_define_ins_parser(last_pos + 1, rpos));

    node = new ArgDefinePartAST(child);

    return node;
}

MainExecutePartAST* Parser::main_execute_part_parser(int lpos, int rpos) {
    MainExecutePartAST* node = NULL;

    // choose method
    // notice it can be NULL
    if (lpos > rpos) {
        node = new MainExecutePartAST();
    }
    else if (tokens[lpos].str == "begin") {
        node = new MainExecutePartAST(execute_ins_block_parser(lpos, rpos));
    }
    else {
        node = new MainExecutePartAST(execute_ins_parser(lpos, rpos));
    }

    return node;
}

ExecuteInsBlockAST* Parser::execute_ins_block_parser(int lpos, int rpos) {
    ExecuteInsBlockAST* node = NULL;

    // choose method
    // notice cases of error
    if (rpos - lpos < 1 || tokens[lpos].str != "begin" || tokens[rpos].str != "end") {
        error = 7;
        return node; // NULL
    }
    else if (rpos - lpos == 1) {
        node = new ExecuteInsBlockAST();
    }
    else {
        node = new ExecuteInsBlockAST(execute_ins_part_parser(lpos + 1, rpos - 1));
    }

    return node;
}

ExecuteInsPartAST* Parser::execute_ins_part_parser(int lpos, int rpos) {
    ExecuteInsPartAST* node = NULL;

    std::vector<ExecuteInsAST*> child;

    int tmp_l = lpos;
    int tmp_r = rpos;

    while (true) {
        // need to count the num of begin
        // we should pass all the things in them when searching ';'.
        int begin_num = 0;

        // ;'s position
        int semicolon_pos = -1;

        // tmp_l > tmp_r? break
        if (tmp_l > tmp_r) {
            break;
        }

        // search for first ';'
        // notice we should pass all the context in begin-end
        for (int i = tmp_l; i <= tmp_r; i++) {
            // deal with begin-end first because they control begin_num
            if (tokens[i].type == RESERVEDWORDNO) {
                if (tokens[i].str == "begin") {
                    begin_num++;
                    continue;
                }
                if (tokens[i].str == "end") {
                    begin_num--;
                    continue;
                }
            }

            // error: begin-end not match
            if (begin_num < 0) {
                error = 9;
                return node; // NULL
            }

            // if now we are not in begin-end
            // see whether the token is ';'
            if (begin_num == 0) {
                if (tokens[i].str == ";") {
                    semicolon_pos = i;
                    break;
                }
            }
        }

        // choose method
        // ; not found? error
        if (semicolon_pos == -1) {
            error = 13;
            return node; // NULL
        }
        else {
            child.push_back(execute_ins_parser(tmp_l, semicolon_pos - 1));
            tmp_l = semicolon_pos + 1;
        }
    }

    // error case: no child
    if (child.size() == 0) {
        error = 7;
        return node;
    }

    node = new ExecuteInsPartAST(child);

    return node;
}

ExecuteInsAST* Parser::execute_ins_parser(int lpos, int rpos) {
    ExecuteInsAST* node = NULL;

    // lpos > rpos? error
    if (lpos > rpos) {
        error = 7;
        return node; // NULL
    }

    // choose method
    if (tokens[lpos].str == "for" || tokens[lpos].str == "foreach" || tokens[lpos].str == "while") {
        node = new ExecuteInsAST(loop_ins_parser(lpos, rpos));
    }
    else if (tokens[lpos].str == "if") {
        node = new ExecuteInsAST(condition_ins_parser(lpos, rpos));
    }
    else if (tokens[lpos].str == "return") {
        node = new ExecuteInsAST(return_ins_parser(lpos, rpos));
    }
    else {
        node = new ExecuteInsAST(evaluation_ins_parser(lpos, rpos));
    }

    return node;
}

EvaluationInsAST* Parser::evaluation_ins_parser(int lpos, int rpos) {
    EvaluationInsAST* node = NULL;

    // lpos > rpos? error
    if (lpos > rpos) {
        error = 7;
        return node; // NULL
    }

    // equal's position
    int equal_pos = -1;

    // search for '='
    for (int i = lpos; i <= rpos; i++) {
        if (tokens[i].str == "=") {
            equal_pos = i;
            break;
        }
    }

    // choose method
    if (equal_pos == -1) {
        node = new EvaluationInsAST(expr_parser(lpos, rpos));
    }
    else {
        node = new EvaluationInsAST(memory_struct_parser(lpos, equal_pos - 1), expr_parser(equal_pos + 1, rpos));
    }

    return node;
}

LoopInsAST* Parser::loop_ins_parser(int lpos, int rpos) {
    LoopInsAST* node = NULL;

    // lpos > rpos? error
    if (lpos > rpos) {
        error = 7;
        return node; // NULL
    }

    // choose method
    if (tokens[lpos].str == "for") {
        node = new LoopInsAST(for_ins_parser(lpos, rpos));
    }
    else if (tokens[lpos].str == "foreach") {
        node = new LoopInsAST(foreach_ins_parser(lpos, rpos));
    }
    else if (tokens[lpos].str == "while") {
        node = new LoopInsAST(while_ins_parser(lpos, rpos));
    }
    else {
        error = 7;
    }

    return node;
}

ConditionInsAST* Parser::condition_ins_parser(int lpos, int rpos) {
    ConditionInsAST* node = new ConditionInsAST(if_ins_parser(lpos, rpos));
    return node;
}

ReturnInsAST* Parser::return_ins_parser(int lpos, int rpos) {
    ReturnInsAST* node = NULL;

    // lpos > rpos? choose NULL
    if (lpos > rpos || tokens[lpos].str != "return") {
        error = 7;
        return node; // NULL
    }

    // choose method
    if (lpos == rpos) {
        node = new ReturnInsAST();
    }
    else {
        node = new ReturnInsAST(expr_parser(lpos + 1, rpos));
    }

    return node;
}

ForInsAST* Parser::for_ins_parser(int lpos, int rpos) {
    ForInsAST* node = NULL;

    // lpos > rpos? 'for' is not the first? error
    if (lpos > rpos || tokens[lpos].str != "for") {
        error = 7;
        return node; // NULL
    }

    // position
    int equal_pos = -1;
    int to_pos = -1;
    int downto_pos = -1;
    int do_pos = -1;

    // search for all the things above
    // notice =, to, downto, do will only occur once before ins_block
    // notice do is the end of searching, so do_pos is the max
    for (int i = lpos + 1; i <= rpos; i++) {
        if (tokens[i].str == "=") {
            equal_pos = i;
        }
        else if (tokens[i].str == "to") {
            to_pos = i;
        }
        else if (tokens[i].str == "downto") {
            downto_pos = i;
        }
        else if (tokens[i].str == "do") {
            do_pos = i;
            break; // after 'do' must be ins_block, so we need to exit
        }
    }

    // doesn't exist, error
    if (equal_pos == -1 || (to_pos == -1 && downto_pos == -1) || do_pos == -1) {
        error = 7;
        return node; // NULL
    }

    // choose method
    // to and downto exist at the same time, error
    if (to_pos != -1 && downto_pos != -1) {
        error = 7;
        return node; // NULL
    }
    else if (to_pos != -1) { // to exist;
        if (equal_pos > to_pos) { // not in order, error, don't need to care do_pos
            error = 7;
            return node; //NULL
        }
        node = new ForInsAST(memory_struct_parser(lpos + 1, equal_pos - 1), expr_parser(equal_pos + 1, to_pos - 1), expr_parser(to_pos + 1, do_pos - 1), main_execute_part_parser(do_pos + 1, rpos), tokens[to_pos]);
    }
    else { // downto exist;
        if (equal_pos > downto_pos) { // not in order, error, don't need to care do_pos
            error = 7;
            return node; //NULL
        }
        node = new ForInsAST(memory_struct_parser(lpos + 1, equal_pos - 1), expr_parser(equal_pos + 1, downto_pos - 1), expr_parser(downto_pos + 1, do_pos - 1), main_execute_part_parser(do_pos + 1, rpos), tokens[downto_pos]);
    }

    return node;
}

ForeachInsAST* Parser::foreach_ins_parser(int lpos, int rpos) {
    ForeachInsAST* node = NULL;

    // lpos > rpos? 'foreach' is not the first? error
    if (lpos > rpos || tokens[lpos].str != "foreach") {
        error = 7;
        return node; // NULL
    }

    // position
    int in_pos = -1;
    int do_pos = -1;

    // search for all the things above
    // notice in, do will only occur once before ins_block
    // notice do is the end of searching, so do_pos is the max
    for (int i = lpos + 1; i <= rpos; i++) {
        if (tokens[i].str == "in") {
            in_pos = i;
        }
        else if (tokens[i].str == "do") {
            do_pos = i;
            break; // after 'do' must be ins_block, so we need to exit
        }
    }

    // doesn't exist, error
    if (in_pos == -1 || do_pos == -1) {
        error = 7;
        return node; // NULL
    }

    // needn't care about order because do_pos is the max
    // the only method
    node = new ForeachInsAST(memory_struct_parser(lpos + 1, in_pos - 1), memory_struct_parser(in_pos + 1, do_pos - 1), main_execute_part_parser(do_pos + 1, rpos));

    return node;
}

WhileInsAST* Parser::while_ins_parser(int lpos, int rpos) {
    WhileInsAST* node = NULL;

    // lpos > rpos? 'foreach' is not the first? error
    if (lpos > rpos || tokens[lpos].str != "while") {
        error = 7;
        return node; // NULL
    }

    // position
    int do_pos = -1;

    // search for do
    for (int i = lpos + 1; i <= rpos; i++) {
        if (tokens[i].str == "do") {
            do_pos = i;
            break;
        }
    }

    // doesn't exist, error
    if (do_pos == -1) {
        error = 7;
        return node; // NULL
    }

    // the only method
    node = new WhileInsAST(expr_parser(lpos + 1, do_pos - 1), main_execute_part_parser(do_pos + 1, rpos));

    return node;
}

IfInsAST* Parser::if_ins_parser(int lpos, int rpos) {
    IfInsAST* node = NULL;

    // notice the least structure is <if ... then>, <...> can not be empty
    // so at least three tokens
    if (rpos - lpos < 2 || tokens[lpos].str != "if") {
        error = 7;
        return node; // NULL
    }

    // the number of else, begin
    int else_num = 0;
    int end_num = 0;

    // then, else position
    int then_pos = -1;
    int else_pos = -1;

    // search for first then, can skip the first token
    for (int i = lpos + 1; i <= rpos; i++) {
        if (tokens[i].str == "then") {
            then_pos = i;
            break;
        }
    }

    // then not found? error
    if (then_pos == -1) {
        error = 7;
        return node; // NULL
    }

    // notice the finding should pass all the thing in end-begin
    // now we will find the last 'else' from R to L,
    // which makes the else_num added from 0 to 1
    // notice if 'then' exists when else_num is 0,
    // else_num will not still be 0 because the single 'then' can be also legal
    // notice we should not count the 'then' at then_pos
    for (int i = rpos; i > then_pos; i--) {
        if (tokens[i].str == "begin") {
            end_num--;
            continue;
        }
        if (tokens[i].str == "end") {
            end_num++;
            continue;
        }

        // begin-end not match? error
        if (end_num < 0) {
            error = 9;
            return node; // NULL
        }

        // only not in begin-end can do
        if (end_num == 0) {
            if (tokens[i].str == "then") {
                else_num--;
                if (else_num < 0) {
                    else_num = 0;
                }
                continue;
            }
            if (tokens[i].str == "else") {
                else_num++;
                if (else_num == 1) { // 0 to 1 case
                    else_pos = i;
                }
                continue;
            }
        }
    }

    // choose method
    if (else_num == 0) { // IF-THEN
        node = new IfInsAST(expr_parser(lpos + 1, then_pos - 1), main_execute_part_parser(then_pos + 1, rpos));
    }
    else if (else_num == 1){ // IF-THEN-ELSE
        node = new IfInsAST(expr_parser(lpos + 1, then_pos - 1), main_execute_part_parser(then_pos + 1, else_pos - 1), main_execute_part_parser(else_pos + 1, rpos));
    }
    else { // two or more else? error
        error = 15;
        return node; // NULL
    }

    return node;
}

ExprAST* Parser::expr_parser(int lpos, int rpos) {
    ExprAST* node = NULL;

    // lpos > rpos? error
    if (lpos > rpos) {
        error = 16;
        return node; // NULL
    }

    // need to count the num of right little/middle brackets
    // we should pass all the things in them.
    int lit_brac_num = 0;
    int mid_brac_num = 0;

    // operator's position
    int op_pos = -1;

    for (int i = rpos; i >= lpos; i--) {
        // deal with ()[] first because they control lit/mid_brac_num
        if (tokens[i].type == SPECIALSYMBOLNO) {
            if (tokens[i].str == "(") {
                lit_brac_num--;
                continue;
            }
            if (tokens[i].str == ")") {
                lit_brac_num++;
                continue;
            }
            if (tokens[i].str == "[") {
                mid_brac_num--;
                continue;
            }
            if (tokens[i].str == "]") {
                mid_brac_num++;
                continue;
            }
        }

        // error: brac not match
        if (lit_brac_num < 0 || mid_brac_num < 0) {
            error = 17;
            return node; // NULL
        }

        // if now we are not in brackets
        // see whether the token is ==, !=, <, >, <=, >=.
        if (lit_brac_num == 0 && mid_brac_num == 0) {
            if (isExprOperator(tokens[i].str)) {
                op_pos = i;
                break;
            }
        }
    }

    // choose method
    if (op_pos == -1) {
        node = new ExprAST(term1_parser(lpos, rpos));
    }
    else {
        node = new ExprAST(expr_parser(lpos, op_pos - 1), term1_parser(op_pos + 1, rpos), tokens[op_pos]);
    }

    return node;
}

Term1AST* Parser::term1_parser(int lpos, int rpos) {
    Term1AST* node = NULL;

    // lpos > rpos? error
    if (lpos > rpos) {
        error = 16;
        return node; // NULL
    }

    // need to count the num of right little/middle brackets
    // we should pass all the things in them.
    int lit_brac_num = 0;
    int mid_brac_num = 0;

    // operator's position
    int op_pos = -1;

    for (int i = rpos; i >= lpos; i--) {
        // deal with ()[] first because they control lit/mid_brac_num
        if (tokens[i].type == SPECIALSYMBOLNO) {
            if (tokens[i].str == "(") {
                lit_brac_num--;
                continue;
            }
            if (tokens[i].str == ")") {
                lit_brac_num++;
                continue;
            }
            if (tokens[i].str == "[") {
                mid_brac_num--;
                continue;
            }
            if (tokens[i].str == "]") {
                mid_brac_num++;
                continue;
            }
        }

        // error: brac not match
        if (lit_brac_num < 0 || mid_brac_num < 0) {
            error = 17;
            return node; // NULL
        }

        // if now we are not in brackets
        // see whether the token is +, -, or, xor.
        if (lit_brac_num == 0 && mid_brac_num == 0) {
            if (isTerm1Operator(tokens[i].str)) {
                op_pos = i;
                break;
            }
        }
    }

    // choose method
    if (op_pos == -1) {
        node = new Term1AST(term2_parser(lpos, rpos));
    }
    else {
        node = new Term1AST(term1_parser(lpos, op_pos - 1), term2_parser(op_pos + 1, rpos), tokens[op_pos]);
    }

    return node;
}

Term2AST* Parser::term2_parser(int lpos, int rpos) {
    Term2AST* node = NULL;

    // lpos > rpos? error
    if (lpos > rpos) {
        error = 16;
        return node; // NULL
    }

    // need to count the num of right little/middle brackets
    // we should pass all the things in them.
    int lit_brac_num = 0;
    int mid_brac_num = 0;

    // operator's position
    int op_pos = -1;

    for (int i = rpos; i >= lpos; i--) {
        // deal with ()[] first because they control lit/mid_brac_num
        if (tokens[i].type == SPECIALSYMBOLNO) {
            if (tokens[i].str == "(") {
                lit_brac_num--;
                continue;
            }
            if (tokens[i].str == ")") {
                lit_brac_num++;
                continue;
            }
            if (tokens[i].str == "[") {
                mid_brac_num--;
                continue;
            }
            if (tokens[i].str == "]") {
                mid_brac_num++;
                continue;
            }
        }

        // error: brac not match
        if (lit_brac_num < 0 || mid_brac_num < 0) {
            error = 17;
            return node; // NULL
        }

        // if now we are not in brackets
        // see whether the token is *, /, div, mod, and.
        if (lit_brac_num == 0 && mid_brac_num == 0) {
            if (isTerm2Operator(tokens[i].str)) {
                op_pos = i;
                break;
            }
        }
    }

    // choose method
    if (op_pos == -1) {
        node = new Term2AST(term3_parser(lpos, rpos));
    }
    else {
        node = new Term2AST(term2_parser(lpos, op_pos - 1), term3_parser(op_pos + 1, rpos), tokens[op_pos]);
    }

    return node;
}

Term3AST* Parser::term3_parser(int lpos, int rpos) {
    Term3AST* node = NULL;

    // lpos > rpos? error
    if (lpos > rpos) {
        error = 16;
        return node; // NULL
    }

    // choose method directly
    if (tokens[lpos].str == "not") {
        node = new Term3AST(term3_parser(lpos + 1, rpos), tokens[lpos]);
    }
    else if (tokens[lpos].str == "(" && tokens[rpos].str == ")") {
        node = new Term3AST(expr_parser(lpos + 1, rpos - 1), tokens[lpos], tokens[rpos]);
    }
    else {
        node = new Term3AST(value_struct_parser(lpos, rpos));
    }

    return node;
}

ValueStructAST* Parser::value_struct_parser(int lpos, int rpos) {
    ValueStructAST* node = NULL;

    // lpos > rpos? error
    if (lpos > rpos) {
        error = 16;
        return node; // NULL
    }

    // choose method directly
    switch (tokens[lpos].type) {
    case NUMBERNO: case CHARNO: case STRINGNO:
        // notice this token is the ONLY ONE we left
        if (lpos != rpos) {
            error = 16;
            return node; // NULL
        }

        node = new ValueStructAST(tokens[lpos]);
        break;
    default:
        node = new ValueStructAST(memory_struct_parser(lpos, rpos));
        break;
    }

    return node;
}

MemoryStructAST* Parser::memory_struct_parser(int lpos, int rpos) {
    MemoryStructAST* node = NULL;

    // need to count the num of right little/middle brackets
    // we should pass all the things in them when searching '.'.
    int lit_brac_num = 0;
    int mid_brac_num = 0;

    // flag for ')'/']'
    bool first_get = false;
    bool lit_brac = false;
    bool mid_brac = false;

    // .'s position
    int dot_pos = -1;

    // lpos > rpos? error
    if (lpos > rpos) {
        error = 16;
        return node; // NULL
    }

    // search for first '.' from right to left
    // notice we should pass all the context in ()[]
    for (int i = rpos; i >= lpos; i--) {
        // deal with ()[] first because they control lit/mid_brac_num
        if (tokens[i].type == SPECIALSYMBOLNO) {
            if (tokens[i].str == "(") {
                lit_brac_num--;
                continue;
            }
            if (tokens[i].str == ")") {
                if (!first_get) {
                    lit_brac = true;
                    first_get = true;
                }
                lit_brac_num++;
                continue;
            }
            if (tokens[i].str == "[") {
                mid_brac_num--;
                continue;
            }
            if (tokens[i].str == "]") {
                if (!first_get) {
                    mid_brac = true;
                    first_get = true;
                }
                mid_brac_num++;
                continue;
            }
        }

        // error: brac not match
        if (lit_brac_num < 0 || mid_brac_num < 0) {
            error = 17;
            return node; // NULL
        }

        // if now we are not in brackets
        // see whether the token is '.'
        if (lit_brac_num == 0 && mid_brac_num == 0) {
            if (tokens[i].str == ".") {
                dot_pos = i;
                break;
            }
        }
    }

    // choose method
    if (dot_pos == -1) {
        // dot doesn't exists
        if (lit_brac) {
            node = new MemoryStructAST(function_call_parser(lpos, rpos));
        }
        else {
            node = new MemoryStructAST(array_parser(lpos, rpos));
        }
    }
    else {
        // dot exists
        if (lit_brac) {
            node = new MemoryStructAST(memory_struct_parser(lpos, dot_pos - 1), function_call_parser(dot_pos + 1, rpos));
        }
        else {
            node = new MemoryStructAST(memory_struct_parser(lpos, dot_pos - 1), array_parser(dot_pos + 1, rpos));
        }
    }

    return node;
}

FunctionCallAST* Parser::function_call_parser(int lpos, int rpos) {
    FunctionCallAST* node = NULL;

    // notice the least number of tokens is 3
    if (rpos - lpos < 2) {
        error = 19;
        return node; // NULL
    }

    // notice the ONLY templet is < id(...) >
    if (tokens[lpos].type != IDENTIFIERNO || tokens[lpos + 1].str != "(" || tokens[rpos].str != ")") {
        error = 19;
        return node; // NULL
    }

    // choose method
    if (rpos - lpos == 2) {
        node = new FunctionCallAST(tokens[lpos]);
    }
    else {
        node = new FunctionCallAST(arg_part_parser(lpos + 2, rpos - 1), tokens[lpos]);
    }

    return node;
}

ArrayAST* Parser::array_parser(int lpos, int rpos) {
    ArrayAST* node = NULL;

    // lpos > rpos? error
    if (lpos > rpos) {
        error = 20;
        return node; // NULL
    }

    // notice the ONLY templet is < id ... >
    if (tokens[lpos].type != IDENTIFIERNO) {
        error = 20;
        return node; // NULL
    }

    // choose method
    if (lpos == rpos) {
        node = new ArrayAST(tokens[lpos]);
    }
    else {
        node = new ArrayAST(index_part_parser(lpos + 1, rpos),tokens[lpos]);
    }

    return node;
}

ArgPartAST* Parser::arg_part_parser(int lpos, int rpos) {
    ArgPartAST* node = NULL;

    std::vector<ExprAST*> child;

    int tmp_l = lpos;
    int tmp_r = rpos;

    while (true) {
        // need to count the num of left little/middle brackets
        // we should pass all the things in them when searching ','.
        int lit_brac_num = 0;
        int mid_brac_num = 0;

        // ,'s position
        int comma_pos = -1;

        // tmp_l > tmp_r? break
        if (tmp_l > tmp_r) {
            break;
        }

        // search for first ','
        // notice we should pass all the context in ()[]
        for (int i = tmp_l; i <= tmp_r; i++) {
            // deal with ()[] first because they control lit/mid_brac_num
            if (tokens[i].type == SPECIALSYMBOLNO) {
                if (tokens[i].str == "(") {
                    lit_brac_num++;
                    continue;
                }
                if (tokens[i].str == ")") {
                    lit_brac_num--;
                    continue;
                }
                if (tokens[i].str == "[") {
                    mid_brac_num++;
                    continue;
                }
                if (tokens[i].str == "]") {
                    mid_brac_num--;
                    continue;
                }
            }

            // error: brac not match
            if (lit_brac_num < 0 || mid_brac_num < 0) {
                error = 17;
                return node; // NULL
            }

            // if now we are not in brackets
            // see whether the token is ','
            if (lit_brac_num == 0 && mid_brac_num == 0) {
                if (tokens[i].str == ",") {
                    comma_pos = i;
                    break;
                }
            }
        }

        // choose method
        if (comma_pos == -1) {
            child.push_back(expr_parser(tmp_l, tmp_r));
            tmp_l = tmp_r + 1;
        }
        else {
            child.push_back(expr_parser(tmp_l, comma_pos - 1));
            tmp_l = comma_pos + 1;
        }
    }

    // error case: no child
    if (child.size() == 0) {
        error = 20;
        return node;
    }

    node = new ArgPartAST(child);

    return node;
}

IndexPartAST* Parser::index_part_parser(int lpos, int rpos) {
    IndexPartAST* node = NULL;

    std::vector<ExprAST*> child;

    int tmp_l = lpos;
    int tmp_r = rpos;

    while (true) {
        // need to count the num of left middle brackets
        // notice init is 1 because of tokens[lpos] is ALWAYS '['
        int mid_brac_num = 1;

        // the pair ]'s position
        int brac_pos = -1;

        // notice the least number of tokens is 3, and the first token is '['
        if (tmp_r - tmp_l < 2 || tokens[tmp_l].str != "[") {
            break;
        }

        // search for the pair ']'
        // ignore the first token
        for (int i = tmp_l + 1; i <= tmp_r; i++) {
            if (tokens[i].type == SPECIALSYMBOLNO) {
                if (tokens[i].str == "[") {
                    mid_brac_num++;
                }
                if (tokens[i].str == "]") {
                    mid_brac_num--;
                }
            }

            // see whether the num is 0
            // which means it's the pair ']'
            if (mid_brac_num == 0) {
                brac_pos = i;
                break;
            }
        }

        // no ']'? error;
        if (brac_pos == -1) {
            error = 17;
            return node; // NULL;
        }

        // get a child
        child.push_back(expr_parser(tmp_l + 1, brac_pos - 1));
        tmp_l = brac_pos + 1;
    }

    // error cases: something left or child is empty;
    if (tmp_l <= tmp_r || child.size() == 0) {
        error = 20;
        return node;
    }

    node = new IndexPartAST(child);

    return node;
}

void Parser::skip_comment() {
    for (unsigned int i = 0; i < old_tokens.size(); i++) {
        if (old_tokens[i].type != COMMENTNO) {
            tokens.push_back(old_tokens[i]);
        }
    }
}

AST* Parser::do_parsing(int& _error) {
    AST* root = program_parser(0, tokens.size() - 1);
    _error = error;
    return root;
}
