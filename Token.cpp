//
// Created by Daniel Slade on 11/23/2022.
//
#include "TokenType.h"
#include "Token.h"
#include <string>
#include <utility>
using namespace std;

Token::Token(TokenType t, string val, int line)
{
    type = t;
    value = std::move(val);
    this->line = line;
}
Token::Token(Token *t)
{
    type = t->getTokenType();
    value = *(t->getVal());
    this->line = -1;
}

string Token::toString()
{
    return (to_string(line) + (string)": Token Type: " + getTokenName(type) + ", Value: " + value + "\n");
}

TokenType Token::getTokenType() {
    return type;
}

string* Token::getVal() {
    return &value;
}
int Token::getLine() const {
    return line;
}


string Token::getTokenName(TokenType type)
{
    switch(type) {
        case INVALID:
            return "INVALID";
        case EQUAL:
            return "EQUAL";
        case EQUALEQUAL:
            return "EQUALEQUAL";
        case NOTEQUAL:
            return "NOTEQUAL";
        case LESSTHAN:
            return "LESSTHAN";
        case MORETHAN:
            return "MORETHAN";
        case LESSEQUALS:
            return "LESSEQUALS";
        case MOREEQUALS:
            return "MOREEQUALS";
        case AND:
            return "AND";
        case OR:
            return "OR";
        case PLUS:
            return "PLUS";
        case MINUS:
            return "MINUS";
        case TIMES:
            return "TIMES";
        case DIVIDE:
            return "DIVIDE";
        case MOD:
            return "MOD";
        case NOT:
            return "NOT";
        case PRINT:
            return "PRINT";
        case INPUT:
            return "INPUT";
        case LBRACE:
            return "LBRACE";
        case RBRACE:
            return "RBRACE";
        case LPAREN:
            return "LPAREN";
        case RPAREN:
            return "RPAREN";
        case SEMICOLON:
            return "SEMICOLON";
        case INT:
            return "INT";
        case STR:
            return "STR";
        case NAME:
            return "NAME";
        case INTVAR:
            return "INTVAR";
        case STRVAR:
            return "STRVAR";
        case IF:
            return "IF";
        case ELSE:
            return "ELSE";
        case WHILE:
            return "WHILE";
        case FOR:
            return "FOR";
        default:
            return "UNKNOWN";
    }
}