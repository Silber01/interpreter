#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <map>
#include "Token.h"
#include "Block.h"
using namespace std;

void fillInstMap(map<string, TokenType>* instMap)
{
    instMap->insert({"print", PRINT});
    instMap->insert({"input", INPUT});
    instMap->insert({"int", INTVAR});
    instMap->insert({"string", STRVAR});
    instMap->insert({"if", IF});
    instMap->insert({"else", ELSE});
    instMap->insert({"while", WHILE});
    instMap->insert({";", SEMICOLON});
    instMap->insert({"+", PLUS});
    instMap->insert({"-", MINUS});
    instMap->insert({"*", TIMES});
    instMap->insert({"/", DIVIDE});
    instMap->insert({"%", MOD});
    instMap->insert({"(", LPAREN});
    instMap->insert({")", RPAREN});
    instMap->insert({"{", LBRACE});
    instMap->insert({"}", RBRACE});
    instMap->insert({"&&", AND});
    instMap->insert({"||", OR});


}
int getNextToken(vector<Token>* tokens, string* fileText, string::iterator start, map<string, TokenType>* instMap, int line)
{
    switch (*start) {
        case ' ':
        case '\t':
        case '\n':
            return 1;
    }
    if (*start == '\"') //string literal
    {
        int strStart = (int)(start - fileText->begin()) + 1;
        int length = 0;
        start++;
        while (*start != '\"')
        {
            if (start == fileText->end())
            {
                return -1;
            }
            length++;
            start++;
        }
        Token strLit(STR, fileText->substr(strStart, length), line);
        tokens->push_back(strLit);
        return length + 2;
    }
    for (auto & it : *instMap) //instruction
    {
        if (*fileText == it.first)
        {
            Token instr(it.second, it.first, line);
            tokens->push_back(instr);
            return (int) it.first.length();
        }
        else if ((*fileText).rfind(it.first, (int) (start - (*fileText).begin())) == (start - (*fileText).begin())) {
            if (isalpha(*start))
            {
                char nextChar = *(start + (int)it.first.length());
                if (nextChar != ' ' and nextChar != '(' and nextChar != '\n')
                    continue;
            }

            Token instr(it.second, it.first, line);
            tokens->push_back(instr);
            return (int) it.first.length();
        }

    }
    if (isdigit(*start)) //number
    {
        int numStart = (int)(start - fileText->begin());
        int length = 1;
        start++;
        while(isdigit(*start))
        {
            length++;
            start++;
        }
        Token num(INT, fileText->substr(numStart, length), line);
        tokens->push_back(num);
        return length;
    }
    if (isalpha(*start)) //name
    {
        int nameStart = (int)(start - fileText->begin());
        int length = 1;
        start++;
        while(isdigit(*start) || isalpha(*start))
        {
            length++;
            start++;
        }
        Token name(NAME, fileText->substr(nameStart, length), line);
        tokens->push_back(name);
        return length;
    }
    if (*start == '=') //operator
    {
        if (*(start + 1) == '=')
        {
            Token me(EQUALEQUAL, "==", line);
            tokens->push_back(me);
            return 2;
        }
        Token mt(EQUAL, "=", line);
        tokens->push_back(mt);
        return 1;
    }
    if (*start == '<') //operator
    {
        if (*(start + 1) == '=')
        {
            Token le(LESSEQUALS, "<=", line);
            tokens->push_back(le);
            return 2;
        }
        Token lt(LESSTHAN, "<", line);
        tokens->push_back(lt);
        return 1;
    }
    if (*start == '>') //operator
    {
        if (*(start + 1) == '=')
        {
            Token me(MOREEQUALS, ">=", line);
            tokens->push_back(me);
            return 2;
        }
        Token mt(MORETHAN, ">", line);
        tokens->push_back(mt);
        return 1;
    }
    if (*start == '!') //operator
    {
        if (*(start + 1) == '=')
        {
            Token ne(NOTEQUAL, "!=", line);
            tokens->push_back(ne);
            return 2;
        }
        Token nt(NOT, "!", line);
        tokens->push_back(nt);
        return 1;
    }


    return 1;
}
void getTokens(string* lineText, vector<Token>* tokens, int line, map<string, TokenType>* instMap) {
    string::iterator fileIter = lineText->begin();
    while (fileIter < lineText->end()) {
        int parseResult = getNextToken(tokens, lineText, fileIter, instMap, line);
        if (parseResult > 0)
            fileIter += parseResult;
        else
        {
            tokens->clear();
            Token er(INVALID, "Invalid Token", line);
            (*tokens).push_back(er);
            return;
        }
    }
}
void printLogo()
{
    cout << "      ::::::::   :::::::: ::::::::::: :::  " << endl;
    cout << "    :+:    :+: :+:    :+:    :+:     :+:   " << endl;
    cout << "   +:+        +:+           +:+     +:+    " << endl;
    cout << "  +#+        +#++:++#++    +#+     +#+     " << endl;
    cout << " +#+               +#+    +#+     +#+      " << endl;
    cout << "#+#    #+# #+#    #+#    #+#     #+#       " << endl;
    cout << "########   ######## ########### ########## " << endl;
}
int main() {
    printLogo();
    map<string, TokenType> instMap;
    fillInstMap(&instMap);
    fstream programFile;
    programFile.open("../script.txt", ios::in);
    string fileText;
    string textLine;
    auto* tokens = new vector<Token>;
    int line = 1;
    if (programFile.is_open())
    {
        while(getline(programFile, textLine))
        {
            size_t lineLen = textLine.length();
            int start = 0;
            while(isspace(textLine[start]))
                start += 1;
            textLine = textLine.substr(start, lineLen - start);
            getTokens(&textLine, tokens, line, &instMap);
                if ((*tokens)[0].getTokenType() == INVALID)
                {
                    cout << "Error on line " << line << ": " << (*tokens)[0].getVal();
                    return -1;
                }
            line += 1;
        }
        programFile.close();
    }
    else return -1;

//    cout << "TOKENIZED CODE:\n";
//    for (auto & token : *tokens)
//    {
//        cout << token.toString();
//    }
//    cout << "\n\nEXECUTED CODE:\n";
    unordered_map<string, TokenType> typeLookup;
    unordered_map<string, string> varLookup;
    Block testBlock(tokens, 0, &typeLookup, &varLookup);
    if (testBlock.execute() == -1)
    {
        cout << "Parsing error\n";
        return -1;
    }
//    cout << "Code Compiled.\n";
}
