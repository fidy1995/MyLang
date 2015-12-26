#ifndef SCANNER
#define SCANNER

#include "tokenconst.h"
#include <sstream>
#include <vector>

class Scanner {
private:
    std::vector<std::string> program;
    std::vector<Token> tokens;
    int error;

    void add_space();
    void parse_token();

public:
    Scanner(std::vector<std::string> _p):program(_p), error(0) {}
    std::vector<Token> do_scanning(int& _error);
};

#endif // SCANNER

