#include "scanner.h"

void Scanner::add_space() {
    for (unsigned int i = 0; i < program.size(); i++) {
        std::string str = "";
        bool num_flag = true; // is the pre token num or NULL

        // copymode means copy directly, which is useful if we deal with comm/chr/str
        // 0 means normal, 1 means comm, 2 means chr, 3 means str;
        int copymode = 0;

        for (unsigned int j = 0; j < program[i].size(); j++) {
            // is it '{', '}', ''', '"', which control the openning or closing of copymode
            // [TODO] care about \' and \"
            switch (program[i][j]) {
            case '{':
                if (copymode == 0) {
                    copymode = 1; // copymode for comm
                    str = str + " " + '{'; // notice add space
                    continue;
                }
                break;
            case '}':
                if (copymode == 1) {
                    copymode = 0; // exit copymode
                    str = str + '}' + " "; // notice add space
                    num_flag = true; // notice the token is ended, init num_flag
                    continue;
                }
                break;
            case '\'':
                if (copymode == 0) {
                    copymode = 2; // copymode for char
                    str = str + " " + '\''; // notice add space
                    continue;
                }
                else if (copymode == 2) {
                    copymode = 0; // exit copymode
                    str = str + '\'' + " "; // notice add space
                    num_flag = true; // notice the token is ended, init num_flag
                    continue;
                }
                break;
            case '"':
                if (copymode == 0) {
                    copymode = 3; // copymode for string
                    str = str + " " + '"'; // notice add space
                    continue;
                }
                else if (copymode == 3) {
                    copymode = 0; // exit copymode
                    str = str + '"' + " "; // notice add space
                    num_flag = true; // notice the token is ended, init num_flag
                    continue;
                }
                break;
            default:
                break;
            }

            // in copymode?
            if (copymode != 0) {
                str += program[i][j]; // just copy
                continue;
            }

            // the next part is not in copymode

            // special symbol need to add space
            // notice special character is NOT equal to specail symbol
            if (isSpecialCharacter(program[i][j])) {
                std::string tmp = "";
                tmp += program[i][j];

                // notice some two-special-charactered symbol, only for '>=', '<=', '!=', '=='
                if ((j + 1 < program[i].size()) && (program[i][j + 1] == '=') &&
                    ((program[i][j] == '>') || (program[i][j] == '<') ||
                    (program[i][j] == '!') || (program[i][j] == '='))) {
                    tmp += '=';
                    j++;
                }

                // now it's time to judge whether the tmp is specail symbol
                // actually the only case will not pass is '!'
                if (!isSpecialSymbol(tmp)) {
                    error = 1;
                    return ;
                }

                // add space
                // notice that we should consider the special situation about '.'
                // we should add space for class, and don't do that for number
                // we can use num_flag to help us
                if ((tmp == ".") && (num_flag)) {
                    str += tmp;
                }
                else {
                    str = str + " " + tmp + " ";
                }

                // notice special character always be a new token, so we need to set num_flag to true
                num_flag = true;
                // but two symbol is except, ')' and ']', which always mean the context before is not digit
                if (program[i][j] == ')' || program[i][j] == ']') {
                    num_flag = false;
                }
            }
            else {
                // normal
                // if a non-digit occurs: 1. space? set flag to true; 2.no_space? set flag to false;
                if (!isDigit(program[i][j])){
                    if (program[i][j] == ' ')
                        // the end of token, set flag to init
                        num_flag = true;
                    else
                        // token is not ended, and a non-digit occur, set flag to false
                        num_flag = false;
                }

                str += program[i][j];
            }
        }

        // still in copymode? cause error;
        if (copymode != 0) {
            error = 2;
            return ;
        }

        // overwrite
        program[i] = str;
    }
}

