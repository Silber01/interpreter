//
// Created by Daniel Slade on 11/25/2022.
//

#include "Block.h"
#include <iostream>
using namespace std;

//void printVector(vector<Token*>* tokens)
//{
//    cout << "VECTOR CONTAINS:\n";
//    for (auto &i: *tokens)
//    {
//        cout << i->toString();
//    }
//}
//void printVector(vector<Token>* tokens)
//{
//    cout << "VECTOR CONTAINS:\n";
//    for (auto &i: *tokens)
//    {
//        cout << i.toString();
//    }
//}
Token* getVar(string *name,
           unordered_map<std::string, TokenType>* typeLookup,
           unordered_map<std::string, string>* varLookup)
{
    if (varLookup->count(*name) == 0)
    {
        return new Token(INVALID, "", -1);
    }
    if ((*typeLookup)[*name] == INT)
    {
        return new Token(INT, (*varLookup)[*name], -1);
    }
    return new Token(STR, (*varLookup)[*name], -1);
}



Token* solveArithmetic(Token* arg1, Token* op, Token* arg2)
{
    int val1;
    int val2;
    try {
        val1 = stoi(*(arg1->getVal()));
    }
    catch (...)
    {
        cerr << "INVALID ARGUMENT: " << arg1->toString();
        return new Token(INVALID, "", -1);
    }
    try {
        val2 = stoi(*(arg2->getVal()));
    }
    catch (...)
    {
        cerr << "INVALID ARGUMENT: " << arg2->toString();
        return new Token(INVALID, "", -1);
    }
    switch(op->getTokenType())
    {
        case PLUS:
            return new Token(INT, to_string(val1 + val2), -1);
        case MINUS:
            return new Token(INT, to_string(val1 - val2), -1);
        case TIMES:
            return new Token(INT, to_string(val1 * val2), -1);
        case DIVIDE:
            return new Token(INT, to_string(val1 / val2), -1);
        case MOD:
            return new Token(INT, to_string(val1 % val2), -1);
        case MORETHAN:
            return new Token(INT, to_string(val1 > val2), -1);
        case LESSTHAN:
            return new Token(INT, to_string(val1 < val2), -1);
        case MOREEQUALS:
            return new Token(INT, to_string(val1 >= val2), -1);
        case LESSEQUALS:
            return new Token(INT, to_string(val1 <= val2), -1);
        case EQUALEQUAL:
            return new Token(INT, to_string(val1 == val2), -1);
        case NOTEQUAL:
            return new Token(INT, to_string(val1 != val2), -1);
        case AND:
            return new Token(INT, to_string(val1 && val2), -1);
        case OR:
            return new Token(INT, to_string(val1 || val2), -1);
        default:
            cerr << "INVALID OPERATOR\n";
            return new Token(INVALID, "", -1);
    }
}
void freeToken(Token* token)
{
    if (token->getLine() == -1)
    {
        //cout << "Deleting " << token->toString();
        delete(token);
    }
}

void replaceExpr(vector<Token*>* expression, int start)
{
    Token* arg1 = (*expression)[start];
    Token* op = (*expression)[start+1];
    Token* arg2 = (*expression)[start+2];
    expression->erase(expression->begin() + start);
    expression->erase(expression->begin() + start);
    expression->erase(expression->begin() + start);
    Token* ans = solveArithmetic(arg1, op, arg2);
    expression->insert(expression->begin() + start, ans);
    freeToken(arg1);
    freeToken(arg2);
    freeToken(op);
}

