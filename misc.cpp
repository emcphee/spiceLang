//
// Created by Ethan on 6/10/2022.
//

#include "misc.h"

std::string file_to_string(const std::string& filepath){
    std::ifstream t(filepath);
    std::stringstream buffer;
    buffer << t.rdbuf();
    return buffer.str();
}

void print_token_type(enum tokenType type){
    switch(type){
        case TOKEN_ID: std::cout << "TOKEN_ID" << std::endl; break;
        case TOKEN_STRING: std::cout << "TOKEN_STRING" << std::endl; break;
        case TOKEN_NUMERIC: std::cout << "TOKEN_NUMERIC" << std::endl; break;
        case TOKEN_LPAREN: std::cout << "TOKEN_LPAREN" << std::endl; break;
        case TOKEN_RPAREN: std::cout << "TOKEN_RPAREN" << std::endl; break;
        case TOKEN_LCURLY: std::cout << "TOKEN_LCURLY" << std::endl; break;
        case TOKEN_RCURLY: std::cout << "TOKEN_RCURLY" << std::endl; break;
        case TOKEN_SEMI_COLON: std::cout << "TOKEN_SEMI_COLON" << std::endl; break;
        case TOKEN_EQUALS: std::cout << "TOKEN_EQUALS" << std::endl; break;
        case TOKEN_EOF: std::cout << "TOKEN_EOF" << std::endl; break;
        case TOKEN_GREATER_THAN: std::cout << "TOKEN_GREATER_THAN" << std::endl; break;
        case TOKEN_LESS_THAN: std::cout << "TOKEN_LESS_THAN" << std::endl; break;
        case TOKEN_PLUS: std::cout << "TOKEN_PLUS" << std::endl; break;
        case TOKEN_MINUS: std::cout << "TOKEN_MINUS" << std::endl; break;
        case TOKEN_DASH: std::cout << "TOKEN_DASH" << std::endl; break;
        case TOKEN_F_SLASH: std::cout << "TOKEN_F_SLASH" << std::endl; break;
        case TOKEN_B_SLASH: std::cout << "TOKEN_B_SLASH" << std::endl; break;
        case TOKEN_COMMA: std::cout << "TOKEN_COMMA" << std::endl; break;
        case TOKEN_EXCLAMATION: std::cout << "TOKEN_EXCLAMATION" << std::endl; break;
        default:
            std::cout << "Unlabeled token in print_node_type() internal function." << std::endl;
    }
}

void print_AST_tree(ASTNode* head){
    print_AST_tree_helper(head, 0);
}
void print_AST_tree_helper(ASTNode* node, int depth){
    for(int i = 0; i < depth; i++){
        std::cout << "   ";
    }
    if(node->type == ASSIGNMENT){
        std::string initialized;
        if(node->variable_initialized){initialized="true";} else {initialized="false";}
        std::cout << "ASSIGNMENT : init=" << initialized << std::endl;
        if(node->variable_initialized)print_AST_tree_helper(node->variable_contents,depth+1);
    }
    if(node->type == VAR_REFERENCE){
        std::cout << "VAR_REFERENCE : " << node->variable_name << std::endl;
    }
    if(node->type == FUNC_CALL){
        std::cout << "FUNC_CALL : " << node->function_call_name << std::endl;
        for(auto & function_arg : node->function_args){
            print_AST_tree_helper(function_arg, depth + 1);
        }
    }
    if(node->type == BOOL_LITERAL){
        std::cout << "BOOL_LITERAL : " << node->bool_value << std::endl;
    }
    if(node->type == STRING_LITERAL){
        std::cout << "STRING_LITERAL : " << node->string_value << std::endl;
    }
    if(node->type == NUMERIC_LITERAL){
        std::cout << "NUMERIC_LITERAL : " << node->double_value << std::endl;
    }
    if(node->type == CODE_BLOCK){
        for(auto n : node->compound_value){
            print_AST_tree_helper(n, depth + 1);
        }
    }
    if(node->type == BINARY_OP){
        std::cout << "BINARY OP : " << node->operation << std::endl;
        print_AST_tree_helper(node->leftOp, depth + 1);
        print_AST_tree_helper(node->rightOp, depth + 1);
    }
    if(node->type == COMPARE_OP){
        std::cout << "COMPARE OP : " << node->operation << std::endl;
        print_AST_tree_helper(node->leftOp, depth + 1);
        print_AST_tree_helper(node->rightOp, depth + 1);
    }
    if(node->type == LOGIC_OP){
        std::cout << "LOGIC OP : " << node->operation << std::endl;
        print_AST_tree_helper(node->leftOp, depth + 1);
        print_AST_tree_helper(node->rightOp, depth + 1);
    }
}