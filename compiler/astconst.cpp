#include "astconst.h"

std::vector<AST*> AST::getChildren() {
    std::vector<AST*> null;
    switch (type) {
    case 0:
        return ((ProgramAST*)this)->get();
    case 1:
        return ((TypeDefineBlockAST*)this)->get();
    case 2:
        return ((VarDefineBlockAST*)this)->get();
    case 3:
        return ((FunctionDefineBlockAST*)this)->get();
    case 4:
        return ((ProgramExecuteBlockAST*)this)->get();
    case 5:
        return ((TypeDefinePartAST*)this)->get();
    case 6:
        return ((TypeDefineInsAST*)this)->get();
    case 7:
        return ((ClassBlockAST*)this)->get();
    case 8:
        return ((ClassVarPartAST*)this)->get();
    case 9:
        return ((ClassFunctionPartAST*)this)->get();
    case 10:
        return ((VarDefineInsPartAST*)this)->get();
    case 11:
        return ((VarDefineInsAST*)this)->get();
    case 12:
        return ((FunctionDefinePartAST*)this)->get();
    case 13:
        return ((FunctionNameAST*)this)->get();
    case 14:
        return ((FunctionVarDefineBlockAST*)this)->get();
    case 15:
        return ((FunctionExecuteBlockAST*)this)->get();
    case 16:
        return ((ArgDefinePartAST*)this)->get();
    case 17:
        return ((MainExecutePartAST*)this)->get();
    case 18:
        return ((ExecuteInsBlockAST*)this)->get();
    case 19:
        return ((ExecuteInsPartAST*)this)->get();
    case 20:
        return ((ExecuteInsAST*)this)->get();
    case 21:
        return ((EvaluationInsAST*)this)->get();
    case 22:
        return ((LoopInsAST*)this)->get();
    case 23:
        return ((ConditionInsAST*)this)->get();
    case 24:
        return ((ReturnInsAST*)this)->get();
    case 25:
        return ((ForInsAST*)this)->get();
    case 26:
        return ((ForeachInsAST*)this)->get();
    case 27:
        return ((WhileInsAST*)this)->get();
    case 28:
        return ((IfInsAST*)this)->get();
    case 29:
        return ((ExprAST*)this)->get();
    case 30:
        return ((Term1AST*)this)->get();
    case 31:
        return ((Term2AST*)this)->get();
    case 32:
        return ((Term3AST*)this)->get();
    case 33:
        return ((ValueStructAST*)this)->get();
    case 34:
        return ((MemoryStructAST*)this)->get();
    case 35:
        return ((ArrayAST*)this)->get();
    case 36:
        return ((IndexPartAST*)this)->get();
    case 37:
        return ((FunctionCallAST*)this)->get();
    case 38:
        return ((ArgPartAST*)this)->get();
    case 39:
        return ((TokenAST*)this)->get();
    default:
        return null;
    }
}

std::string AST::getValue() {
    if (type == 39)
        return ASTNAME[type] + "[" + ((TokenAST*)this)->getOp().getTypeToString() + ":" + ((TokenAST*)this)->getOp().getValue() + "]";
    return ASTNAME[type];
}

void ProgramAST::print() {
    std::cout << "TypeDefineBlockAST{ ";
    type->print();
    std::cout << "}\n";
    std::cout << "VarDefineBlockAST{ ";
    var->print();
    std::cout << "}\n";
    std::cout << "FunctionDefineBlockAST{ ";
    func->print();
    std::cout << "}\n";
    std::cout << "ProgramExecuteBlockAST{ ";
    main->print();
    std::cout << "}\n";
}

std::vector<AST*> ProgramAST::get() {
    std::vector<AST*> result;
    result.push_back(type);
    result.push_back(var);
    result.push_back(func);
    result.push_back(main);
    return result;
}

void TypeDefineBlockAST::print() {
    switch (type) {
    case 0:
        std::cout << "NULL ";
        break;
    case 1:
        std::cout << "Type ";
        break;
    case 2:
        std::cout << "TypeDefineInsPartAST{ ";
        node->print();
        std::cout << "} ";
        break;
    default:
        break;
    }
}

