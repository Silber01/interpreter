//
// Created by Daniel Slade on 11/23/2022.
//

#ifndef INTERPRETER_TOKEN_H
#define INTERPRETER_TOKEN_H
#include <string>
#include "TokenType.h"

class Token {
public:
    Token(TokenType t, std::string val, int line);
    Token(Token *t, int i = 0);
    std::string toString();
    TokenType getTokenType();
    std::string* getVal();
    int getLine() const;
    static std::string getTokenName(TokenType t);
private:
    TokenType type;
    std::string value;
    int line;
};


#endif //INTERPRETER_TOKEN_H
