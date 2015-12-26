#include "analysis.h"

#include <iostream>

/*
// [TODO] args should be changed to index
*/
bool Analysis::legalEvaluation(std::string memoryOrigin, std::string exprOrigin, int& warning) {
    if (memoryOrigin == "integer") {
        if (exprOrigin == "integer" || exprOrigin == "boolean" || exprOrigin == "char") {
            return true; // no data lost, safe
        }
        else if (exprOrigin == "real") {
            warning = 0; // data lost, warning(0): real to integer
            return true;
        }
        else {
            return false; // error: cannot transfor
        }
    }
    else if (memoryOrigin == "real") {
        if (exprOrigin == "integer" || exprOrigin == "real" || exprOrigin == "boolean" || exprOrigin == "char") {
            return true; // no data lost, safe
        }
        else {
            return false; // error: cannot transfor
        }
    }
    else if (memoryOrigin == "boolean") {
        if (exprOrigin == "boolean") {
            return true; // no data lost, safe
        }
        else if (exprOrigin == "integer") {
            warning = 1; // data lost, warning(1): boolean to integer
            return true;
        }
        else {
            return false; // error: cannot transfor
        }
    }
    else if (memoryOrigin == "string") {
        if (exprOrigin == "string" || exprOrigin == "char") {
            return true; // no data lost, safe
        }
        else {
            return false; // error: cannot transfor
        }
    }
    else if (memoryOrigin == "char") {
        if (exprOrigin == "char") {
            return true; // no data lost, safe
        }
        else if (exprOrigin == "integer") {
            warning = 2; // data lost, warning(2): integer to char
            return true;
        }
        else {
            return false; // error: cannot transfor
        }
    }
    else if (memoryOrigin == "class") {
        if (exprOrigin == "class") {
            // warning: is it really safe;
            /*
            // [TODO] notice class cannot use '=' unless:
            // 1. the same type (typeName = typeName)
            // [2]. MType is the father of EType
            // [3]. EType is the father of MType, will cause warning
            // notice 2,3 is optional because we can define that
            // we don't support transfor between parent and children
            */
            return true; // no data lost, safe
        }
        else {
            return false; // error: cannot transfor
        }
    }

    // other case?
    return false; // secret error...
}

// -1 for not found
int Analysis::getIndexFromTDT(std::string name) {
    for (unsigned int i = 0; i < TDT.typeDescriptors.size(); i++) {
        if (name == TDT.typeDescriptors[i].name) {
            return i;
        }
    }
    return -1;
}

// -1 for not found
int Analysis::getIndexFromVFRegion(std::string name, VarFuncRegion region) {
    for (unsigned int i = 0; i < region.varFunc_defs.size(); i++) {
        if (name == region.varFunc_defs[i].def.name) {
            return i;
        }
    }
    return -1;
}

std::string Analysis::getTransType(std::string str) {
    if (str == "integer") return "int";
    if (str == "real") return "float";
    if (str == "boolean") return "bool";
    if (str == "class") return "struct";
    if (str == "char") return "char";
    if (str == "string") return "string";
    return str; // for id or error type
}

std::string Analysis::getTransOp(std::string str) {
    /*
     * [TODO] bit-op not supporting for integer
     */
    if (str == "/") return "*1.0/";
    if (str == "div") return "/";
    if (str == "mod") return "%";
    if (str == "and") return "&&";
    if (str == "or") return "||";
    if (str == "xor") return "^";
    if (str == "not") return "!";
    // for ==/!=/>/</>=/<=/+/-/* or error
    return str;
}

VarFuncRegion Analysis::loadLibFunc(VarFuncRegion r) {
    VarFuncRegion region = r;

    Vardef v_def;
    v_def.name = "readInteger";
    v_def.parent = "void";
    v_def.origin = "void";
    v_def.maxIndexLevel = 0;

    Vardef arg;
    arg.name = "i";
    arg.parent = "integer";
    arg.origin = "integer";
    arg.maxIndexLevel = 0;

    std::vector<Vardef> funcArg;
    funcArg.push_back(arg);

    VarFuncdef vf_def;
    vf_def.def = v_def;
    vf_def.attribute = "Function";
    vf_def.funcArg = funcArg;

    VarFuncDescriptor desc;
    desc.name = v_def.name;
    desc.parent = v_def.parent;
    desc.origin = v_def.origin;
    desc.attribute = vf_def.attribute;
    desc.env = "Lib";

    // for all read
    // for readInteger
    code += "void readInteger(int& i){\n";
    code += "cin >> i;\n";
    code += "}\n";
    region.varFunc_defs.push_back(vf_def);
    VFDT.varFuncDescriptors.push_back(desc);

    // for readReal;
    code += "void readReal(float& i){\n";
    code += "cin >> i;\n";
    code += "}\n";
    vf_def.def.name = "readReal";
    vf_def.funcArg[0].parent = "real";
    vf_def.funcArg[0].origin = "real";
    region.varFunc_defs.push_back(vf_def);
    desc.name = vf_def.def.name;
    VFDT.varFuncDescriptors.push_back(desc);

    // for readString
    code += "void readString(string& i){\n";
    code += "cin >> i;\n";
    code += "}\n";
    vf_def.def.name = "readString";
    vf_def.funcArg[0].parent = "string";
    vf_def.funcArg[0].origin = "string";
    region.varFunc_defs.push_back(vf_def);
    desc.name = vf_def.def.name;
    VFDT.varFuncDescriptors.push_back(desc);

    // for readChar
    code += "void readChar(char& i){\n";
    code += "cin >> i;\n";
    code += "}\n";
    vf_def.def.name = "readChar";
    vf_def.funcArg[0].parent = "char";
    vf_def.funcArg[0].origin = "char";
    region.varFunc_defs.push_back(vf_def);
    desc.name = vf_def.def.name;
    VFDT.varFuncDescriptors.push_back(desc);

    // for write
    // for writeInteger
    code += "void writeInteger(int i){\n";
    code += "cout << i;\n";
    code += "}\n";
    vf_def.def.name = "writeInteger";
    vf_def.funcArg[0].parent = "integer";
    vf_def.funcArg[0].origin = "integer";
    region.varFunc_defs.push_back(vf_def);
    desc.name = vf_def.def.name;
    VFDT.varFuncDescriptors.push_back(desc);

    // for writeReal
    code += "void writeReal(float i){\n";
    code += "cout << i;\n";
    code += "}\n";
    vf_def.def.name = "writeReal";
    vf_def.funcArg[0].parent = "real";
    vf_def.funcArg[0].origin = "real";
    region.varFunc_defs.push_back(vf_def);
    desc.name = vf_def.def.name;
    VFDT.varFuncDescriptors.push_back(desc);

    // for writeString
    code += "void writeString(string i){\n";
    code += "cout << i;\n";
    code += "}\n";
    vf_def.def.name = "writeString";
    vf_def.funcArg[0].parent = "string";
    vf_def.funcArg[0].origin = "string";
    region.varFunc_defs.push_back(vf_def);
    desc.name = vf_def.def.name;
    VFDT.varFuncDescriptors.push_back(desc);

    // for writeChar
    code += "void writeChar(char i){\n";
    code += "cout << i;\n";
    code += "}\n";
    vf_def.def.name = "writeChar";
    vf_def.funcArg[0].parent = "char";
    vf_def.funcArg[0].origin = "char";
    region.varFunc_defs.push_back(vf_def);
    desc.name = vf_def.def.name;
    VFDT.varFuncDescriptors.push_back(desc);

    // for writeln
    // for writelnInteger
    code += "void writelnInteger(int i){\n";
    code += "cout << i << endl;\n";
    code += "}\n";
    vf_def.def.name = "writelnInteger";
    vf_def.funcArg[0].parent = "integer";
    vf_def.funcArg[0].origin = "integer";
    region.varFunc_defs.push_back(vf_def);
    desc.name = vf_def.def.name;
    VFDT.varFuncDescriptors.push_back(desc);

    // for writelnReal
    code += "void writelnReal(float i){\n";
    code += "cout << i << endl;\n";
    code += "}\n";
    vf_def.def.name = "writelnReal";
    vf_def.funcArg[0].parent = "real";
    vf_def.funcArg[0].origin = "real";
    region.varFunc_defs.push_back(vf_def);
    desc.name = vf_def.def.name;
    VFDT.varFuncDescriptors.push_back(desc);

    // for writelnString
    code += "void writelnString(string i){\n";
    code += "cout << i << endl;\n";
    code += "}\n";
    vf_def.def.name = "writelnString";
    vf_def.funcArg[0].parent = "string";
    vf_def.funcArg[0].origin = "string";
    region.varFunc_defs.push_back(vf_def);
    desc.name = vf_def.def.name;
    VFDT.varFuncDescriptors.push_back(desc);

    // for writelnChar
    code += "void writelnChar(char i){\n";
    code += "cout << i << endl;\n";
    code += "}\n";
    vf_def.def.name = "writelnChar";
    vf_def.funcArg[0].parent = "char";
    vf_def.funcArg[0].origin = "char";
    region.varFunc_defs.push_back(vf_def);
    desc.name = vf_def.def.name;
    VFDT.varFuncDescriptors.push_back(desc);

    return region;
}