std::vector<AST*> TypeDefineBlockAST::get() {
    std::vector<AST*> result;
    switch (type) {
    case 0: case 1:
        break;
    case 2:
        result.push_back(node);
        break;
    default:
        break;
    }
    return result;
}

void VarDefineBlockAST::print() {
    switch (type) {
    case 0:
        std::cout << "NULL ";
        break;
    case 1:
        std::cout << "Var ";
        break;
    case 2:
        std::cout << "VarDefineInsPartAST{ ";
        node->print();
        std::cout << "} ";
        break;
    default:
        break;
    }
}

std::vector<AST*> VarDefineBlockAST::get() {
    std::vector<AST*> result;
    switch (type) {
    case 0: case 1:
        break;
    case 2:
        result.push_back(node);
        break;
    default:
        break;
    }
    return result;
}

void FunctionDefineBlockAST::print() {
    for (unsigned int i = 0; i < nodes.size(); i++) {
        std::cout << "FunctionDefinePartAST{ ";
        nodes[i]->print();
        std::cout << "} ";
        std::cout << "; ";
    }
    if (nodes.size() == 0) {
        std::cout << "NULL ";
    }
}

std::vector<AST*> FunctionDefineBlockAST::get() {
    std::vector<AST*> result;
    for (unsigned int i = 0; i < nodes.size(); i++) {
        result.push_back(nodes[i]);
    }
    return result;
}

void ProgramExecuteBlockAST::print() {
    std::cout << "ExecuteInsBlockAST{ ";
    node->print();
    std::cout << "} . ";
}

std::vector<AST*> ProgramExecuteBlockAST::get() {
    std::vector<AST*> result;
    result.push_back(node);
    return result;
}

void TypeDefinePartAST::print() {
    for (unsigned int i = 0; i < nodes.size(); i++) {
        std::cout << "TypeDefineInsAST{ ";
        nodes[i]->print();
        std::cout << "} ";
        std::cout << "; ";
    }
}

std::vector<AST*> TypeDefinePartAST::get() {
    std::vector<AST*> result;
    for (unsigned int i = 0; i < nodes.size(); i++) {
        result.push_back(nodes[i]);
    }
    return result;
}

void TypeDefineInsAST::print() {
    std::cout << id->getOp().type << "\\" << id->getOp().str << "\\ = ";
    std::cout << type->getOp().type << "\\" << type->getOp().str << "\\ ";
    if (node != NULL) {
        std::cout << "ClassBlockAST{ ";
        node->print();
        std::cout << "} ";
    }
}

std::vector<AST*> TypeDefineInsAST::get() {
    std::vector<AST*> result;
    result.push_back(id);
    result.push_back(type);
    if (node != NULL) {
        result.push_back(node);
    }
    return result;
}

void ClassBlockAST::print() {
    std::cout << "begin ClassVarPartAST{ ";
    left->print();
    std::cout << "} ";
    std::cout << "ClassFunctionPartAST{ ";
    right->print();
    std::cout << "} end ";
}

std::vector<AST*> ClassBlockAST::get() {
    std::vector<AST*> result;
    result.push_back(left);
    result.push_back(right);
    return result;
}

void ClassVarPartAST::print() {
    if (node != NULL) {
        std::cout << "VarDefineInsPartAST{ ";
        node->print();
        std::cout << "} ";
    }
    else {
        std::cout << "NULL ";
    }
}

std::vector<AST*> ClassVarPartAST::get() {
    std::vector<AST*> result;
    if (node != NULL) {
        result.push_back(node);
    }
    return result;
}

void ClassFunctionPartAST::print() {
    std::cout << "FunctionDefineBlockAST{ ";
    node->print();
    std::cout << "} ";
}

std::vector<AST*> ClassFunctionPartAST::get() {
    std::vector<AST*> result;
    result.push_back(node);
    return result;
}

void VarDefineInsPartAST::print() {
    for (unsigned int i = 0; i < nodes.size(); i++) {
        std::cout << "VarDefineInsAST{ ";
        nodes[i]->print();
        std::cout << "} ";
        std::cout << "; ";
    }
}