Token* solveExpression(vector<Token*>* expression, unordered_map<string, TokenType>* typeLookup, unordered_map<string, string>* varLookup)
{
    if (expression->empty())
    {
        cerr << "EMPTY EXPRESSION\n";
        return new Token(INVALID, "", -1);
    }
    // ----------------- Step 1: Get expressions without parenthesis -----------------
    int tIter = 0;
    vector<Token*> noParens;
    while (tIter != expression->size())
    {
        Token *thisToken = (*expression)[tIter];
        if (thisToken->getTokenType() == RPAREN)
            break;
        else if (thisToken->getTokenType() == LPAREN)
        {
            vector<Token*> recurExprsn;
            int depth = 1;
            while (depth > 0)
            {
                tIter++;
                if (tIter == expression->size())
                {
                    cerr << "MISSING END PARENTHESIS\n";
                    return new Token(INVALID, "", -1);
                }
                thisToken = (*expression)[tIter];
                if (thisToken->getTokenType() == LPAREN)
                    depth++;
                else if (thisToken->getTokenType() == RPAREN)
                    depth--;
                if (depth > 0)
                    recurExprsn.push_back(thisToken);
            }
            Token *solved = solveExpression(&recurExprsn, typeLookup, varLookup);
            if (solved->getTokenType() == INVALID)
                return solved;
            noParens.push_back(solved);
        }
        else
        {
            noParens.push_back(thisToken);
        }
        tIter++;
    }
    expression = &noParens;
    vector<Token*> noVars;
    // ----------------- Step 2: Convert variables into their values -----------------
    tIter = 0;
    while (tIter != expression->size())
    {
        Token *thisToken = (*expression)[tIter];
        if (thisToken->getTokenType() == NAME)
        {
            Token* varVal = getVar(thisToken->getVal(), typeLookup, varLookup);
            if (varVal->getTokenType() == INVALID)
            {
                cerr << "REFERENCE BEFORE ASSIGNMENT ERROR\n";
                return varVal;
            }
            noVars.push_back(varVal);
        }
        else
            noVars.push_back(thisToken);
        tIter++;
    }
    expression = &noVars;
    // ----------------- Step 3: Check if first argument is a string -----------------
    if ((*expression)[0]->getTokenType() == STR)
    {
        //Check if all even indices are strings and all odd indices are plus's, else return invalid
        tIter = 0;
        string result;
        while (tIter != expression->size())
        {
            Token *thisToken = (*expression)[tIter];
            if (tIter % 2 == 0)
            {
                if (thisToken->getTokenType() != STR and thisToken->getTokenType() != INT)
                {
                    cerr << "INVALID STRING EXPRESSION: STRING NOT FOUND WHEN EXPECTED";
                    return new Token(INVALID, "", -1);
                }
                result += *(thisToken->getVal());
                if (thisToken->getLine() == -1)
                    freeToken(thisToken);
            }
            else
            {
                if (thisToken->getTokenType() != PLUS)
                {
                    cerr << "INVALID STRING EXPRESSION: PLUS NOT FOUND WHEN EXPECTED";
                    return new Token(INVALID, "", -1);
                }
            }
            tIter++;
        }
        return new Token(STR, result, -1);
    }
    //----------------- Step 4: Check for standalone minuses -----------------

    vector<Token*> noMinuses;
    tIter = 0;
    while (tIter < expression->size()) {
        if ((*expression)[tIter]->getTokenType() == MINUS) {
            if (tIter != 0) {
                TokenType lastToken = (*expression)[tIter - 1]->getTokenType();
                if (lastToken == STR or lastToken == INT) {
                    noMinuses.push_back((*expression)[tIter]);
                    tIter++;
                    continue;
                }
            }
            if (expression->size() == tIter + 1) {
                cerr << "MINUS AT END OF STATEMENT\n";
                return new Token(INVALID, "", -1);
            }
            if ((*expression)[tIter + 1]->getTokenType() != INT) {
                cerr << "MINUS TO NONINTEGER\n";
                return new Token(INVALID, "", -1);
            }
            Token *negateMe = (*expression)[tIter + 1];
            int newVal = stoi(*(negateMe->getVal())) * -1;
            auto *negated = new Token(INT, to_string(newVal), -1);
            freeToken(negateMe);
            noMinuses.push_back(negated);
            tIter++;
        }
        else
        {

            noMinuses.push_back((*expression)[tIter]);
        }
        tIter++;
    }
    expression = &noMinuses;

    //----------------- Step 4: Check for standalone nots -----------------

    vector<Token*> noNots;
    tIter = 0;
    while (tIter < expression->size()) {
        if ((*expression)[tIter]->getTokenType() == NOT) {
            if (expression->size() == tIter + 1) {
                cerr << "NOT AT END OF STATEMENT\n";
                return new Token(INVALID, "", -1);
            }
            if ((*expression)[tIter + 1]->getTokenType() != INT) {
                cerr << "NOT TO NONINTEGER\n";
                return new Token(INVALID, "", -1);
            }
            Token *notMe = (*expression)[tIter + 1];
            int newVal = !stoi(*(notMe->getVal()));
            auto *notted = new Token(INT, to_string(newVal), -1);
            freeToken(notMe);
            noNots.push_back(notted);
            tIter++;
        }
        else
        {
            noNots.push_back((*expression)[tIter]);
        }
        tIter++;
    }
    expression = &noNots;



    if (expression->size() % 2 == 0)
    {
        cerr << "INVALID EXPRESSION COUNT\n";
        for (auto &i: *expression)
            cerr << i->toString();
        return new Token(INVALID, "", -1);
    }
    unordered_set<TokenType> ops({TIMES, DIVIDE, MOD, PLUS, MINUS, LESSEQUALS, LESSTHAN, MOREEQUALS, MORETHAN, EQUALEQUAL, NOTEQUAL, AND, OR});
    //----------------- Step 5: Check if even indices are values and odd indeces are operators -----------------
    tIter = 0;
    while (tIter < expression->size()) {
        Token* thisToken = (*expression)[tIter];
        if (tIter % 2 == 0)
        {
            if (thisToken->getTokenType() != INT and thisToken->getTokenType() != STR)
            {
                cerr << "VALUE NOT FOUND WHEN EXPECTED\n";
                return new Token(INVALID, "", -1);
            }
        }
        else
        {
            if (ops.find(thisToken->getTokenType()) == ops.end())
            {
                cerr << "OPERATOR NOT FOUND WHEN EXPECTED: " << thisToken->toString();
                return new Token(INVALID, "", -1);
            }
        }
        tIter++;
    }
    //----------------- Step 6: Evaluate the rest of the expressions in order of operation precedence -----------------
    unordered_set<TokenType> t1Ops({TIMES, DIVIDE, MOD});
    unordered_set<TokenType> t2Ops({PLUS, MINUS});
    unordered_set<TokenType> t3Ops({LESSEQUALS, LESSTHAN, MOREEQUALS, MORETHAN, EQUALEQUAL, NOTEQUAL});
    unordered_set<TokenType> t4Ops({AND, OR});
    unordered_set<TokenType>* opTiers[] = {&t1Ops, &t2Ops, &t3Ops, &t4Ops};

    for (auto validOps : opTiers)
    {
        bool noExpr = false;
        while (!noExpr)
        {
            //printVector(expression);
            noExpr = true;
            auto* reduced = new vector<Token*>;
            tIter = 0;
            while (tIter < (signed int)(expression->size()) - 2) {
                if (validOps->find((*expression)[tIter+1]->getTokenType()) != validOps->end())
                {
                    replaceExpr(expression, tIter);
                    noExpr = false;
                    //cout << "REPLACEMENT OCCURED, NOW POINTING TO " << (*expression)[tIter]->toString();;
                }
                reduced->push_back(new Token((*expression)[tIter]));
                tIter++;
            }
            while (tIter < (signed int)(expression->size()))
            {
                reduced->push_back(new Token((*expression)[tIter]));
                tIter++;
            }
            for (auto &i : *expression)
                freeToken(i);
            expression->clear();
            for (auto &t: *reduced)
            {
                expression->push_back(t);
            }
            delete(reduced);
        }

    }
//    for (int i = 1; i < expression->size(); ++i)
//    {
//        freeToken((*expression)[i]);
//    }
    Token *result = (*expression)[0];
    //cout << "RETURNING " << result->toString();
    return new Token(result->getTokenType(), *result->getVal(), -1);

    //make new vector (all vectors will be of type Token*)
    //pass 1: if token not lparen, put in vector. Else, solve recursively, put result in vector, and skip tokens that were parsed
    //pass 1: set expressions as new vector
    //before pass 2: if first token INT, solve for int expression. Else, solve for string expression
    //pass 2: convert all variables into their values
    //pass 4 (for string): Every even index should be a STR, every odd index should be a PLUS. If false, return parsing error
    //pass 5 (for string): Concatenate every even index's value. If token's line is -1, delete it from memory
    //pass 3 (for int): Search for MINUS. if first token or is directly after anything besides an INT or STR prepend an INT with value 0.
    //pass 4 (for int): make new vector, check for NOT. if NOT is found, next value should be of type INT or STR. If false, return parsing error.
    //pass 4 (for int): If STR, attempt to cast as INT. if cast fails, return parsing error.
    //pass 4 (for int): If INT, put in new vector 0 if INT nonzero, and 1 otherwise and parse forwards by 1. If INT line is -1, delete it from memory
    //pass 4 (for int): if NOT not found, put token in vector. Set expression to vector.
    //pass 5 (for int): Even indexes should be INT or STR, odd indexes should be *, /, %, +, -, <, >, <=, >=, ==, or !=. If false, return parsing error.
    //pass 6 (for int): make new vector, check for *, /, and %. If found, check tokens before and after operator to see if they are both STR or INT
    //pass 6 (for int): if STR, attempt to cast as INT. If cast fails, return parsing error.
    //pass 6 (for int): if not INT or STR, return parsing error
    //pass 6 (for int): perform operation between the two tokens and put them in vector. If either variables had line -1, delete it from memory.
    //pass 6 (for int): If operator not found, put tokens in vector. Set expression as vector.
    //pass 7 (for int): Perform same procedure as pass 5, but for + and -.
    //pass 8 (for int): Perform same procedure as pass 5, but for <, >, <=, >=, ==, and !=.
    //pass 9 (for int): There should only be one more token in expression, so return that.
}