void Analysis::createAndCheckDT(AST* node) {
    std::vector<AST*> children = node->getChildren();
    int ASTtype = node->getTypeNum();
    // std::cout << node->getValue() << '\n';

    /*RULE:
     * For Var and Func
     * 1. In same region, ids for var or func *CAN NOT BE THE SAME* with each other.
     * 2. In different region, ids *CAN BE THE SAME*.
     * 3. If id is both in the r1 and r2, we can *ONLY USE TYPR IN R2*.
     * For Type
     * 1. Type name *CANNOT BE THE SAME*.
     * 2. Type origin can *ONLY BE RT*.
    */

    // now do things for different AST
    switch (ASTtype) {
    case 0: // ProgramAST
        // necessary include
        code += "#include <iostream>\n";
        code += "#include <string>\n";
        code += "#include <stdio.h>\n\n";
        code += "using namespace std;\n\n";

        node->setR1Env("Global"); // set Env
        // TypeDefineBlockAST
        createAndCheckDT(children[0]);
        if (error > 0) { return; }

        // VarDefineBlockAST
        children[1]->setR1(node->getR1()); // set r1 to Var
        createAndCheckDT(children[1]);
        if (error > 0) { return; }
        node->setR1(children[1]->getR1()); // get r1 from Var

        // Lib Function
        node->setR1(loadLibFunc(node->getR1())); // load lib function

        // FunctionDefineBlockAST
        children[2]->setR1(node->getR1()); // set r1 to Func
        createAndCheckDT(children[2]);
        if (error > 0) { return; }
        node->setR1(children[2]->getR1()); // get r1 from Func

        // add code
        code += "int main(){\n";
        // ProgramExecuteBlockAST
        children[3]->setR1(node->getR1()); // set r1 to Execute
        createAndCheckDT(children[3]);
        if (error > 0) { return; }
        // add code
        code += "}\n";

        break;
    case 1: // TypeDefineBlockAST
        if (children.size() != 0) {
            createAndCheckDT(children[0]);
            if (error > 0) { return; }
        }
        break;
    case 2: // VarDefineBlockAST
        if (children.size() != 0) {
            children[0]->setR1(node->getR1()); // set r1 to Var
            createAndCheckDT(children[0]);
            if (error > 0) { return; }
            node->setR1(children[0]->getR1()); // get r1 from Var
        }
        break;
    case 3: // FunctionDefineBlockAST
        for (unsigned int i = 0; i < children.size(); i++) {
            children[i]->setR1(node->getR1()); // set r1 to Var
            createAndCheckDT(children[i]);
            if (error > 0) { return; }
            node->setR1(children[i]->getR1()); // get r1 from Var
        }
        break;
    case 4: // ProgramExecuteBlockAST
        children[0]->setR1(node->getR1()); // set r1 to Execute
        createAndCheckDT(children[0]);
        if (error > 0) { return; }
        break;
    case 5: // TypeDefinePartAST
        for (unsigned int i = 0; i < children.size(); i++) {
            createAndCheckDT(children[i]);
            if (error > 0) { return; }
        }
        break;
    case 6: { // TypeDefineInsAST
        Typedef def;
        def.name = ((TokenAST*)children[0])->getOp().str;
        def.type = ((TokenAST*)children[1])->getOp().str;

        // error: same
        if (def.name == def.type) {
            error = 21;
            return;
        }

        TypeDescriptor desc;
        desc.name = def.name;
        desc.parent = def.type;
        desc.origin = def.type; // for RESERVEDTYPE

        int index;
        // search the origin type in TDT for IDENTIFIER
        if (((TokenAST*)children[1])->getOp().type == IDENTIFIERNO) {
            index = getIndexFromTDT(desc.origin);

            // error: not found
            if (index == -1) {
                error = 22;
                return;
            }
            else {
                desc.origin = TDT.typeDescriptors[index].origin;
            }
        }

        // get region for class
        if (desc.origin == "class") {
            // add code part1
            code += "class " + desc.name;
            if (desc.parent != "class") {
                code += ":public " + desc.parent;
            }
            code += "{\n";
            code += "public:\n";

            def.region.envName = "Class " + def.name;
            children[2]->setR1(def.region);
            createAndCheckDT(children[2]);
            if (error > 0) { return; }
            def.region = children[2]->getR1();

            // add code part2
            code += "};\n";

            // if the class type has parent, add it.
            if (index != -1) {
                VarFuncRegion region = typeRegion.typedefs[index].region;
                for (unsigned int i = 0; i < region.varFunc_defs.size(); i++) {
                    // need to deal with same name;
                    for (unsigned int j = 0; j < def.region.varFunc_defs.size(); j++) {
                        if (def.region.varFunc_defs[j].def.name == region.varFunc_defs[i].def.name) {
                            error = 23;
                            return;
                        }
                    }
                    def.region.varFunc_defs.push_back(region.varFunc_defs[i]);
                }
            }
        }
        else {
            // add code
            code += "typedef " + getTransType(def.type) + " " + def.name + ";\n";;
        }

        // need to deal with same name;
        index = getIndexFromTDT(desc.name);
        if (index != -1) {
            error = 21;
            return;
        }

        // renew TDT and typeRegion
        typeRegion.typedefs.push_back(def);
        TDT.typeDescriptors.push_back(desc);

        break;
    }
    case 7: // ClassBlockAST
        // ClassVarPartAST
        children[0]->setR1(node->getR1()); // set r1 to Var
        createAndCheckDT(children[0]);
        if (error > 0) { return; }
        node->setR1(children[0]->getR1()); // get r1 from Var

        // ClassFunctionPartAST
        children[1]->setR1(node->getR1()); // set r1 to Func
        createAndCheckDT(children[1]);
        if (error > 0) { return; }
        node->setR1(children[1]->getR1()); // get r1 from Func

        break;
    case 8: // ClassVarPartAST
        // VarDefineInsPartAST
        if (children.size() != 0) {
            children[0]->setR1(node->getR1()); // set r1 to Var
            createAndCheckDT(children[0]);
            if (error > 0) { return; }
            node->setR1(children[0]->getR1()); // get r1 from Var
        }
        break;
    case 9: // ClassFunctionPartAST
        children[0]->setR1(node->getR1()); // set r1 to Func;
        createAndCheckDT(children[0]);
        if (error > 0) { return; }
        node->setR1(children[0]->getR1()); // get r1 from Func;
        break;
    case 10: { // VarDefineInsPartAST
        for (unsigned int i = 0; i < children.size(); i++) {
            createAndCheckDT(children[i]);
            if (error > 0) { return; }

            // need to deal with same name;
            int index = getIndexFromVFRegion((children[i]->getR1()).varFunc_defs[0].def.name, node->getR1());
            if (index != -1) {
                error = 24;
                return;
            }

            node->addR1(children[i]->getR1()); // get r1 from Var
            // add code
            code += ";\n";
        }

        // deal with VFDT
        VarFuncRegion region = node->getR1();
        for (unsigned int i = 0; i < region.varFunc_defs.size(); i++) {
            VarFuncDescriptor desc;
            VarFuncdef vf_def = region.varFunc_defs[i];
            Vardef v_def = vf_def.def;
            desc.name = v_def.name;
            desc.parent = v_def.parent;
            desc.origin = v_def.origin;
            desc.attribute = vf_def.attribute;
            desc.env = region.envName;

            // renew VFDT
            VFDT.varFuncDescriptors.push_back(desc);
        }

        break;
    }
    case 11: { // VarDefineInsAST
        // add code
        code += getTransType(((TokenAST*)children[1])->getOp().str) + " ";

        ((ArrayAST*)children[0])->changeDefined(); // set to true
        createAndCheckDT(children[0]); // ArrayAST
        if (error > 0) { return; }

        Vardef v_def;
        v_def.name = ((ArrayAST*)children[0])->getIdName();
        v_def.parent = ((TokenAST*)children[1])->getOp().str;
        v_def.origin = v_def.parent; // For RESERVEDTYPE
        v_def.maxIndexLevel = ((ArrayAST*)children[0])->getMaxIndexLevel();

        // get origin from TDT for IDENTIFIER
        int index = -1;
        if (!isReservedType(v_def.origin)) {
            index = getIndexFromTDT(v_def.origin);

            // error: not found
            if (index == -1) {
                error = 22;
                return;
            }
            else {
               v_def.origin = TDT.typeDescriptors[index].origin;
            }
        }

        VarFuncdef vf_def;
        vf_def.def = v_def;
        if (v_def.maxIndexLevel > 0) {
            vf_def.attribute = "Array";
        }
        else {
            vf_def.attribute = "Single";
        }

        // add region
        node->addR1(vf_def);
        break;
    }

    case 12: // FunctionDefinePartAST
        // FunctionNameAST
        children[0]->setR1(node->getR1()); // set r1 to Name
        createAndCheckDT(children[0]);
        if (error > 0) { return; }
        node->setR1(children[0]->getR1()); // get r1 from Name
        node->setR2(children[0]->getR2()); // get r2 with env from Name
        // add code
        code += "{\n";

        // FunctionVarDefineBlockAST
        children[1]->setR1(node->getR2()); // set r2 to Var's r1
        createAndCheckDT(children[1]);
        if (error > 0) { return; }
        node->setR2(children[1]->getR1()); // get r2 from Var's r1

        // FunctionExecuteBlockAST
        children[2]->setR1(node->getR1()); // set r1 to Execute
        children[2]->setR2(node->getR2()); // set r2 to Execute
        createAndCheckDT(children[2]);
        if (error > 0) { return; }
        // add code
        code += "}\n";

        break;
    case 13: { // FunctionNameAST
        Vardef v_def;
        v_def.name = ((TokenAST*)children[0])->getOp().str;
        if (children.size() == 3) {
            v_def.parent = ((TokenAST*)children[2])->getOp().str;
            v_def.origin = v_def.parent; // For RESERVEDTYPE

            // get origin from TDT for IDENTIFIER
            int index = -1;
            if (!isReservedType(v_def.origin)) {
                index = getIndexFromTDT(v_def.origin);

                // error: not found
                if (index == -1) {
                    error = 22;
                    return;
                }
                else {
                    v_def.origin = TDT.typeDescriptors[index].origin;
                }
            }

            // add code
            code += v_def.parent + " " + v_def.name;
        }
        else {
            v_def.parent = "void";
            v_def.origin = "void";
            code += "void " + v_def.name;
        }
        v_def.maxIndexLevel = 0; // only id, no array

        createAndCheckDT(children[1]); // ArgDefinePartAST
        if (error > 0) { return; }
        VarFuncRegion region = children[1]->getR1();

        VarFuncdef vf_def;
        vf_def.def = v_def;
        vf_def.attribute = "Function";
        std::vector<Vardef> funcArg;
        for (unsigned int i = 0; i < region.varFunc_defs.size(); i++) {
            Vardef tmp = region.varFunc_defs[i].def;
            funcArg.push_back(tmp);
        }
        vf_def.funcArg = funcArg;

        // need to deal with same name;
        int index = getIndexFromVFRegion(v_def.name, node->getR1());
        if (index != -1) {
            error = 24;
            return;
        }

        // add r1
        node->addR1(vf_def);

        // create VFD
        VarFuncDescriptor desc;
        desc.name = v_def.name;
        desc.parent = v_def.parent;
        desc.origin = v_def.origin;
        desc.attribute = vf_def.attribute;
        desc.env = node->getR1().envName;

        // renew VFDT
        VFDT.varFuncDescriptors.push_back(desc);

        // create and set r2
        VarFuncRegion r2;
        r2.envName = node->getR1().envName;
        r2.envName += (" Function " + v_def.name);
        for (unsigned int i = 0; i < funcArg.size(); i++) {
            VarFuncdef tmp;
            tmp.def = funcArg[i];
            if (tmp.def.maxIndexLevel > 0) {
                tmp.attribute = "Array";
            }
            else {
                tmp.attribute = "Single";
            }

            // need to deal with same name;
            int index = getIndexFromVFRegion(tmp.def.name, r2);
            if (index != -1) {
                error = 24;
                return;
            }
            r2.varFunc_defs.push_back(tmp);
        }
        node->setR2(r2);

        break;
    }
    case 14: // FunctionVarDefineBlockAST
        children[0]->setR1(node->getR1()); // set r1 to Var
        createAndCheckDT(children[0]);
        if (error > 0) { return; }
        node->setR1(children[0]->getR1()); // get r1 from Var
        break;
    case 15: // FunctionExecuteBlockAST
        children[0]->setR1(node->getR1()); // set r1 to Execute
        children[0]->setR2(node->getR2()); // set r2 to Execute
        createAndCheckDT(children[0]);
        if (error > 0) { return; }
        break;
    case 16: { // ArgDefinePartAST
        code += "(";
        if (children.size() != 0) {
            for (unsigned int i = 0; i < children.size() - 1; i++) {
                createAndCheckDT(children[i]);
                if (error > 0) { return; }
                node->addR1(children[i]->getR1()); // get r1 from Var
                // add code
                code += ",";
            }
            createAndCheckDT(children[children.size() - 1]);
            if (error > 0) { return; }
            node->addR1(children[children.size() - 1]->getR1()); // get r1 from Var
        }
        // add code
        code += ")";
        break;
    }
    case 17: // MainExecutePartAST
        if (children.size() != 0) {
            children[0]->setR1(node->getR1()); // set r1 to Execute
            children[0]->setR2(node->getR2()); // set r2 to Execute
            createAndCheckDT(children[0]);
            if (error > 0) { return; }
        }
        if (((MainExecutePartAST*)node)->getType() == 1) {
            code += ";\n";
        }
        break;
    case 18: // ExecuteInsBlockAST
        if (children.size() != 0) {
            children[0]->setR1(node->getR1()); // set r1 to Execute
            children[0]->setR2(node->getR2()); // set r2 to Execute
            createAndCheckDT(children[0]);
            if (error > 0) { return; }
        }
        break;
    case 19: // ExecuteInsPartAST
        for (unsigned int i = 0; i < children.size(); i++) {
            children[i]->setR1(node->getR1()); // set r1 to Execute
            children[i]->setR2(node->getR2()); // set r2 to Execute
            createAndCheckDT(children[i]);
            if (error > 0) { return; }
            // add code;
            code += ";\n";
        }
        break;
    case 20: { // ExecuteInsAST
        int type = ((ExecuteInsAST*)node)->getType();
        children[0]->setR1(node->getR1()); // set r1 to Execute
        children[0]->setR2(node->getR2()); // set r2 to Execute

        // notice ret only can occur in func
        // if not in func, r2 is null
        if (type == 3 && node->getR2().envName == "") {
            error = 1;
            std::cout << "haha: Execute err\n";
            return;
        }

        createAndCheckDT(children[0]);
        if (error > 0) { return; }
        break;
    }
    case 21: { // EvaluationInsAST
        if (children.size() == 1) { // for ExprAST
            children[0]->setR1(node->getR1()); // set r1 to Execute
            children[0]->setR2(node->getR2()); // set r2 to Execute
            createAndCheckDT(children[0]);
            if (error > 0) { return; }
        }
        else { // for MemoryStructAST = ExprAST
            children[0]->setR1(node->getR1()); // set r1 to Execute
            children[0]->setR2(node->getR2()); // set r2 to Execute
            createAndCheckDT(children[0]);
            if (error > 0) { return; }

            // add code
            code += " = ";

            children[1]->setR1(node->getR1()); // set r1 to Execute
            children[1]->setR2(node->getR2()); // set r2 to Execute
            createAndCheckDT(children[1]);
            if (error > 0) { return; }

            // get type info for MemoryStructAST and ExprAST
            std::string memoryType = ((MemoryStructAST*)children[0])->getTypeName();
            std::string exprType = ((ExprAST*)children[1])->getTypeName();
            int memoryIndex = getIndexFromTDT(memoryType);
            int exprIndex = getIndexFromTDT(exprType);
            std::string memoryOrigin = memoryType; // for RESERVEDTYPE or ERROR
            if (memoryIndex != -1) {
                memoryOrigin = TDT.typeDescriptors[memoryIndex].origin;
            }
            std::string exprOrigin = exprType; // for RESERVEDTYPE or ERROR
            if (exprIndex != -1) {
                exprOrigin = TDT.typeDescriptors[exprIndex].origin;
            }

            // check the type
            int warn = -1;
            if (!legalEvaluation(memoryOrigin, exprOrigin, warn)) {
                error = 25;
                return;
            }

            // deal with warn
            if (warn >= 0) {
                warning.push_back(warn);
            }
        }
        break;
    }
    case 22: // LoopInsAST
        children[0]->setR1(node->getR1()); // set r1 to Execute
        children[0]->setR2(node->getR2()); // set r2 to Execute
        createAndCheckDT(children[0]);
        if (error > 0) { return; }
        break;
    case 23: // ConditionInsAST
        children[0]->setR1(node->getR1()); // set r1 to Execute
        children[0]->setR2(node->getR2()); // set r2 to Execute
        createAndCheckDT(children[0]);
        if (error > 0) { return; }
        break;
    case 24: { // ReturnInsAST
        // get func name from r2.env
        std::stringstream ss(node->getR2().envName);
        std::string funcName, tmp;
        while (ss >> tmp) {
            funcName = tmp;
        }

        // get func origin from r1
        VarFuncRegion r1 = node->getR1();
        int funcIndex = getIndexFromVFRegion(funcName, r1);

        if (funcIndex == -1) {
            error = 33; // secret error?
            return;
        }

        std::string funcOrigin = r1.varFunc_defs[funcIndex].def.origin;

        // add code
        code += "return ";

        // now for the ret-Expr
        if (children.size() != 0) {
            children[0]->setR1(node->getR1()); // set r1 to Execute
            children[0]->setR2(node->getR2()); // set r2 to Execute
            createAndCheckDT(children[0]);
            if (error > 0) { return; }

            // class should be defined
            // get ret origin from child
            std::string returnType = ((ExprAST*)children[0])->getTypeName();
            int returnIndex = getIndexFromTDT(returnType);
            std::string returnOrigin = returnType; // for RESERVEDTYPE or ERROR
            if (returnIndex != -1) {
                returnOrigin = TDT.typeDescriptors[funcIndex].origin;
            }
            if (returnOrigin == "class") {
                returnOrigin == returnType; // class should keep its type
            }

            // check the type, notice 'return Expr' is equal to 'TempMS(funcType) = Expr'
            int warn = -1;
            if (!legalEvaluation(funcOrigin, returnOrigin, warn)) {
                error = 26;
                return;
            }

            // deal with warn
            if (warn >= 0) {
                warning.push_back(warn);
            }
        }
        else {
            if (funcOrigin != "void") {
                error = 26;
                return;
            }
        }
        break;
    }
    case 25: { // ForInsAST
        // add code
        code += "for (";
        children[0]->setR1(node->getR1()); // set r1 to Execute
        children[0]->setR2(node->getR2()); // set r2 to Execute
        createAndCheckDT(children[0]);
        if (error > 0) { return; }

        // add code
        code += "= ";

        children[1]->setR1(node->getR1()); // set r1 to Execute
        children[1]->setR2(node->getR2()); // set r2 to Execute
        createAndCheckDT(children[1]);
        if (error > 0) { return; }

        // add code
        code += "; ";

        children[0]->setR1(node->getR1()); // set r1 to Execute
        children[0]->setR2(node->getR2()); // set r2 to Execute
        createAndCheckDT(children[0]);
        if (error > 0) { return; }

        // add code
        code += "<= ";

        children[2]->setR1(node->getR1()); // set r1 to Execute
        children[2]->setR2(node->getR2()); // set r2 to Execute
        createAndCheckDT(children[2]);
        if (error > 0) { return; }

        // add code
        code += "; ";

        children[0]->setR1(node->getR1()); // set r1 to Execute
        children[0]->setR2(node->getR2()); // set r2 to Execute
        createAndCheckDT(children[0]);
        if (error > 0) { return; }

        // add code
        if (((ForInsAST*)node)->isUpper()) {
            code += "++";
        }
        else {
            code += "--";
        }
        code += ")";

        // get type info for MemoryStructAST ExprAST1 and ExprAST2
        std::string memoryType = ((MemoryStructAST*)children[0])->getTypeName();
        std::string expr1Type = ((ExprAST*)children[1])->getTypeName();
        std::string expr2Type = ((ExprAST*)children[2])->getTypeName();
        int memoryIndex = getIndexFromTDT(memoryType);
        int expr1Index = getIndexFromTDT(expr1Type);
        int expr2Index = getIndexFromTDT(expr2Type);
        std::string memoryOrigin = memoryType; // for RESERVEDTYPE or ERROR
        if (memoryIndex != -1) {
            memoryOrigin = TDT.typeDescriptors[memoryIndex].origin;
        }
        std::string expr1Origin = expr1Type; // for RESERVEDTYPE or ERROR
        if (expr1Index != -1) {
            expr1Origin = TDT.typeDescriptors[expr1Index].origin;
        }
        std::string expr2Origin = expr2Type; // for RESERVEDTYPE or ERROR
        if (expr2Index != -1) {
            expr2Origin = TDT.typeDescriptors[expr2Index].origin;
        }

        // check the type
        /*
         * [TODO] maybe for the char
         * [TODO] maybe type transform
         */
        if (memoryOrigin != "integer" || expr1Origin != "integer" || expr2Origin != "integer") {
            error = 27;
            return;
        }

        // add code
        code += "{\n";

        children[3]->setR1(node->getR1()); // set r1 to Execute
        children[3]->setR2(node->getR2()); // set r2 to Execute
        createAndCheckDT(children[3]);
        if (error > 0) { return; }

        // add code
        code += "}";

        break;
    }
    /*
    case 26: { // ForeachInsAST [TODO]
        children[0]->setR1(node->getR1()); // set r1 to Execute
        children[0]->setR2(node->getR2()); // set r2 to Execute
        createAndCheckDT(children[0]);

        children[1]->setR1(node->getR1()); // set r1 to Execute
        children[1]->setR2(node->getR2()); // set r2 to Execute
        createAndCheckDT(children[1]);

        // get type info for MemoryStructAST1 and MemoryStructAST2
        std::string memory1Type = ((MemoryStructAST*)children[0])->getTypeName();
        std::string memory2Type = ((MemoryStructAST*)children[1])->getTypeName();
        int memory1Index = getIndexFromTDT(memory1Type);
        int memory2Index = getIndexFromTDT(memory2Type);
        std::string memory1Origin = memory1Type; // for RESERVEDTYPE or ERROR
        if (memory1Index != -1) {
            memory1Origin = TDT.typeDescriptors[memory1Index].origin;
        }
        std::string memory2Origin = memory2Type; // for RESERVEDTYPE or ERROR
        if (memory2Index != -1) {
            memory2Origin = TDT.typeDescriptors[memory2Index].origin;
        }

        // check the type
        /*
         * [TODO] maybe we need more strict judgement
         */
        /*
        if (memory1Origin != memory2Origin) {
            error = 1;
cout << "haha: ForeachIns err\n";
            return;
        }

        children[2]->setR1(node->getR1()); // set r1 to Execute
        children[2]->setR2(node->getR2()); // set r2 to Execute
        createAndCheckDT(children[2]);

        break;
    }*/
    case 27: { // WhileInsAST
        // add code
        code += "while (";

        children[0]->setR1(node->getR1()); // set r1 to Execute
        children[0]->setR2(node->getR2()); // set r2 to Execute
        createAndCheckDT(children[0]);
        if (error > 0) { return; }

        // add code
        code += ")";

        // get type info for ExprAST
        std::string exprType = ((ExprAST*)children[0])->getTypeName();
        int exprIndex = getIndexFromTDT(exprType);
        std::string exprOrigin = exprType; // for RESERVEDTYPE or ERROR
        if (exprIndex != -1) {
            exprOrigin = TDT.typeDescriptors[exprIndex].origin;
        }

        // check the type
        /*
         * [TODO] maybe we can deal with integer
         */
        if (exprOrigin != "boolean") {
            error = 28;
            return;
        }

        // add code
        code += "{\n";

        children[1]->setR1(node->getR1()); // set r1 to Execute
        children[1]->setR2(node->getR2()); // set r2 to Execute
        createAndCheckDT(children[1]);
        if (error > 0) { return; }

        // add code
        code += "}";

        break;
    }
    case 28: { // IfInsAST
        // add code
        code += "if (";

        children[0]->setR1(node->getR1()); // set r1 to Execute
        children[0]->setR2(node->getR2()); // set r2 to Execute
        createAndCheckDT(children[0]);
        if (error > 0) { return; }

        // add code
        code += ")";

        // get type info for ExprAST
        std::string exprType = ((ExprAST*)children[0])->getTypeName();
        int exprIndex = getIndexFromTDT(exprType);
        std::string exprOrigin = exprType; // for RESERVEDTYPE or ERROR
        if (exprIndex != -1) {
            exprOrigin = TDT.typeDescriptors[exprIndex].origin;
        }

        // check the type
        /*
         * [TODO] maybe we can deal with integer
         */
        if (exprOrigin != "boolean") {
            error = 29;
            return;
        }

        // for then
        // add code
        code += "{\n";

        children[1]->setR1(node->getR1()); // set r1 to Execute
        children[1]->setR2(node->getR2()); // set r2 to Execute
        createAndCheckDT(children[1]);
        if (error > 0) { return; }

        // add code
        code += "}";

        // for else
        if (children.size() == 3) {
            // add code
            code += "\nelse{\n";

            children[2]->setR1(node->getR1()); // set r1 to Execute
            children[2]->setR2(node->getR2()); // set r2 to Execute
            createAndCheckDT(children[2]);
            if (error > 0) { return; }

            // add code
            code += "}";
        }

        break;
    }
    case 29: // ExprAST
        if (children.size() == 1) { // for Term1AST
            children[0]->setR1(node->getR1()); // set r1 to Execute
            children[0]->setR2(node->getR2()); // set r2 to Execute
            createAndCheckDT(children[0]);
            if (error > 0) { return; }

            ((ExprAST*)node)->setTypeName(((Term1AST*)children[0])->getTypeName()); // set typeName
        }
        else { // for ExprAST op Term1AST
            // add code
            code += "(";

            children[0]->setR1(node->getR1()); // set r1 to Execute
            children[0]->setR2(node->getR2()); // set r2 to Execute
            createAndCheckDT(children[0]);
            if (error > 0) { return; }

            // add code
            code += getTransOp(((TokenAST*)children[1])->getOp().str) + " ";

            children[2]->setR1(node->getR1()); // set r1 to Execute
            children[2]->setR2(node->getR2()); // set r2 to Execute
            createAndCheckDT(children[2]);
            if (error > 0) { return; }

            // get type info for ExprAST and Term1AST
            std::string exprType = ((ExprAST*)children[0])->getTypeName();
            std::string term1Type = ((Term1AST*)children[2])->getTypeName();
            int exprIndex = getIndexFromTDT(exprType);
            int term1Index = getIndexFromTDT(term1Type);
            std::string exprOrigin = exprType; // for RESERVEDTYPE or ERROR
            if (exprIndex != -1) {
                exprOrigin = TDT.typeDescriptors[exprIndex].origin;
            }
            std::string term1Origin = term1Type; // for RESERVEDTYPE or ERROR
            if (term1Index != -1) {
                term1Origin = TDT.typeDescriptors[term1Index].origin;
            }

            // check the type, only for same type and don't care about class and void
            /*
            // [TODO] may be we should think about type transformation
            */
            if (exprOrigin == term1Origin && exprOrigin != "class" && exprOrigin != "void") {
                ((ExprAST*)node)->setTypeName("boolean"); // set typeName
            }
            else {
                error = 30;
                return;
            }

            // add code
            code += ")";
        }
        break;
    case 30: // Term1AST
        if (children.size() == 1) { // for Term1AST
            children[0]->setR1(node->getR1()); // set r1 to Execute
            children[0]->setR2(node->getR2()); // set r2 to Execute
            createAndCheckDT(children[0]);
            if (error > 0) { return; }

            ((Term1AST*)node)->setTypeName(((Term2AST*)children[0])->getTypeName()); // set typeName
        }
        else { // for Term1AST op Term2AST
            // add code
            code += "(";

            children[0]->setR1(node->getR1()); // set r1 to Execute
            children[0]->setR2(node->getR2()); // set r2 to Execute
            createAndCheckDT(children[0]);
            if (error > 0) { return; }

            // add code
            code += getTransOp(((TokenAST*)children[1])->getOp().str) + " ";

            children[2]->setR1(node->getR1()); // set r1 to Execute
            children[2]->setR2(node->getR2()); // set r2 to Execute
            createAndCheckDT(children[2]);
            if (error > 0) { return; }

            // add code
            code += ")";

            // get type info for Term1AST and Term2AST
            std::string term1Type = ((Term1AST*)children[0])->getTypeName();
            std::string term2Type = ((Term2AST*)children[2])->getTypeName();
            int term1Index = getIndexFromTDT(term1Type);
            int term2Index = getIndexFromTDT(term2Type);
            std::string term1Origin = term1Type; // for RESERVEDTYPE or ERROR
            if (term1Index != -1) {
                term1Origin = TDT.typeDescriptors[term1Index].origin;
            }
            std::string term2Origin = term2Type; // for RESERVEDTYPE or ERROR
            if (term2Index != -1) {
                term2Origin = TDT.typeDescriptors[term2Index].origin;
            }

            // check the type
            /*
            // [TODO] may be we should think about type transformation
            */
            Token op = ((TokenAST*)children[1])->getOp();
            if (op.str == "+") {
                // char + char
                // string + string
                // integer + integer
                if ((term1Origin == term2Origin) &&
                    (term1Origin == "char" || term1Origin == "string" || term1Origin == "integer")) {
                    ((Term1AST*)node)->setTypeName(term1Origin); // set typeName
                }
                else if ((term1Origin == "integer" || term1Origin == "real") &&
                         (term2Origin == "integer" || term2Origin == "real")) { // integer/real + integer/real, but not both integer
                    ((Term1AST*)node)->setTypeName("real"); // set typeName
                }
                else {
                    error = 30; // error: not suitable type
                    return;
                }
            }
            else if (op.str == "-") {
                // integer - integer
                if (term1Origin == term2Origin && term1Origin == "integer") {
                    ((Term1AST*)node)->setTypeName("integer"); // set typeName
                }
                else if ((term1Origin == "integer" || term1Origin == "real") &&
                         (term2Origin == "integer" || term2Origin == "real")) { // integer/real + integer/real, but not both integer
                    ((Term1AST*)node)->setTypeName("real"); // set typeName
                }
                else {
                    error = 30; // error: not suitable type
                    return;
                }
            }
            else if (op.str == "or" || op.str == "xor") {
                // integer op integer
                // boolean op boolean
                if (term1Origin == term2Origin && (term1Origin == "integer" || term1Origin == "boolean")) {
                    ((Term1AST*)node)->setTypeName(term1Origin); // set typeName
                }
                else {
                    error = 30;
                    return;
                }
            }
            else {
                error = 33; // secret error?
                return;
            }
        }
        break;
    case 31:  // Term2AST
        if (children.size() == 1) { // for Term3 AST
            children[0]->setR1(node->getR1()); // set r1 to Execute
            children[0]->setR2(node->getR2()); // set r2 to Execute
            createAndCheckDT(children[0]);
            if (error > 0) { return; }

            ((Term2AST*)node)->setTypeName(((Term3AST*)children[0])->getTypeName()); // set typeName
        }
        else { // for Term2AST op Term3AST
            // add code
            code += "(";

            children[0]->setR1(node->getR1()); // set r1 to Execute
            children[0]->setR2(node->getR2()); // set r2 to Execute
            createAndCheckDT(children[0]);
            if (error > 0) { return; }

            // add code
            code += getTransOp(((TokenAST*)children[1])->getOp().str) + " ";

            children[2]->setR1(node->getR1()); // set r1 to Execute
            children[2]->setR2(node->getR2()); // set r2 to Execute
            createAndCheckDT(children[2]);
            if (error > 0) { return; }

            // add code
            code += ")";

            // get type info for Term2AST and Term3AST
            std::string term2Type = ((Term2AST*)children[0])->getTypeName();
            std::string term3Type = ((Term3AST*)children[2])->getTypeName();
            int term2Index = getIndexFromTDT(term2Type);
            int term3Index = getIndexFromTDT(term3Type);
            std::string term2Origin = term2Type; // for RESERVEDTYPE or ERROR
            if (term2Index != -1) {
                term2Origin = TDT.typeDescriptors[term2Index].origin;
            }
            std::string term3Origin = term3Type; // for RESERVEDTYPE or ERROR
            if (term3Index != -1) {
                term3Origin = TDT.typeDescriptors[term3Index].origin;
            }

            // check the type
            /*
            // [TODO] may be we should think about type transformation
            */
            Token op = ((TokenAST*)children[1])->getOp();
            if (op.str == "*") {
                // integer * integer
                if (term2Origin == term3Origin && term2Origin == "integer") {
                    ((Term2AST*)node)->setTypeName("integer"); // set typeName
                }
                else if ((term2Origin == "integer" || term2Origin == "real") &&
                         (term3Origin == "integer" || term3Origin == "real")) { // integer/real * integer/real, but not both integer
                    ((Term2AST*)node)->setTypeName("real"); // set typeName
                }
                else {
                    error = 30; // error: not suitable type
                    return;
                }
            }
            else if (op.str == "/") {
                // integer/real / integer/real
                if ((term2Origin == "integer" || term2Origin == "real") &&
                    (term3Origin == "integer" || term3Origin == "real")) {
                    ((Term1AST*)node)->setTypeName("real"); // set typeName
                }
                else {
                    error = 30; // error: not suitable type
                    return;
                }
            }
            else if (op.str == "mod" || op.str == "div") {
                //integer op integer
                if (term2Origin == term3Origin && term2Origin == "integer") {
                    ((Term2AST*)node)->setTypeName("integer");
                }
                else {
                    error = 30; // error: not suitable type
                    return;
                }
            }
            else if (op.str == "and") {
                // integer and integer
                // boolean and boolean
                if (term2Origin == term3Origin && (term2Origin == "integer" || term2Origin == "boolean")) {
                    ((Term2AST*)node)->setTypeName(term2Origin); // set typeName
                }
                else {
                    error = 30; // error: not suitable type
                    return;
                }
            }
            else {
                error = 33; // secret error?
                return;
            }
        }
        break;
    case 32: // Term3AST
        if (children.size() == 1) { // for ValueStructAST
            children[0]->setR1(node->getR1()); // set r1 to Execute
            children[0]->setR2(node->getR2()); // set r2 to Execute
            createAndCheckDT(children[0]);
            if (error > 0) { return; }

            ((Term3AST*)node)->setTypeName(((ValueStructAST*)children[0])->getTypeName()); // set typeName
        }
        else if (children.size() == 2) { // for not Term3AST
            // add code
            code += "(";

            // add code
            code += getTransOp(((TokenAST*)children[0])->getOp().str) + " ";

            children[1]->setR1(node->getR1()); // set r1 to Execute
            children[1]->setR2(node->getR2()); // set r2 to Execute
            createAndCheckDT(children[1]);
            if (error > 0) { return; }

            // add code
            code += ")";

            // get Term3 type
            std::string type = ((Term3AST*)children[0])->getTypeName();
            int index = getIndexFromTDT(type);
            std::string origin = type; // for RESERVEDTYPE or ERROR
            if (index != -1) {
                origin = TDT.typeDescriptors[index].origin;
            }

            // type check
            if (origin == "boolean" || origin == "integer") {
                ((Term3AST*)node)->setTypeName(origin); // set typeName
            }
            else {
                error = 30; // error: not suitable type
                return;
            }
        }
        else { // for ( ExprAST )
            // add code
            code += "(";

            children[1]->setR1(node->getR1()); // set r1 to Execute
            children[1]->setR2(node->getR2()); // set r2 to Execute
            createAndCheckDT(children[1]);
            if (error > 0) { return; }

            // add code
            code += ")";

            ((Term3AST*)node)->setTypeName(((ExprAST*)children[1])->getTypeName()); // set typeName
        }
        break;
    case 33: { // ValueStructAST
        int type = ((ValueStructAST*)node)->getType();

        if (type == 0) { // for MemoryStructAST
            children[0]->setR1(node->getR1()); // set r1 to Execute
            children[0]->setR2(node->getR2()); // set r2 to Execute
            createAndCheckDT(children[0]);
            if (error > 0) { return; }

            ((ValueStructAST*)node)->setTypeName(((MemoryStructAST*)children[0])->getTypeName()); // set typeName
        }
        else { // for const Token
            Token op = ((TokenAST*)children[0])->getOp();
            // add code
            code += op.str + " ";

            switch (op.type) {
            case NUMBERNO: {
                // differ integer and real, they can all be NUMBERNO
                std::string tmp = "integer";
                for (unsigned int i = 0; i < op.str.size(); i++) {
                    if (op.str[i]=='.') {
                        tmp = "real";
                        break;
                    }
                }
                ((ValueStructAST*)node)->setTypeName(tmp); // number
                break;
            }
            case CHARNO:
                ((ValueStructAST*)node)->setTypeName("char"); // char
                break;
            case STRINGNO:
                ((ValueStructAST*)node)->setTypeName("string"); // string
                break;
            default: // secret error?
                error = 33;
                return;
            }
        }
        break;
    }
    case 34: { // MemoryStructAST
        int type = ((MemoryStructAST*)node)->getType();
        VarFuncRegion null;

        switch (type) {
        case 0: case 1: { // for M.F/M.A
            children[0]->setR1(node->getR1()); // set r1 to Execute
            children[0]->setR2(node->getR2()); // set r2 to Execute
            createAndCheckDT(children[0]);
            if (error > 0) { return; }

            // add code
            code += ".";

            node->setR1(children[0]->getR1()); // get r1 from M
            node->setR2(children[0]->getR2()); // get r2 from M (is always null)

            children[1]->setR1(node->getR1()); // set r1 to Execute
            children[1]->setR2(node->getR2()); // set r2 to Execute
            createAndCheckDT(children[1]);
            if (error > 0) { return; }

            // get F/A type and region
            std::string typeName;
            if (type == 0) {
                typeName = ((FunctionCallAST*)children[1])->getTypeName();
            }
            else {
                typeName = ((ArrayAST*)children[1])->getTypeName();
            }
            int index = getIndexFromTDT(typeName);

            VarFuncRegion region = null; // for RESERVEDTYPE or ERROR
            if (index != -1) {
                region = typeRegion.typedefs[index].region; // notice index in typeRegion is same with TDT
            }

            ((MemoryStructAST*)node)->setTypeName(typeName); // set typeName
            node->setR1(region); // set new region to r1
            node->setR2(null); // node's r2 is null
            break;
        }
        case 2: case 3: { // for F/A
            children[0]->setR1(node->getR1()); // set r1 to Execute
            children[0]->setR2(node->getR2()); // set r2 to Execute
            createAndCheckDT(children[0]);
            if (error > 0) { return; }

            // get F/A type and region
            std::string typeName;
            if (type == 2) {
                typeName = ((FunctionCallAST*)children[0])->getTypeName();
            }
            else {
                typeName = ((ArrayAST*)children[0])->getTypeName();
            }
            int index = getIndexFromTDT(typeName);

            VarFuncRegion region = null; // for RESERVEDTYPE or ERROR
            if (index != -1) {
                region = typeRegion.typedefs[index].region; // notice index in typeRegion is same with TDT
            }

            ((MemoryStructAST*)node)->setTypeName(typeName); // set typeName
            node->setR1(region); // set new region to r1
            node->setR2(null); // node's r2 is null
            break;
        }
        default:
            break;
        }
        break;
    }
    case 35: { // ArrayAST
        if (!(((ArrayAST*)node)->isDefined())) {
            children[0]->setR1(node->getR1()); // set r1 to Execute
            children[0]->setR2(node->getR2()); // set r2 to Execute
            createAndCheckDT(children[0]);
            if (error > 0) { return; }

            ((ArrayAST*)node)->setTypeName(((TokenAST*)children[0])->getTypeName());
            ((ArrayAST*)node)->setMaxIndexLevel(0); // for no index

            if (children.size() != 1) {
                children[1]->setR1(node->getR1()); // set r1 to Execute
                children[1]->setR2(node->getR2()); // set r2 to Execute
                createAndCheckDT(children[1]);
                if (error > 0) { return; }
                ((ArrayAST*)node)->setMaxIndexLevel(((IndexPartAST*)children[1])->getMaxIndexLevel());
            }

            // is it really array
            if (((TokenAST*)children[0])->isFunc()) {
                error = 31;
                return;
            }

            /*
            // [TODO] check maxIndexLevel
            */
        }
        else {
            code += ((TokenAST*)children[0])->getOp().str;
            ((ArrayAST*)node)->setIdName(((TokenAST*)children[0])->getOp().str);
            ((ArrayAST*)node)->setMaxIndexLevel(0); // for no index

            if (children.size() != 1) {
                createAndCheckDT(children[1]);
                ((ArrayAST*)node)->setMaxIndexLevel(((IndexPartAST*)children[1])->getMaxIndexLevel());
            }
        }
        break;
    }
    case 36: // IndexPartAST
        ((IndexPartAST*)node)->setMaxIndexLevel(children.size());
        for (unsigned int i = 0; i < children.size(); i++) {
            // add code
            code += "[";

            children[i]->setR1(node->getR1()); // set r1 to Execute
            children[i]->setR2(node->getR2()); // set r2 to Execute
            createAndCheckDT(children[i]);
            if (error > 0) { return; }

            // add code
            code += "]";

            // get typeName
            std::string typeName = ((ExprAST*)children[i])->getTypeName();

            int index = getIndexFromTDT(typeName);

            // only for integer
            /*
             * [TODO] maybe for char
            */
            if (index == -1) {
                if (typeName != "integer") {
                    error = 32;
                    return;
                }
            }
            else if (TDT.typeDescriptors[index].origin != "integer") {
                error = 32;
                return;
            }
        }
        break;
    case 37: { // FunctionCallAST
        children[0]->setR1(node->getR1()); // set r1 to Execute
        children[0]->setR2(node->getR2()); // set r2 to Execute
        createAndCheckDT(children[0]);
        if (error > 0) { return; }

        // add code
        code += "(";

        ((FunctionCallAST*)node)->setTypeName(((TokenAST*)children[0])->getTypeName());

        if (children.size() != 1) {
            children[1]->setR1(node->getR1()); // set r1 to Execute
            children[1]->setR2(node->getR2()); // set r2 to Execute
            createAndCheckDT(children[1]);
            if (error > 0) { return; }

            // check ArgPart
            std::vector<std::string> funcArg = ((ArgPartAST*)children[1])->getTypeNames();
            std::vector<Vardef> defFuncArg = ((TokenAST*)children[0])->getFuncArg();
            if (funcArg.size() != defFuncArg.size()) {
                error = 31;
                return;
            }
            else {
                for (unsigned int i = 0; i < funcArg.size(); i++) {
                    std::string origin = defFuncArg[i].origin;
                    if (origin == "class") {
                        origin = defFuncArg[i].parent;
                    }
                    if (funcArg[i] != origin) {
                        error = 31;
                        return;
                    }
                }
            }
        }

        // add code
        code += ")";

        // is it really func
        if (!(((TokenAST*)children[0])->isFunc())) {
            error = 31;
            return;
        }

        break;
    }
    case 38: { // ArgPartAST
        for (unsigned int i = 0; i < children.size(); i++) {
            children[i]->setR1(node->getR1()); // set r1 to Execute
            children[i]->setR2(node->getR2()); // set r2 to Execute
            createAndCheckDT(children[i]);
            if (error > 0) { return; }

            // add code
            if (i < children.size()-1) {
                code += ",";
            }

            // get typeName
            std::string typeName = ((ExprAST*)children[i])->getTypeName();

            int index = getIndexFromTDT(typeName);
            std::string origin = typeName; // for RESERVEDTYPE or error

            // found
            if (index != -1) {
                origin = TDT.typeDescriptors[index].origin;
            }
            if (origin == "class") {
                origin = typeName;
            }

            // add to typeNames
            ((ArgPartAST*)node)->addTypeNames(origin);
        }

        break;
    }
    case 39: { // TokenAST
        VarFuncRegion r1 = ((TokenAST*)node)->getR1();
        VarFuncRegion r2 = ((TokenAST*)node)->getR2();
        Token op = ((TokenAST*)node)->getOp();

        // add code
        code += op.str;

        // only for id
        if (op.type != IDENTIFIERNO) {
            error = 33;
            return;
        }

        // search in r2
        int index = getIndexFromVFRegion(op.str, r2);

        // set typeName
        if (index != -1) {
            ((TokenAST*)node)->setTypeName(r2.varFunc_defs[index].def.parent);
            if (r2.varFunc_defs[index].attribute == "Function") {
                ((TokenAST*)node)->setFunc(true);
                ((TokenAST*)node)->setFuncArg(r2.varFunc_defs[index].funcArg);
            }
            else {
                ((TokenAST*)node)->setFunc(false);
            }
        }
        else { // not found: search in r1
            index = getIndexFromVFRegion(op.str, r1);

            // not found: error
            if (index == -1) {
                error = 31;
                return;
            }

            ((TokenAST*)node)->setTypeName(r1.varFunc_defs[index].def.parent);
            if (r1.varFunc_defs[index].attribute == "Function") {
                ((TokenAST*)node)->setFunc(true);
                ((TokenAST*)node)->setFuncArg(r1.varFunc_defs[index].funcArg);
            }
            else {
                ((TokenAST*)node)->setFunc(false);
            }
        }
        break;
    }
    default:
        return;
    }
}

void Analysis::do_analysing(int& _error) {
    createAndCheckDT(root);
    _error = error;
}
