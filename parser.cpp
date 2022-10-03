//
// Created by Ethan on 6/11/2022.
//

#include "parser.h"

parser::parser(lexer* _lexer) {
    lex = _lexer;
    curToken = lex->get_next_token();
}



ASTNode *parser::parse_expression() {
    int parenthesis_depth = 0;
    std::vector<lexerToken*> expr_tok_list;
    while(!(parenthesis_depth == 0 && (curToken->type == TOKEN_COMMA || curToken->type == TOKEN_SEMI_COLON))){
        if(curToken->type == TOKEN_EOF) {std::cout << "Expression Unfinished" << std::endl; exit(1);}
        if(curToken->type == TOKEN_LPAREN) parenthesis_depth++;
        if(curToken->type == TOKEN_RPAREN) parenthesis_depth--;
        expr_tok_list.push_back(curToken);
        eat_token(curToken->type);
    }
    return parse_partial_expression(expr_tok_list);
}

void parser::eat_token(tokenType type) {
    if(curToken->type == type){
        lexerToken* temp = prevToken;
        prevToken = curToken;
        curToken = lex->get_next_token();
    }else{
        std::cout << "Unexpected token " << curToken->contents << " with type " << curToken->type << std::endl;
        exit(1);
    }
}


bool outside_paren_match(std::vector<lexerToken*> toklist){

    if(toklist.size() < 3) return false;

    toklist.erase(toklist.begin());
    toklist.pop_back();
    int count = 0;
    for(auto t : toklist){
        if(t->type == TOKEN_RPAREN){
            count -= 1;
            if(count<0) return false;
        }
        if(t->type == TOKEN_LPAREN){
            count += 1;
        }
    }
    return true;

}