std::vector<AST*> VarDefineInsPartAST::get() {
    std::vector<AST*> result;
    for (unsigned int i = 0; i < nodes.size(); i++) {
        result.push_back(nodes[i]);
    }
    return result;
}

void VarDefineInsAST::print() {
    std::cout << "ArrayAST{ ";
    node->print();
    std::cout << "} : ";
    std::cout << type->getOp().type << "\\" << type->getOp().str << "\\ ";
}

std::vector<AST*> VarDefineInsAST::get() {
    std::vector<AST*> result;
    result.push_back(node);
    result.push_back(type);
    return result;
}

void FunctionDefinePartAST::print() {
    std::cout << "FunctionNameAST{ ";
    name->print();
    std::cout << "} ";
    std::cout << "FunctionVarDefineAST{ ";
    var->print();
    std::cout << "} ";
    std::cout << "FunctionExecuteBlockAST{ ";
    main->print();
    std::cout << "} ";
}

std::vector<AST*> FunctionDefinePartAST::get() {
    std::vector<AST*> result;
    result.push_back(name);
    result.push_back(var);
    result.push_back(main);
    return result;
}

void FunctionNameAST::print() {
    std::cout << "function " << name->getOp().type << "\\" << name->getOp().str << "\\ ( ";
    std::cout << "ArgDefinePartAST{ ";
    node->print();
    std::cout << "} ) ";
    if (type != NULL) {
        std::cout << ": " << type->getOp().type << "\\" << type->getOp().str << "\\ ";
    }
}

std::vector<AST*> FunctionNameAST::get() {
    std::vector<AST*> result;
    result.push_back(name);
    result.push_back(node);
    if (type != NULL) {
        result.push_back(type);
    }
    return result;
}

void FunctionVarDefineBlockAST::print() {
    std::cout << "VarDefineBlockAST{ ";
    node->print();
    std::cout << "} ";
}

std::vector<AST*> FunctionVarDefineBlockAST::get() {
    std::vector<AST*> result;
    result.push_back(node);
    return result;
}

void FunctionExecuteBlockAST::print() {
    std::cout << "ExecuteInsBlockAST{ ";
    node->print();
    std::cout << "} ";
}

std::vector<AST*> FunctionExecuteBlockAST::get() {
    std::vector<AST*> result;
    result.push_back(node);
    return result;
}

void ArgDefinePartAST::print() {
    if (nodes.size() == 0) {
        std::cout << "NULL ";
        return ;
    }
    for (unsigned int i = 0; i < nodes.size() - 1; i++) {
        std::cout << "VarDefineInsAST{ ";
        nodes[i]->print();
        std::cout << "} ";
        std::cout << ", ";
    }
    std::cout << "VarDefineInsAST{ ";
    nodes[nodes.size() - 1]->print();
    std::cout << "} ";
}

std::vector<AST*> ArgDefinePartAST::get() {
    std::vector<AST*> result;
    for (unsigned int i = 0; i < nodes.size(); i++) {
        result.push_back(nodes[i]);
    }
    return result;
}

void ExecuteInsBlockAST::print() {
    if (node != NULL) {
        std::cout << "begin ";
        std::cout << "ExecuteInsPartAST{ ";
        node->print();
        std::cout << "} end ";
    }
    else {
        std::cout << "begin " << "end ";
    }
}

std::vector<AST*> ExecuteInsBlockAST::get() {
    std::vector<AST*> result;
    if (node != NULL) {
        result.push_back(node);
    }
    return result;
}

void ExecuteInsPartAST::print() {
    for (unsigned int i = 0; i < nodes.size(); i++) {
        std::cout << "ExecuteInsAST{ ";
        nodes[i]->print();
        std::cout << "} ";
        std::cout << "; ";
    }
}

std::vector<AST*> ExecuteInsPartAST::get() {
    std::vector<AST*> result;
    for (unsigned int i = 0; i < nodes.size(); i++) {
        result.push_back(nodes[i]);
    }
    return result;
}