void Scanner::parse_token() {
    for (unsigned int i = 0; i < program.size(); i++) {
        std::stringstream ss(program[i]);
        while (true) {
            Token token;
            std::string str = "";

            // notice special cases in comm/chr/str
            // in these cases space is not used for splitting
            // which means str may be just a part of token
            ss >> str;
            if (str == "") break;
            token.str = str;

            // there are 8 different types
            if (isReservedWord(str)) {
                token.type = RESERVEDWORDNO;
            }
            else if (isReservedType(str)) {
                token.type = RESERVEDTYPENO;
            }
            else if (isSpecialSymbol(str)) {
                token.type = SPECIALSYMBOLNO;
            }
            else if (isLetter(str[0]) || str[0] == '_') { // the beginning is letter or '_'
                for (unsigned int i = 1; i < str.size(); i++) {
                    if (!isLetter(str[i]) && !isDigit(str[i]) && (str[i] != '_')) {
                        error = 3;
                        return ;
                    }
                }
                token.type = IDENTIFIERNO;
            }
            else if (isDigit(str[0]) || str[0] == '.') { // the beginning is digit or '.'
                // notice we need to cosider the situation that we have more than one '.'
                bool dot_flag = false; // flag of '.'
                for (unsigned int i = 0; i < str.size(); i++) {
                    if (!isDigit(str[i])) {
                        if ((str[i] == '.') && (!dot_flag)){
                            dot_flag = true;
                        }
                        else {
                            error = 4;
                            return ;
                        }
                    }
                }
                token.type = NUMBERNO;
            }
            else if (str[0] == '{') {  // for comm
                // notice space will not be used for splitting, we need to get char one by one;
                // so we will use ss.get() directly
                char ch;

                // str is the all only when the end of str is '}'
                if (str[str.size() - 1] != '}') {
                    // not the end, get one by one
                    // copymode judge in add_space() can keep the existence of '}'
                    while ((ch = ss.get()) != '}') {
                        str += ch;
                    }
                    str += ch;
                    // notice we need to reset token.str again
                    token.str = str;
                }

                // because there is always a space after '}', so we don't need to judge correctness
                token.type = COMMENTNO;
            }
            else if (str[0] == '\'') { // for char
                // notice space will not be used for splitting, we need to get char one by one;
                // so we will use ss.get() directly
                char ch;

                // type Char only has one letter
                // so the case only be ' ' or '(non_space)'
                // ' ':str.size() == 1; '(non_space)':str.size() == 3 and str[2] == '\''
                // [TODO] deal with '\''
                if (str.size() == 1) {
                    ch = ss.get(); // get one letter
                    str += ch;

                    ch = ss.get(); // get '\''
                    if (ch != '\'') {
                        error = 5;
                        return ;
                    }
                    str += ch;

                    // notice we need to reset token.str again
                    token.str = str;
                }
                else if ((str.size() != 3) || (str[2] != '\'')) {
                    error = 5;
                    return ;
                }

                // because there is always a space after '}', so we don't need to judge correctness
                token.type = CHARNO;
            }
            else if (str[0] == '"') {  // for string
                // notice space will not be used for splitting, we need to get char one by one;
                // so we will use ss.get() directly
                char ch;

                // str is the all only when the end of str is '"'
                // also notice the special case for only one "
                if (str[str.size() - 1] != '"' || str.size() == 1) {
                    // copymode judge in add_space() can keep the existence of '"'
                    // [TODO] NEED TO CONSIDER "\""
                    while ((ch = ss.get()) != '"') {
                        str += ch;
                    }
                    str += ch;

                    // notice we need to reset token.str again
                    token.str = str;
                }

                // because there is always a space after '"', so we don't need to judge correctness
                token.type = STRINGNO;
            }
            else {
                error = 6;
                return ;
            }

            // push back the token
            tokens.push_back(token);
        }
    }
}

std::vector<Token> Scanner::do_scanning(int& _error) {
    std::vector<Token> null;

    add_space();
    if (error != 0) {
        _error = error;
        return null;
    }

    parse_token();
    _error = error;
    if (error == 0) {
        return tokens;
    }
    else {
        return null;
    }
}