ASTNode *parser::parse_partial_expression(const std::vector<lexerToken*>& expr_tok_list) {

    std::vector<lexerToken *> clean_tok_list = expr_tok_list;
    // cleans away all the redundant parenthesis
    if(clean_tok_list.empty()){
        std::cout << "empty expression error" << std::endl;
        exit(1);
    }

    while (clean_tok_list[0]->type == TOKEN_LPAREN && clean_tok_list[clean_tok_list.size() - 1]->type == TOKEN_RPAREN &&
                                                                            outside_paren_match(clean_tok_list)){
        clean_tok_list.erase(clean_tok_list.begin());
        clean_tok_list.pop_back();
    }

    int paren_depth = 0;
    // search through all tokens that are NOT in parentheses to find || or &&
    for (int i = clean_tok_list.size() - 1; i >= 0; i--) {

        // block makes sure to ignore everything inside parenthesis
        if (clean_tok_list[i]->type == TOKEN_RPAREN) {
            paren_depth += 1;
            continue;
        }
        if (clean_tok_list[i]->type == TOKEN_LPAREN) {
            paren_depth -= 1;
            continue;
        }
        if (paren_depth > 0) continue;


        // OR OP
        if (clean_tok_list[i]->type == TOKEN_BAR) {
            if (i > 0 && clean_tok_list[i - 1]->type == TOKEN_BAR) {
                // we are pivoting on the next operation ( || ) here. left op is v[0:i-1] and right op v[i+1:v.size()]
                ASTNode *leftOp = parse_partial_expression(
                        std::vector<lexerToken *>(clean_tok_list.begin(), clean_tok_list.begin() + i - 1));
                ASTNode *rightOp = parse_partial_expression(
                        std::vector<lexerToken *>(clean_tok_list.begin() + i + 1, clean_tok_list.end()));
                auto compNode = new ASTNode(LOGIC_OP);
                compNode->operation = "||";
                compNode->leftOp = leftOp;
                compNode->rightOp = rightOp;
                return compNode;
            } else {
                std::cout << "Expected second | token after first." << std::endl;
                exit(1);
            }
        }
        // AND OP
        if (clean_tok_list[i]->type == TOKEN_AMPER) {
            if (i > 0 && clean_tok_list[i - 1]->type == TOKEN_AMPER) {
                // we are pivoting on the next operation ( || ) here. left op is v[0:i-1] and right op v[i+1:v.size()]
                ASTNode *leftOp = parse_partial_expression(
                        std::vector<lexerToken *>(clean_tok_list.begin(), clean_tok_list.begin() + i - 1));
                ASTNode *rightOp = parse_partial_expression(
                        std::vector<lexerToken *>(clean_tok_list.begin() + i + 1, clean_tok_list.end()));
                auto compNode = new ASTNode(LOGIC_OP);
                compNode->operation = "&&";
                compNode->leftOp = leftOp;
                compNode->rightOp = rightOp;
                return compNode;
            } else {
                std::cout << "Expected second & token after first." << std::endl;
                exit(1);
            }
        }

    }
    //
    for (int i = clean_tok_list.size() - 1; i >= 0; i--) {
        // block makes sure to ignore everything inside parenthesis
        if (clean_tok_list[i]->type == TOKEN_RPAREN) {
            paren_depth += 1;
            continue;
        }
        if (clean_tok_list[i]->type == TOKEN_LPAREN) {
            paren_depth -= 1;
            continue;
        }
        if (paren_depth > 0) continue;

        // possible ops: != == > < <= >=

        if(clean_tok_list[i]->type == TOKEN_EQUALS){
            if(clean_tok_list[i - 1]->type == TOKEN_EQUALS){
                // ==
                ASTNode *leftOp = parse_partial_expression(
                        std::vector<lexerToken *>(clean_tok_list.begin(), clean_tok_list.begin() + i - 1));
                ASTNode *rightOp = parse_partial_expression(
                        std::vector<lexerToken *>(clean_tok_list.begin() + i + 1, clean_tok_list.end()));
                auto compNode = new ASTNode(COMPARE_OP);
                compNode->operation = "==";
                compNode->leftOp = leftOp;
                compNode->rightOp = rightOp;
                return compNode;
            }
            if(clean_tok_list[i - 1]->type == TOKEN_EXCLAMATION){
                // !=
                ASTNode *leftOp = parse_partial_expression(
                        std::vector<lexerToken *>(clean_tok_list.begin(), clean_tok_list.begin() + i - 1));
                ASTNode *rightOp = parse_partial_expression(
                        std::vector<lexerToken *>(clean_tok_list.begin() + i + 1, clean_tok_list.end()));
                auto compNode = new ASTNode(COMPARE_OP);
                compNode->operation = "!=";
                compNode->leftOp = leftOp;
                compNode->rightOp = rightOp;
                return compNode;
            }
            if(clean_tok_list[i - 1]->type == TOKEN_GREATER_THAN){
                // >=
                ASTNode *leftOp = parse_partial_expression(
                        std::vector<lexerToken *>(clean_tok_list.begin(), clean_tok_list.begin() + i - 1));
                ASTNode *rightOp = parse_partial_expression(
                        std::vector<lexerToken *>(clean_tok_list.begin() + i + 1, clean_tok_list.end()));
                auto compNode = new ASTNode(COMPARE_OP);
                compNode->operation = ">=";
                compNode->leftOp = leftOp;
                compNode->rightOp = rightOp;
                return compNode;
            }
            if(clean_tok_list[i - 1]->type == TOKEN_LESS_THAN){
                // >=
                ASTNode *leftOp = parse_partial_expression(
                        std::vector<lexerToken *>(clean_tok_list.begin(), clean_tok_list.begin() + i - 1));
                ASTNode *rightOp = parse_partial_expression(
                        std::vector<lexerToken *>(clean_tok_list.begin() + i + 1, clean_tok_list.end()));
                auto compNode = new ASTNode(COMPARE_OP);
                compNode->operation = "<=";
                compNode->leftOp = leftOp;
                compNode->rightOp = rightOp;
                return compNode;
            }
        }
        if(clean_tok_list[i]->type == TOKEN_GREATER_THAN){
            // >
            ASTNode *leftOp = parse_partial_expression(
                    std::vector<lexerToken *>(clean_tok_list.begin(), clean_tok_list.begin() + i));
            ASTNode *rightOp = parse_partial_expression(
                    std::vector<lexerToken *>(clean_tok_list.begin() + i + 1, clean_tok_list.end()));
            auto compNode = new ASTNode(COMPARE_OP);
            compNode->operation = ">";
            compNode->leftOp = leftOp;
            compNode->rightOp = rightOp;
            return compNode;
        }
        if(clean_tok_list[i]->type == TOKEN_LESS_THAN){
            // <
            ASTNode *leftOp = parse_partial_expression(
                    std::vector<lexerToken *>(clean_tok_list.begin(), clean_tok_list.begin() + i));
            ASTNode *rightOp = parse_partial_expression(
                    std::vector<lexerToken *>(clean_tok_list.begin() + i + 1, clean_tok_list.end()));
            auto compNode = new ASTNode(COMPARE_OP);
            compNode->operation = "<";
            compNode->leftOp = leftOp;
            compNode->rightOp = rightOp;
            return compNode;
        }
    }

    for (int i = clean_tok_list.size() - 1; i >= 0; i--) {
        // block makes sure to ignore everything inside parenthesis
        if (clean_tok_list[i]->type == TOKEN_RPAREN) {
            paren_depth += 1;
            continue;
        }
        if (clean_tok_list[i]->type == TOKEN_LPAREN) {
            paren_depth -= 1;
            continue;
        }
        if (paren_depth > 0) continue;

        if(clean_tok_list[i]->type == TOKEN_PLUS){
            // +
            ASTNode *leftOp = parse_partial_expression(
                    std::vector<lexerToken *>(clean_tok_list.begin(), clean_tok_list.begin() + i));
            ASTNode *rightOp = parse_partial_expression(
                    std::vector<lexerToken *>(clean_tok_list.begin() + i + 1, clean_tok_list.end()));
            auto compNode = new ASTNode(BINARY_OP);
            compNode->operation = "+";
            compNode->leftOp = leftOp;
            compNode->rightOp = rightOp;
            return compNode;
        }

        if(clean_tok_list[i]->type == TOKEN_MINUS){
            // -
            ASTNode *leftOp = parse_partial_expression(
                    std::vector<lexerToken *>(clean_tok_list.begin(), clean_tok_list.begin() + i));
            ASTNode *rightOp = parse_partial_expression(
                    std::vector<lexerToken *>(clean_tok_list.begin() + i + 1, clean_tok_list.end()));
            auto compNode = new ASTNode(BINARY_OP);
            compNode->operation = "-";
            compNode->leftOp = leftOp;
            compNode->rightOp = rightOp;
            return compNode;
        }
    }

    for (int i = clean_tok_list.size() - 1; i >= 0; i--) {
        // block makes sure to ignore everything inside parenthesis
        if (clean_tok_list[i]->type == TOKEN_RPAREN) {
            paren_depth += 1;
            continue;
        }
        if (clean_tok_list[i]->type == TOKEN_LPAREN) {
            paren_depth -= 1;
            continue;
        }
        if (paren_depth > 0) continue;

        if(clean_tok_list[i]->type == TOKEN_DASH){
            // *
            ASTNode *leftOp = parse_partial_expression(
                    std::vector<lexerToken *>(clean_tok_list.begin(), clean_tok_list.begin() + i));
            ASTNode *rightOp = parse_partial_expression(
                    std::vector<lexerToken *>(clean_tok_list.begin() + i + 1, clean_tok_list.end()));
            auto compNode = new ASTNode(BINARY_OP);
            compNode->operation = "*";
            compNode->leftOp = leftOp;
            compNode->rightOp = rightOp;
            return compNode;
        }

        if(clean_tok_list[i]->type == TOKEN_F_SLASH){
            // /
            ASTNode *leftOp = parse_partial_expression(
                    std::vector<lexerToken *>(clean_tok_list.begin(), clean_tok_list.begin() + i));
            ASTNode *rightOp = parse_partial_expression(
                    std::vector<lexerToken *>(clean_tok_list.begin() + i + 1, clean_tok_list.end()));
            auto compNode = new ASTNode(BINARY_OP);
            compNode->operation = "/";
            compNode->leftOp = leftOp;
            compNode->rightOp = rightOp;
            return compNode;
        }
    }

    for (int i = clean_tok_list.size() - 1; i >= 0; i--) {
        // block makes sure to ignore everything inside parenthesis
        if (clean_tok_list[i]->type == TOKEN_RPAREN) {
            paren_depth += 1;
            continue;
        }
        if (clean_tok_list[i]->type == TOKEN_LPAREN) {
            paren_depth -= 1;
            continue;
        }
        if (paren_depth > 0) continue;

        if (clean_tok_list[i]->type == TOKEN_DASH) {
            if (clean_tok_list[i-1]->type == TOKEN_DASH) {
                // **
                ASTNode *leftOp = parse_partial_expression(
                        std::vector<lexerToken *>(clean_tok_list.begin(), clean_tok_list.begin() + i - 1));
                ASTNode *rightOp = parse_partial_expression(
                        std::vector<lexerToken *>(clean_tok_list.begin() + i + 1, clean_tok_list.end()));
                auto compNode = new ASTNode(BINARY_OP);
                compNode->operation = "**";
                compNode->leftOp = leftOp;
                compNode->rightOp = rightOp;
                return compNode;
            }
        }
    }


    if(clean_tok_list.size() == 1){
        lexerToken* tok = clean_tok_list[0];
        if(tok->type == TOKEN_NUMERIC){
            auto compNode = new ASTNode(NUMERIC_LITERAL);
            compNode->double_value = std::stod(tok->contents);
            return compNode;
        }
        if(tok->type == TOKEN_STRING){
            auto compNode = new ASTNode(STRING_LITERAL);
            compNode->string_value = tok->contents;
            return compNode;
        }
        if(tok->type == TOKEN_ID){
            if(tok->contents == "false"){
                auto compNode = new ASTNode(BOOL_LITERAL);
                compNode->bool_value = false;
                return compNode;
            }
            if(tok->contents == "true"){
                auto compNode = new ASTNode(BOOL_LITERAL);
                compNode->bool_value = true;
                return compNode;
            }

            // not true of false literals so must be variable name
            auto compNode = new ASTNode(VAR_REFERENCE);
            compNode->variable_name = tok->contents;
            return compNode;
        }
    }

    // not a single value, and no ops can be done so only valid syntax left means this must be a function call.

    // ensure that the first token is ID, second token is ( and last token is )
    if(clean_tok_list[0]->type != TOKEN_ID){
        std::cout << "invalid expression (1)" << std::endl;
        exit(1);
    }
    if(clean_tok_list[1]->type != TOKEN_LPAREN){
        std::cout << "invalid expression (2)" << std::endl;
        exit(1);
    }
    if(clean_tok_list[clean_tok_list.size() - 1]->type != TOKEN_RPAREN){
        std::cout << "invalid expression (3)" << std::endl;
        exit(1);
    }

    // if all met then it's a function call
    auto compNode = new ASTNode(FUNC_CALL);
    compNode->function_call_name = (*clean_tok_list.begin())->contents;
    std::vector<lexerToken*> func_params = std::vector<lexerToken *>(clean_tok_list.begin() + 2, clean_tok_list.end() - 1);
    std::vector<lexerToken*> single_param;
    int p_depth = 0;
    for(auto & func_param : func_params){
        if(func_param->type == TOKEN_LPAREN){
            p_depth += 1;
            single_param.push_back(func_param);
            continue;
        }
        if(func_param->type == TOKEN_RPAREN){
            p_depth -=1;
            single_param.push_back(func_param);
            continue;
        }
        if(func_param->type == TOKEN_COMMA && p_depth == 0){
            ASTNode* param_head = parse_partial_expression(single_param);
            compNode->function_args.push_back(param_head);
            single_param.clear();
        }else{
            single_param.push_back(func_param);
        }
    }
    if(!func_params.empty()){
        compNode->function_args.push_back(parse_partial_expression(single_param));
    }
    return compNode;
}

ASTNode *parser::parse_code_block() {
    return NULL;
}