void EvaluationInsAST::print() {
    if (left == NULL) {
        std::cout << "ExprAST{ ";
        right->print();
        std::cout << "} ";
    }
    else {
        std::cout << "MemoryStructAST{ ";
        left->print();
        std::cout << "} = " << "ExprAST{ ";
        right->print();
        std::cout << "} ";
    }
}

std::vector<AST*> EvaluationInsAST::get() {
    std::vector<AST*> result;
    if (left == NULL) {
        result.push_back(right);
    }
    else {
        result.push_back(left);
        result.push_back(right);
    }
    return result;
}

void ForInsAST::print() {
    std::cout << "for MemoryStructAST{ ";
    var->print();
    std::cout << "} = ExprAST{ ";
    left->print();
    std::cout << "} " << op.str << " ExprAST{ ";
    right->print();
    std::cout << "} do MainExecutePartAST{ ";
    main->print();
    std::cout << "} ";
}

std::vector<AST*> ForInsAST::get() {
    std::vector<AST*> result;
    result.push_back(var);
    result.push_back(left);
    result.push_back(right);
    result.push_back(main);
    return result;
}

void ForeachInsAST::print() {
    std::cout << "foreach MemoryStructAST{ ";
    var->print();
    std::cout << "} in MemoryStructAST{ ";
    arr->print();
    std::cout << "} do MainExecutePartAST{ ";
    main->print();
    std::cout << "} ";
}

std::vector<AST*> ForeachInsAST::get() {
    std::vector<AST*> result;
    result.push_back(var);
    result.push_back(arr);
    result.push_back(main);
    return result;
}

void WhileInsAST::print() {
    std::cout << "while ExprAST{ ";
    node->print();
    std::cout << "} do MainExecutePartAST{ ";
    main->print();
    std::cout << "} ";
}

std::vector<AST*> WhileInsAST::get() {
    std::vector<AST*> result;
    result.push_back(node);
    result.push_back(main);
    return result;
}

void LoopInsAST::print() {
    switch (type) {
    case 0:
        std::cout << "ForInsAST{ ";
        ((ForInsAST*)node)->print();
        std::cout << "} ";
        break;
    case 1:
        std::cout << "ForeachInsAST{ ";
        ((ForeachInsAST*)node)->print();
        std::cout << "} ";
        break;
    case 2:
        std::cout << "WhileInsAST{ ";
        ((WhileInsAST*)node)->print();
        std::cout << "} ";
        break;
    default:
        break;
    }
}

std::vector<AST*> LoopInsAST::get() {
    std::vector<AST*> result;
    result.push_back(node);
    return result;
}

void IfInsAST::print() {
    std::cout << "If ExprAST{ ";
    node->print();
    std::cout << "} Then ";
    std::cout << "MainInsPartAST{ ";
    then_main->print();
    std::cout << "} ";
    if (else_main != NULL) {
        std::cout << " Else MainInsPartAST{ ";
        else_main->print();
        std::cout << "} ";
    }
}

std::vector<AST*> IfInsAST::get() {
    std::vector<AST*> result;
    result.push_back(node);
    result.push_back(then_main);
    if (else_main != NULL) {
        result.push_back(else_main);
    }
    return result;
}

void ConditionInsAST::print() {
    std::cout << "ConditionInsAST{ ";
    ((IfInsAST*)node)->print();
    std::cout << "} ";
}

std::vector<AST*> ConditionInsAST::get() {
    std::vector<AST*> result;
    result.push_back(node);
    return result;
}

void ReturnInsAST::print() {
    std::cout << "return ";
    if (node != NULL) {
        std::cout << "ExprAST{ ";
        node->print();
        std::cout << "} ";
    }
}

std::vector<AST*> ReturnInsAST::get() {
    std::vector<AST*> result;
    if (node != NULL) {
        result.push_back(node);
    }
    return result;
}