int executePrint(vector<Token>* tokens, int tIter, unordered_map<string, TokenType>* typeLookup, unordered_map<string, string>* varLookup)
{
    vector<Token*> expression;
    while((*tokens)[tIter].getTokenType() != SEMICOLON)
    {
        if (tIter >= (tokens->size()))
            return tIter;
        expression.push_back(&(*tokens)[tIter]);
        tIter++;
    }
    Token* result = solveExpression(&expression, typeLookup, varLookup);
    if (result->getTokenType() == INVALID)
    {
        delete(result);
        return -1;
    }
    cout << *(*result).getVal() << "\n";
    delete(result);
    return tIter;
}

int executeVarInit(TokenType type, vector<Token>* tokens, int tIter, unordered_map<string, TokenType>* typeLookup, unordered_map<string, string>* varLookup)
{
    Token thisToken = (*tokens)[tIter];
    if (thisToken.getTokenType() != NAME)
    {
        cerr << "INVALID NAME\n";
        return -1;
    }
    string varName = *(thisToken.getVal());
    if (typeLookup->count(varName))
    {
        cerr << "NAME ALREADY INITIALIZED\n";
        return -1;
    }
    (*typeLookup)[varName] = type;
    tIter++;
    thisToken = (*tokens)[tIter];
    if (thisToken.getTokenType() == SEMICOLON)
        return tIter;
    else if (thisToken.getTokenType() == EQUAL)
    {
        tIter++;
        vector<Token*> expression;
        while((*tokens)[tIter].getTokenType() != SEMICOLON)
        {
            if (tIter >= (tokens->size()))
                return tIter;
            expression.push_back(&(*tokens)[tIter]);
            tIter++;
        }
        Token* val = solveExpression(&expression, typeLookup, varLookup);
        if (val->getTokenType() != type)
        {
            cerr << "INVALID VARIABLE TYPE\n";
            delete(val);
            return -1;
        }
        (*varLookup)[varName] = *(val->getVal());
        delete(val);

    }
    else return -1;
    return tIter;
}
int executeRedeclaration(vector<Token>* tokens, int tIter, unordered_map<string, TokenType>* typeLookup, unordered_map<string, string>* varLookup)
{
    Token thisToken = (*tokens)[tIter];
    if (thisToken.getTokenType() != NAME)
    {
        cerr << "INVALID VARIABLE REASSIGNMENT NAME\n";
        return -1;
    }
    string name = *(thisToken.getVal());
    if (!typeLookup->count(name))
    {
        cerr << "NAME NOT INITIALIZED\n";
        return -1;
    }
    thisToken = (*tokens)[++tIter];
    if (thisToken.getTokenType() != EQUAL)
    {
        cerr << "INVALID VARIABLE REASSIGNMENT SYNTAX\n";
        return -1;
    }
    tIter++;
    vector<Token*> expression;
    while((*tokens)[tIter].getTokenType() != SEMICOLON)
    {
        if (tIter >= (tokens->size()))
            return tIter;
        expression.push_back(&(*tokens)[tIter]);
        tIter++;
    }
    Token* val = solveExpression(&expression, typeLookup, varLookup);
    if (val->getTokenType() != (*typeLookup)[name])
    {
        cerr << "VARIABLE REASSIGNMENT TO WRONG TYPE\n";
        delete(val);
        return -1;
    }
    (*varLookup)[name] = *(val->getVal());
    delete(val);
    return tIter;
}
int executeIf(vector<Token>* tokens, int tIter, unordered_map<string, TokenType>* typeLookup, unordered_map<string, string>* varLookup, bool *lastCondition)
{
    Token lParen = (*tokens)[++tIter];
    tIter++;
    int depth = 1;
    vector<Token*> expression;
    while(depth != 0)
    {
        Token thisToken = (*tokens)[tIter];
        if (thisToken.getTokenType() == LPAREN)
            depth += 1;
        else if (thisToken.getTokenType() == RPAREN)
        {
            depth -= 1;
            if (depth == 0)
                continue;
        }
        if (tIter >= (tokens->size()))
            return tIter;
        expression.push_back(&(*tokens)[tIter]);
        tIter++;
    }
    Token *result = solveExpression(&expression, typeLookup, varLookup);
    if (result->getTokenType() == STR)
        *lastCondition = true;
    else
        *lastCondition = stoi(*(result->getVal()));
    delete(result);
    return tIter;
}
int executeInput(vector<Token>* tokens, int tIter, unordered_map<string, TokenType>* typeLookup, unordered_map<string, string>* varLookup)
{
    if (tokens->size() - tIter < 5)
    {
        cerr << "IMPROPER INPUT SYNTAX: NOT ENOUGH TOKENS\n";
        return -1;
    }
    Token lParen = (*tokens)[++tIter];
    Token var = (*tokens)[++tIter];
    Token rParen = (*tokens)[++tIter];
    Token semi = (*tokens)[++tIter];
    if (lParen.getTokenType() != LPAREN)
    {
        cerr << "MISSING LEFT PARENTHESIS AFTER INPUT\n";
        return -1;
    }
    if (!typeLookup->count(*(var.getVal())))
    {
        cerr << "INPUT BEFORE VARIABLE ASSIGNMENT\n";
        return -1;
    }
    if (rParen.getTokenType() != RPAREN) {
        cerr << "MISSING RIGHT PARENTHESIS AFTER INPUT\n";
        return -1;
    }
    //cout << semi.toString();
    if (semi.getTokenType() != SEMICOLON) {
        cerr << "MISSING SEMICOLON AFTER INPUT\n";
        return -1;
    }
    string input;
    getline(cin, input);
    string varName = *(var.getVal());
    TokenType varType = (*typeLookup)[varName];
    if (varType == INT)
    {
        try {
            input = to_string(stoi(input));
        }
        catch(const std::invalid_argument& ia)
        {
            cerr << "STRING ASSIGNMENT TO INT\n";
            return -1;
        }
    }
    (*varLookup)[varName] = input;
    return tIter;
}


