#ifndef REGIONCONST
#define REGIONCONST

#include "scanner.h"

class Vardef {
public:
    std::string name;
    std::string parent; // uplevel type
    std::string origin; // origin type
    int maxIndexLevel; // for array
};

class VarFuncdef {
public:
    Vardef def;
    std::string attribute; // only Single/Function/Array
    std::vector<Vardef> funcArg; // for func's args
};

class VarFuncRegion {
public:
    std::string envName;
    std::vector<VarFuncdef> varFunc_defs;
};

class VarFuncDescriptor { // for output;
public:
    std::string name;
    std::string parent; // uplevel type
    std::string origin; // origin type
    std::string attribute; // only Single/Function/Array
    std::string env; // for the env in which it can be used
};

class VarFuncDescriptorTable {
public:
    std::vector<VarFuncDescriptor> varFuncDescriptors;
};

class Typedef {
public:
    std::string name;
    std::string type;
    VarFuncRegion region; // for all the var we can use
};

class TypeRegion {
public:
    std::vector<Typedef> typedefs;
};

class TypeDescriptor {
public:
    std::string name; // type name
    std::string parent; // uplevel type
    std::string origin; // original type
};

class TypeDescriptorTable {
public:
    std::vector<TypeDescriptor> typeDescriptors;
};

#endif // REGIONCONST