void ExecuteInsAST::print() {
    switch (type) {
    case 0:
        std::cout << "EvaluationInsAST{ ";
        ((EvaluationInsAST*)node)->print();
        std::cout << "} ";
        break;
    case 1:
        std::cout << "LoopInsAST{ ";
        ((LoopInsAST*)node)->print();
        std::cout << "} ";
        break;
    case 2:
        std::cout << "ConditionInsAST{ ";
        ((ConditionInsAST*)node)->print();
        std::cout << "} ";
        break;
    case 3:
        std::cout << "ReturnInsAST{ ";
        ((ReturnInsAST*)node)->print();
        std::cout << "} ";
        break;
    default:
        break;
    }
}

std::vector<AST*> ExecuteInsAST::get() {
    std::vector<AST*> result;
    result.push_back(node);
    return result;
}

void MainExecutePartAST::print() {
    switch (type) {
    case 0:
        std::cout << "NULL ";
        break;
    case 1:
        std::cout << "ExecuteInsAST{ ";
        ((ExecuteInsAST*)node)->print();
        std::cout << "} ";
        break;
    case 2:
        std::cout << "ExecuteInsBlockAST{ ";
        ((ExecuteInsBlockAST*)node)->print();
        std::cout << "} ";
        break;
    default:
        break;
    }
}

std::vector<AST*> MainExecutePartAST::get() {
    std::vector<AST*> result;
    switch (type) {
    case 0:
        break;
    case 1: case 2:
        result.push_back(node);
        break;
    default:
        break;
    }
    return result;
}

void ExprAST::print() {
    if (op == NULL) {
        std::cout << " Term1AST{ ";
        right->print();
        std::cout << "} ";
    }
    else {
        std::cout << "ExprAST{ ";
        left->print();
        std::cout << "} " << op->getOp().type << "\\" << op->getOp().str << "\\" << " Term1AST{ ";
        right->print();
        std::cout << "} ";
    }
}

std::vector<AST*> ExprAST::get() {
    std::vector<AST*> result;
    if (op == NULL) {
        result.push_back(right);
    }
    else {
        result.push_back(left);
        result.push_back(op);
        result.push_back(right);
    }

    return result;
}

void Term1AST::print() {
    if (op == NULL) {
        std::cout << "Term2AST{ ";
        right->print();
        std::cout << "} ";
    }
    else {
        std::cout << "Term1AST{ ";
        left->print();
        std::cout << "} " << op->getOp().type << "\\" << op->getOp().str << "\\" << " Term2AST{ ";
        right->print();
        std::cout << "} ";
    }
}

std::vector<AST*> Term1AST::get() {
    std::vector<AST*> result;
    if (op == NULL) {
        result.push_back(right);
    }
    else {
        result.push_back(left);
        result.push_back(op);
        result.push_back(right);
    }
    return result;
}

void Term2AST::print() {
    if (op == NULL) {
        std::cout << "Term3AST{ ";
        right->print();
        std::cout << "} ";
    }
    else {
        std::cout << "Term2AST{ ";
        left->print();
        std::cout << "} " << op->getOp().type << "\\" << op->getOp().str << "\\" << " Term3AST{ ";
        right->print();
        std::cout << "} ";
    }
}

std::vector<AST*> Term2AST::get() {
    std::vector<AST*> result;
    if (op == NULL) {
        result.push_back(right);
    }
    else {
        result.push_back(left);
        result.push_back(op);
        result.push_back(right);
    }
    return result;
}

void ValueStructAST::print() {
    if (node != NULL) {
        std::cout << "MemoryStructAST{ ";
        node->print();
        std::cout << "} ";
    }
    else {
        std::cout << value->getOp().type << "\\" << value->getOp().str << "\\ ";
    }
}

std::vector<AST*> ValueStructAST::get() {
    std::vector<AST*> result;
    if (node != NULL) {
        result.push_back(node);
    }
    else {
        result.push_back(value);
    }
    return result;
}

void Term3AST::print() {
    if (op1 == NULL) {
        std::cout << "ValueStructAST{ ";
        ((ValueStructAST*)node)->print();
        std::cout << "} ";
    }
    else if (op2 == NULL){
        std::cout << op1->getOp().type << "\\ " << op1->getOp().str << " \\ ";
        std::cout << "Term3AST{ ";
        ((Term3AST*)node)->print();
        std::cout << "} ";
    }
    else {
        std::cout << op1->getOp().type << "\\ " << op1->getOp().str << " \\ ";
        std::cout << "ExprAST{ ";
        ((ExprAST*)node)->print();
        std::cout << "} ";
        std::cout << op2->getOp().type << "\\ " << op2->getOp().str << " \\ ";
    }
}