Block::Block(std::vector<Token>* t, int i,
             unordered_map<string, TokenType>* typeLookup,
             unordered_map<string, std::string>* varLookup)
{
    tIter = i;
    tokens = t;
    this->typeLookup = typeLookup;
    this->varLookup = varLookup;
}
int Block::execute()
{
    int loopIndex = 0;
    bool lastCondition = false;
    TokenType lastStatement = RBRACE;
    Token thisToken = (*tokens)[tIter];
    while (thisToken.getTokenType() != RBRACE and tIter < (tokens->size()))
    {
        thisToken = (*tokens)[tIter];
        // --------------- Start of Block ---------------
        if (thisToken.getTokenType() == LBRACE)
        {
            //cout << "new block at " << thisToken.toString();
            Block newBlock(tokens, tIter + 1, typeLookup, varLookup);
            if (lastStatement == IF || lastStatement == ELSE)
            {
                if (lastCondition)
                {
                    tIter = newBlock.execute();
                }
                else
                {
                    tIter = newBlock.skip();
                }
            }
            else if (lastStatement == WHILE)
            {
                if (lastCondition)
                {
                    if (newBlock.execute() != -1)
                        tIter = loopIndex - 1;
                    else
                        tIter = -1;
                }
                else
                {
                    tIter = newBlock.skip();
                }
            }
            else
            {
                cout << "block does not follow if or while statement at " << thisToken.toString();
                tIter = newBlock.execute();
            }
            if (tIter == -1)
            {
                return -1;
            }

            //cout << "block has ended with tIter " << tIter << ", now we're at " << (*tokens)[tIter].toString();
        }
        // --------------- Print Statement ---------------
        else if (thisToken.getTokenType() == PRINT)
        {
            //cout << "print statement at " << thisToken.toString();
            tIter = executePrint(tokens, tIter+1, typeLookup, varLookup);
            if (tIter == -1)
            {
                return -1;
            }
            //cout << "PRINT DONE, NOW AT " << (*tokens)[tIter].toString();

        }
            // --------------- int declaration ---------------
        else if (thisToken.getTokenType() == INTVAR)
        {
            //cout << "init int statement at " << thisToken.toString();
            tIter = executeVarInit(INT, tokens, tIter+1, typeLookup, varLookup);
        }
            // --------------- string declaration ---------------
        else if (thisToken.getTokenType() == STRVAR)
        {
            //cout << "init str statement at " << thisToken.toString();
            tIter = executeVarInit(STR, tokens, tIter+1, typeLookup, varLookup);
        }
            // --------------- var redeclaration ---------------
        else if (thisToken.getTokenType() == NAME)
        {
            //cout << "var redeclaration statement at " << thisToken.toString();
            tIter = executeRedeclaration(tokens, tIter, typeLookup, varLookup);
        }
            // --------------- input ---------------
        else if (thisToken.getTokenType() == INPUT)
        {
            //cout << "input statement at " << thisToken.toString();
            tIter = executeInput(tokens, tIter, typeLookup, varLookup);
        }
            // --------------- if statement ---------------
        else if (thisToken.getTokenType() == IF)
        {
            //cout << "if statement at " << thisToken.toString();
            tIter = executeIf(tokens, tIter, typeLookup, varLookup, &lastCondition);
        }
        else if (thisToken.getTokenType() == WHILE)
        {
            //cout << "while statement at " << thisToken.toString();
            loopIndex = tIter;
            //cout << "loopIndex goes to " << (*tokens)[loopIndex].toString();
            tIter = executeIf(tokens, tIter, typeLookup, varLookup, &lastCondition);
        }
        else if (thisToken.getTokenType() == ELSE)
        {
            if (lastStatement == LBRACE)
            {
                lastCondition = !lastCondition;
            }
        }
//        else cout << thisToken.toString();
        if (tIter >= (tokens->size()) or tIter == -1)
        {
            return -1;
        }
        tIter++;
        lastStatement = thisToken.getTokenType();
    }
    return tIter-1;

}
int Block::skip()
{
    int depth = 1;
    while (depth != 0 and tIter < (tokens->size()))
    {
        tIter++;
        Token thisToken = (*tokens)[tIter];
        if (thisToken.getTokenType() == RBRACE)
            depth--;
        else if (thisToken.getTokenType() == LBRACE)
            depth++;
    }
    return tIter;
}

