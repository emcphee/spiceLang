//
// Created by Ethan on 6/11/2022.
//

#ifndef SPICELANG_PARSER_H
#define SPICELANG_PARSER_H
#include "ASTNode.h"
#include "lexer.h"
#include <vector>
#include <iostream>
#include "misc.h"
#include <stack>
class parser {
public:
    explicit parser(lexer* _lexer);

    lexer* lex;
    lexerToken* curToken;
    lexerToken* prevToken{};

    void eat_token(tokenType type);

    std::vector<ASTNode*> function_refs;

    ASTNode* parse_code_block();

    ASTNode* parse_expression();
    ASTNode* parse_partial_expression(const std::vector<lexerToken*>& expr_tok_list);
};


#endif //SPICELANG_PARSER_H