std::vector<AST*> Term3AST::get() {
    std::vector<AST*> result;
    if (op1 == NULL) {
        result.push_back(node);
    }
    else if (op2 == NULL){
        result.push_back(op1);
        result.push_back(node);
    }
    else {
        result.push_back(op1);
        result.push_back(node);
        result.push_back(op2);
    }

    return result;
}

void ArrayAST::print() {
    if (node != NULL) {
        std::cout << id->getOp().type << "\\ " << id->getOp().str << " \\ ";
        std::cout << "IndexPartAST{ ";
        node->print();
        std::cout << "} ";
    }
    else {
        std::cout << id->getOp().type << "\\ " << id->getOp().str << " \\ ";
    }
}

std::vector<AST*> ArrayAST::get() {
    std::vector<AST*> result;
    if (node != NULL) {
        result.push_back(id);
        result.push_back(node);
    }
    else {
        result.push_back(id);
    }
    return result;
}

void FunctionCallAST::print() {
    if (node != NULL) {
        std::cout << id->getOp().type << "\\ " << id->getOp().str << " \\ ( ";
        std::cout << "IndexPartAST{ ";
        node->print();
        std::cout << "} ";
        std::cout << ") ";
    }
    else {
        std::cout << id->getOp().type << "\\ " << id->getOp().str << " \\ " << "( ";
        std::cout << ") ";
    }
}

std::vector<AST*> FunctionCallAST::get() {
    std::vector<AST*> result;
    if (node != NULL) {
        result.push_back(id);
        result.push_back(node);
    }
    else {
        result.push_back(id);
    }
    return result;
}

void MemoryStructAST::print() {
    if (left != NULL) {
        std::cout << "MemoryStructAST{ ";
        left->print();
        std::cout << "} ";
        std::cout << ". ";
        if (type == 0) {
            std::cout << "FunctionCallAST{ ";
            ((FunctionCallAST*)right)->print();
            std::cout << "} ";
        }
        else {
            std::cout << "ArrayAST{ ";
            ((ArrayAST*)right)->print();
            std::cout << "} ";
        }
    }
    else {
        if (type == 2) {
            std::cout << "FunctionCallAST{ ";
            ((FunctionCallAST*)right)->print();
            std::cout << "} ";
        }
        else {
            std::cout << "ArrayAST{ ";
            ((ArrayAST*)right)->print();
            std::cout << "} ";
        }
    }
}

std::vector<AST*> MemoryStructAST::get() {
    std::vector<AST*> result;
    if (left != NULL) {
        result.push_back(left);
        result.push_back(right);
    }
    else {
        result.push_back(right);
    }
    return result;
}

void IndexPartAST::print() {
    for (unsigned int i = 0; i < nodes.size(); i++) {
        std::cout << "[ ";
        std::cout << "ExprAST{ ";
        nodes[i]->print();
        std::cout << "} ";
        std::cout << "] ";
    }
}

std::vector<AST*> IndexPartAST::get() {
    std::vector<AST*> result;
    for (unsigned int i = 0; i < nodes.size(); i++) {
        result.push_back(nodes[i]);
    }
    return result;
}

void ArgPartAST::print() {
    for (unsigned int i = 0; i < nodes.size() - 1; i++) {
        std::cout << "ExprAST{ ";
        nodes[i]->print();
        std::cout << "} ";
        std::cout << ", ";
    }
    std::cout << "ExprAST{ ";
    nodes[nodes.size() - 1]->print();
    std::cout << "} ";
}

std::vector<AST*> ArgPartAST::get() {
    std::vector<AST*> result;
    for (unsigned int i = 0; i < nodes.size(); i++) {
        result.push_back(nodes[i]);
    }
    return result;
}
