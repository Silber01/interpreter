//
// Created by Daniel Slade on 11/25/2022.
//

#ifndef INTERPRETER_BLOCK_H
#define INTERPRETER_BLOCK_H
#include "Token.h"
#include <vector>
#include <unordered_map>

class Block {
public:
    Block(std::vector<Token>* t, int i,
          std::unordered_map<std::string, TokenType>* typeLookup,
          std::unordered_map<std::string, std::string>* varLookup);
    int execute();
    int skip();
private:
    std::vector<Token>* tokens;
    int tIter;
    std::unordered_map<std::string, TokenType>* typeLookup;
    std::unordered_map<std::string, std::string>* varLookup;
};


#endif //INTERPRETER_BLOCK_H
